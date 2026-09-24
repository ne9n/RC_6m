// Platform-independent FSK/FM quadrature discriminator core.
// No Arduino/ESP-IDF dependency -- feed it raw ADC sample values via processSample().
#pragma once

#include <cstdint>
#include <cmath>

#ifndef FSK_DEMOD_PI
#define FSK_DEMOD_PI 3.14159265358979323846
#endif

class FskDemod {
public:
    FskDemod(uint32_t sample_rate_hz, uint32_t if_freq_hz, uint32_t baud_rate, float lpf_cutoff_hz)
        : lpf_alpha_(1.0f - std::exp(-2.0f * float(FSK_DEMOD_PI) * lpf_cutoff_hz / float(sample_rate_hz))),
          samples_per_bit_(sample_rate_hz / baud_rate) {
        buildSineTable();
        nco_increment_ = (uint32_t)(((uint64_t)if_freq_hz << 32) / sample_rate_hz);
    }

    void reset() {
        nco_phase_ = 0;
        i_lpf_ = q_lpf_ = 0.0f;
        i_prev_ = q_prev_ = 0.0f;
        have_prev_ = false;
        sample_count_in_bit_ = 0;
        dtheta_accum_ = 0.0f;
    }

    // adc_sample: raw ADC code (e.g. 0..4095 for 12-bit). dc_offset: mid-scale value to subtract
    // (measure this at power-up with no signal, or use a running DC tracker).
    // Returns true when a full symbol period has been integrated; *bit_out then holds the decoded bit.
    bool processSample(int32_t adc_sample, int32_t dc_offset, bool *bit_out) {
        float x = float(adc_sample - dc_offset);

        uint32_t idx = nco_phase_ >> (32 - NCO_TABLE_BITS);
        int16_t sin_val = sine_table_[idx];
        int16_t cos_val = sine_table_[(idx + NCO_TABLE_SIZE / 4) & (NCO_TABLE_SIZE - 1)];
        nco_phase_ += nco_increment_;

        float i_raw = x * (cos_val / 32768.0f);
        float q_raw = -x * (sin_val / 32768.0f);

        i_lpf_ += lpf_alpha_ * (i_raw - i_lpf_);
        q_lpf_ += lpf_alpha_ * (q_raw - q_lpf_);

        if (have_prev_) {
            float dtheta = std::atan2(i_lpf_ * q_prev_ - q_lpf_ * i_prev_,
                                       i_lpf_ * i_prev_ + q_lpf_ * q_prev_);
            dtheta_accum_ += dtheta;
        }
        i_prev_ = i_lpf_;
        q_prev_ = q_lpf_;
        have_prev_ = true;

        if (++sample_count_in_bit_ >= samples_per_bit_) {
            *bit_out = (dtheta_accum_ < 0.0f);
            dtheta_accum_ = 0.0f;
            sample_count_in_bit_ = 0;
            return true;
        }
        return false;
    }

private:
    static constexpr uint32_t NCO_TABLE_BITS = 10;
    static constexpr uint32_t NCO_TABLE_SIZE = 1u << NCO_TABLE_BITS;

    static void buildSineTable() {
        static bool built = false;
        if (built) return;
        for (uint32_t i = 0; i < NCO_TABLE_SIZE; i++) {
            sine_table_[i] = (int16_t)std::lround(32767.0 * std::sin(2.0 * FSK_DEMOD_PI * i / NCO_TABLE_SIZE));
        }
        built = true;
    }

    static int16_t sine_table_[NCO_TABLE_SIZE];

    uint32_t nco_phase_ = 0;
    uint32_t nco_increment_;

    float lpf_alpha_;
    float i_lpf_ = 0.0f, q_lpf_ = 0.0f;

    float i_prev_ = 0.0f, q_prev_ = 0.0f;
    bool have_prev_ = false;

    uint32_t samples_per_bit_;
    uint32_t sample_count_in_bit_ = 0;
    float dtheta_accum_ = 0.0f;
};

int16_t FskDemod::sine_table_[FskDemod::NCO_TABLE_SIZE];
