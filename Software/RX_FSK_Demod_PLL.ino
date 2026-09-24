/*
 * 50MHz RC Receiver - FSK bit recovery for the PLL-discriminator RX architecture.
 *
 * Hardware: LT5560 mixer -> 10.7MHz IF filter -> CD74HC4046APWR PLL discriminator ->
 * RC low-pass -> ESP32-S3 ADC. See RX_Hardware/RX_Design_Details.md section 3 for why
 * this replaced the original IF-undersampling plan (ESP32-S3 ADC continuous mode tops
 * out at 83.3kSPS -- fine for the FSK signal, but 30x too slow to Nyquist-sample the
 * 230kHz-wide IF filter output; the classic single-chip FM-IF ICs that would have avoided
 * this -- MC3361/MC3362/MC3372, SA604A, NJM2211 -- are all confirmed obsolete as of
 * 2026-09-22).
 *
 * The 4046's loop filter output is already demodulated FSK audio (two DC-ish levels, not
 * two frequencies), so this is just a low-rate baseband sample + bit slicer -- no NCO/DDC
 * needed (contrast with the superseded RX_FSK_Demod.ino). Ordinary analogRead() polling
 * at a modest rate is sufficient; no continuous/DMA ADC driver required.
 */

#include <Arduino.h>
#include "fsk_baseband_slicer.h"

#ifndef ADC_IF_PIN
#define ADC_IF_PIN 4  // ASSUMED -- update once the schematic's ADC_IF net is pinned to a real GPIO
#endif

#define SAMPLE_RATE_HZ  20000u  // ~8x oversample at 2400 baud; well within analogRead() polling rate
#define FSK_BAUD        2400u

static hw_timer_t *s_sample_timer = nullptr;
static volatile bool s_sample_due = false;

static FskBasebandSlicer s_slicer(SAMPLE_RATE_HZ, FSK_BAUD,
                                   /*signal_lpf_cutoff_hz=*/FSK_BAUD * 2.0f,
                                   /*center_tracking_cutoff_hz=*/FSK_BAUD / 8.0f);

void IRAM_ATTR onSampleTimer() {
    s_sample_due = true;
}

void setup() {
    Serial.begin(115200);
    analogReadResolution(12);
    analogSetPinAttenuation(ADC_IF_PIN, ADC_11db);

    s_sample_timer = timerBegin(SAMPLE_RATE_HZ);
    timerAttachInterrupt(s_sample_timer, &onSampleTimer);
    timerAlarm(s_sample_timer, 1, true, 0);
}

void loop() {
    if (!s_sample_due) return;
    s_sample_due = false;

    int32_t raw = analogRead(ADC_IF_PIN);
    bool bit;
    if (s_slicer.processSample(raw, &bit)) {
        // TODO: hand `bit` to a bit/byte assembler + the ELRS-style packet framer
        // described in Firmware_Implementation.md section 3.
    }
}
