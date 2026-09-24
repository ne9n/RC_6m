/*
 * 50MHz RC Receiver - IF-undersampling FSK demodulator (ESP32-S3)
 *
 * Implements RX_Design_Details.md / Firmware_Implementation.md section 2:
 *   Undersample IF -> digital down-conversion (I/Q) -> phase discriminator -> bit slicer.
 *
 * IMPORTANT -- read before wiring this to the real IF filter:
 * The ESP32-S3's ADC continuous-mode driver is hardware-capped at
 * SOC_ADC_SAMPLE_FREQ_THRES_HIGH = 83,333 Hz (esp32s3/soc_caps.h), not the 2.5Msps the
 * design doc assumed. That's fine for the FSK signal itself (Carson's-rule bandwidth is
 * only ~15kHz at 5kHz deviation / 2400 baud), but the BOM's 10.7MHz ceramic filter
 * (Murata SFE, ~230kHz bandwidth) is far wider than this ADC can Nyquist-sample without
 * aliasing -- everything within that 230kHz window folds into the decoded signal as noise.
 * Before relying on this: either narrow the IF filter to roughly match the FSK signal's
 * occupied bandwidth (~20-30kHz), or add an analog discriminator ahead of the ADC instead
 * of undersampling the raw IF. See chat discussion for the tradeoff.
 *
 * Library note: no off-the-shelf FSK/DSP library is used. Arduino's analogContinuous()
 * only returns *averaged* results per callback (see esp32-hal-adc.h) -- unusable for
 * per-sample I/Q down-conversion -- so this talks to the IDF adc_continuous driver
 * directly. Requires arduino-esp32 core 3.x (IDF 5.x); the classic core 2.x bundled by
 * the stock PlatformIO "espressif32" registry package predates this driver on the S3.
 */

#include <Arduino.h>
#include "esp_adc/adc_continuous.h"
#include "fsk_demod_core.h"

// ASSUMED, per the schematic's own note ("ASSUMED (not specified in source docs): ESP32
// GPIO nums for TR_SW, RX_EN, ADC_IF"). Must be an ESP32-S3 ADC1 pin (GPIO1-10). Update
// once the schematic's ADC_IF net is pinned to a real GPIO.
#ifndef ADC_IF_PIN
#define ADC_IF_PIN 4
#endif

#define SAMPLE_RATE_HZ    83106u              // 4*10.7MHz/515 -- see quarter-IF derivation below
#define IF_ALIAS_HZ       (SAMPLE_RATE_HZ / 4) // true 10.7MHz IF aliases here by construction
#define FSK_DEVIATION_HZ  5000u                // matches Si5351_GFSK_Driver.cpp
#define FSK_BAUD          2400u

/*
 * Quarter-IF sample rate derivation:
 * Bandpass/undersampling requires Fs = 4*Fif/(2k+1) for some integer k, which places the
 * alias exactly at Fs/4 -- no NCO frequency search needed, the ratio is exact by construction.
 * Solving for the largest Fs <= 83333 Hz with Fif = 10.7MHz gives k=257 -> Fs = 83106.8 Hz.
 */

static adc_continuous_handle_t s_adc_handle = nullptr;
static FskDemod s_demod(SAMPLE_RATE_HZ, IF_ALIAS_HZ, FSK_BAUD, FSK_BAUD * 3.0f);
static int32_t s_dc_offset = 2048; // TODO: measure at boot with RX_EN low, or track with a slow LPF

static bool IRAM_ATTR onConvDone(adc_continuous_handle_t handle, const adc_continuous_evt_data_t *edata, void *user_data) {
    // Just needs to exist for the driver's internal signaling; actual sample handling
    // happens in pollAdcAndDemod() via adc_continuous_read().
    return false;
}

static void setupContinuousAdc() {
    adc_unit_t unit;
    adc_channel_t channel;
    ESP_ERROR_CHECK(adc_continuous_io_to_channel(ADC_IF_PIN, &unit, &channel));

    adc_continuous_handle_cfg_t handle_cfg = {
        .max_store_buf_size = 4096,
        .conv_frame_size = 256,
    };
    ESP_ERROR_CHECK(adc_continuous_new_handle(&handle_cfg, &s_adc_handle));

    static adc_digi_pattern_config_t pattern[1] = {};
    pattern[0].atten = ADC_ATTEN_DB_11;
    pattern[0].channel = channel;
    pattern[0].unit = unit;
    pattern[0].bit_width = SOC_ADC_DIGI_MAX_BITWIDTH;

    adc_continuous_config_t dig_cfg = {
        .pattern_num = 1,
        .adc_pattern = pattern,
        .sample_freq_hz = SAMPLE_RATE_HZ,
        .conv_mode = ADC_CONV_SINGLE_UNIT_1,
        .format = ADC_DIGI_OUTPUT_FORMAT_TYPE2,
    };
    ESP_ERROR_CHECK(adc_continuous_config(s_adc_handle, &dig_cfg));

    adc_continuous_evt_cbs_t cbs = { .on_conv_done = onConvDone };
    ESP_ERROR_CHECK(adc_continuous_register_event_callbacks(s_adc_handle, &cbs, nullptr));

    ESP_ERROR_CHECK(adc_continuous_start(s_adc_handle));
}

static uint8_t s_read_buf[1024];

void pollAdcAndDemod() {
    uint32_t out_len = 0;
    esp_err_t err = adc_continuous_read(s_adc_handle, s_read_buf, sizeof(s_read_buf), &out_len, 0);
    if (err != ESP_OK) return;

    for (uint32_t i = 0; i < out_len; i += SOC_ADC_DIGI_RESULT_BYTES) {
        adc_digi_output_data_t *p = (adc_digi_output_data_t *)&s_read_buf[i];
        int32_t raw = p->type2.data;
        bool bit;
        if (s_demod.processSample(raw, s_dc_offset, &bit)) {
            // TODO: hand `bit` to a bit/byte assembler + the ELRS-style packet framer
            // described in Firmware_Implementation.md section 3.
        }
    }
}

void setup() {
    Serial.begin(115200);
    setupContinuousAdc();
}

void loop() {
    pollAdcAndDemod();
}
