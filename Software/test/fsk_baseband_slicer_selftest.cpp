// Native self-test for fsk_baseband_slicer.h -- simulates the CD74HC4046APWR loop filter's
// analog output (slew-rate-limited two-level signal, offset from VCO free-run tolerance,
// plus noise) and checks the slicer recovers the original bits.
// Build: g++ -O2 -std=c++17 -I.. fsk_baseband_slicer_selftest.cpp -o selftest_baseband
#include "fsk_baseband_slicer.h"
#include <cstdio>
#include <vector>
#include <random>
#include <cmath>

static bool runCase(const char *name, uint32_t sample_rate_hz, uint32_t baud, float dc_offset,
                     float loop_filter_cutoff_hz, float noise_amplitude, const std::vector<bool> &bits_in) {
    std::mt19937 rng(42);
    std::normal_distribution<float> noise(0.0f, noise_amplitude);

    FskBasebandSlicer slicer(sample_rate_hz, baud, /*signal_lpf_cutoff_hz=*/baud * 2.0f,
                              /*center_tracking_cutoff_hz=*/baud / 8.0f);

    uint32_t samples_per_bit = sample_rate_hz / baud;
    // Slew-rate-limit the ideal mark/space square wave through a one-pole filter to model
    // the PLL loop filter's own bandwidth (it can't jump instantly between levels).
    float loop_alpha = 1.0f - std::exp(-2.0f * 3.14159265358979323846f * loop_filter_cutoff_hz / sample_rate_hz);
    float loop_state = dc_offset;

    std::vector<bool> bits_out;
    for (bool bit : bits_in) {
        float target = dc_offset + (bit ? 300.0f : -300.0f); // ADC counts around center
        for (uint32_t n = 0; n < samples_per_bit; n++) {
            loop_state += loop_alpha * (target - loop_state);
            float sample = loop_state + noise(rng);
            int32_t adc_code = (int32_t)std::lround(sample);

            bool out_bit;
            if (slicer.processSample(adc_code, &out_bit)) {
                bits_out.push_back(out_bit);
            }
        }
    }

    size_t mismatches = 0;
    size_t compare_n = std::min(bits_in.size(), bits_out.size());
    for (size_t i = 0; i < compare_n; i++) {
        if (bits_in[i] != bits_out[i]) mismatches++;
    }

    printf("[%s] fs=%u baud=%u dc_offset=%.0f loop_bw=%.0fHz noise=%.0f  bits_in=%zu bits_out=%zu mismatches=%zu\n",
           name, sample_rate_hz, baud, dc_offset, loop_filter_cutoff_hz, noise_amplitude,
           bits_in.size(), bits_out.size(), mismatches);
    return mismatches == 0 && bits_out.size() == bits_in.size();
}

int main() {
    std::vector<bool> pattern = {1,0,1,1,0,0,1,0,1,0,1,1,1,0,0,0,1,1,0,1};

    bool ok = true;
    // Clean signal, well-centered VCO.
    ok &= runCase("A: clean, centered", 20000, 2400, 0.0f, 12000.0f, 5.0f, pattern);
    // VCO free-run offset from nominal (component tolerance) -- center tracking should absorb it.
    ok &= runCase("B: 150-count DC offset", 20000, 2400, 150.0f, 12000.0f, 5.0f, pattern);
    // Heavier noise.
    ok &= runCase("C: noisy", 20000, 2400, 50.0f, 12000.0f, 60.0f, pattern);
    // Slower loop filter (tighter PLL bandwidth) -- more inter-symbol smearing.
    ok &= runCase("D: narrow loop BW", 20000, 2400, 0.0f, 6000.0f, 5.0f, pattern);

    printf(ok ? "ALL PASS\n" : "FAIL\n");
    return ok ? 0 : 1;
}
