// Native self-test for fsk_demod_core.h -- no Arduino/ESP-IDF needed.
// Build:  g++ -O2 -std=c++17 -I.. fsk_demod_selftest.cpp -o selftest
// Run:    ./selftest        (exits 0 and prints ALL PASS on success)
#define FSK_DEMOD_PI 3.14159265358979323846
#include "fsk_demod_core.h"
#include <cstdio>
#include <vector>
#include <random>

struct TestCase {
    const char *name;
    uint32_t sample_rate_hz;
    uint32_t if_freq_hz;
    uint32_t deviation_hz;
    uint32_t baud;
    float noise_amplitude; // fraction of signal amplitude
};

static bool runCase(const TestCase &tc, const std::vector<bool> &bits_in) {
    std::mt19937 rng(1234);
    std::normal_distribution<float> noise(0.0f, tc.noise_amplitude * 1800.0f);

    FskDemod demod(tc.sample_rate_hz, tc.if_freq_hz, tc.baud, /*lpf_cutoff_hz=*/tc.baud * 3.0f);

    uint32_t samples_per_bit = tc.sample_rate_hz / tc.baud;
    double phase = 0.0;
    const double two_pi = 2.0 * FSK_DEMOD_PI;

    std::vector<bool> bits_out;
    for (bool bit : bits_in) {
        double f_inst = double(tc.if_freq_hz) + (bit ? double(tc.deviation_hz) : -double(tc.deviation_hz));
        double phase_inc = two_pi * f_inst / tc.sample_rate_hz;
        for (uint32_t n = 0; n < samples_per_bit; n++) {
            float amplitude = 1800.0f; // ADC counts, comfortably inside 12-bit range around mid-scale
            float sample = amplitude * std::sin(phase) + noise(rng);
            int32_t adc_code = 2048 + (int32_t)std::lround(sample);
            phase += phase_inc;
            if (phase > two_pi) phase -= two_pi;

            bool out_bit;
            if (demod.processSample(adc_code, 2048, &out_bit)) {
                bits_out.push_back(out_bit);
            }
        }
    }

    printf("  in: ");
    for (bool b : bits_in) printf("%d", b ? 1 : 0);
    printf("\n out: ");
    for (bool b : bits_out) printf("%d", b ? 1 : 0);
    printf("\n");

    size_t mismatches = 0;
    size_t compare_n = std::min(bits_in.size(), bits_out.size());
    for (size_t i = 0; i < compare_n; i++) {
        if (bits_in[i] != bits_out[i]) mismatches++;
    }

    printf("[%s] fs=%u Hz if=%u Hz dev=%u Hz baud=%u  bits_in=%zu bits_out=%zu mismatches=%zu\n",
           tc.name, tc.sample_rate_hz, tc.if_freq_hz, tc.deviation_hz, tc.baud,
           bits_in.size(), bits_out.size(), mismatches);
    return mismatches == 0 && bits_out.size() == bits_in.size();
}

int main() {
    std::vector<bool> pattern = {1,0,1,1,0,0,1,0,1,0,1,1,1,0,0,0,1,1,0,1};

    // Sanity check the core algorithm at a generous sample rate.
    TestCase caseA{"A: 1MSPS sanity", 1000000, 100000, 5000, 2400, 0.02f};

    // Realistic ESP32-S3 ADC continuous-mode ceiling (SOC_ADC_SAMPLE_FREQ_THRES_HIGH = 83333 Hz),
    // quarter-IF placement so the 10.7MHz real IF aliases to Fs/4 (see RX_FSK_Demod.ino).
    uint32_t fs_b = 83106; // 4*10.7MHz/515
    TestCase caseB{"B: 83.1kSPS quarter-IF", fs_b, fs_b / 4, 5000, 2400, 0.02f};

    // Same, with heavier noise, to see margin.
    TestCase caseC{"C: 83.1kSPS quarter-IF, noisy", fs_b, fs_b / 4, 5000, 2400, 0.15f};

    bool ok = true;
    ok &= runCase(caseA, pattern);
    ok &= runCase(caseB, pattern);
    ok &= runCase(caseC, pattern);

    printf(ok ? "ALL PASS\n" : "FAIL\n");
    return ok ? 0 : 1;
}
