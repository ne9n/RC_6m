// Baseband FSK bit slicer for the PLL-discriminator RX architecture (CD74HC4046APWR).
// The 4046's loop filter output is already demodulated FSK audio -- mark/space appear as
// two DC-ish levels around a center point, not two frequencies -- so no NCO/DDC/atan2
// discriminator is needed here (contrast with fsk_demod_core.h, written for the earlier
// IF-undersampling architecture that turned out not to fit the ESP32-S3's ADC ceiling).
#pragma once

#include <cstdint>
#include <cmath>

class FskBasebandSlicer {
public:
    // sample_rate_hz: ADC sampling rate feeding processSample().
    // baud_rate: FSK symbol rate.
    // signal_lpf_cutoff_hz: smooths ADC/comparator noise; keep above ~2x baud to avoid
    //   smearing bit transitions.
    // center_tracking_cutoff_hz: slow LPF that estimates the mark/space midpoint, so the
    //   slicer isn't thrown off by VCO free-run drift/tolerance; keep well below baud rate.
    FskBasebandSlicer(uint32_t sample_rate_hz, uint32_t baud_rate,
                       float signal_lpf_cutoff_hz, float center_tracking_cutoff_hz)
        : signal_alpha_(alphaFor(signal_lpf_cutoff_hz, sample_rate_hz)),
          center_alpha_(alphaFor(center_tracking_cutoff_hz, sample_rate_hz)),
          samples_per_bit_(sample_rate_hz / baud_rate) {}

    void reset() {
        signal_lpf_ = 0.0f;
        center_lpf_ = 0.0f;
        have_center_ = false;
        sample_count_in_bit_ = 0;
        accum_ = 0.0f;
    }

    // Returns true when a full symbol period has been integrated; *bit_out then holds the
    // decoded bit (true = mark/high, matching the TX side's "bit ? +deviation : -deviation").
    bool processSample(int32_t adc_sample, bool *bit_out) {
        float x = float(adc_sample);

        signal_lpf_ += signal_alpha_ * (x - signal_lpf_);

        if (!have_center_) {
            center_lpf_ = signal_lpf_;
            have_center_ = true;
        } else {
            center_lpf_ += center_alpha_ * (signal_lpf_ - center_lpf_);
        }

        accum_ += (signal_lpf_ - center_lpf_);

        if (++sample_count_in_bit_ >= samples_per_bit_) {
            *bit_out = (accum_ > 0.0f);
            accum_ = 0.0f;
            sample_count_in_bit_ = 0;
            return true;
        }
        return false;
    }

private:
    static float alphaFor(float cutoff_hz, uint32_t sample_rate_hz) {
        return 1.0f - std::exp(-2.0f * 3.14159265358979323846f * cutoff_hz / float(sample_rate_hz));
    }

    float signal_alpha_;
    float center_alpha_;
    uint32_t samples_per_bit_;

    float signal_lpf_ = 0.0f;
    float center_lpf_ = 0.0f;
    bool have_center_ = false;

    uint32_t sample_count_in_bit_ = 0;
    float accum_ = 0.0f;
};
