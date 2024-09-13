#pragma once

#define __cplusplus 201103L
#include <array>

namespace WaveLUT {

    template <unsigned int N>
    struct SineWave {
        public:
        std::array<int, N> v_lut = {};
        SineWave(int amplitude_div) {
            for (int i = 0; i < N; ++i) {
                v_lut[i] = int(amplitude_div * (sin(2.0 * M_PI * i / N) + 1.0));
            }
        }
    };

    template <unsigned int N>
    struct SawWave {
        public:
        std::array<int, N> v_lut = {};
        SawWave(int amplitude_div, float rising_fraction) {
            if (rising_fraction < 0.0 || rising_fraction > 1.0) {
                rising_fraction = 0.5;
            }
            int turning_pt = int(N * rising_fraction);
            int max_val_div = amplitude_div * 2;
            for (int i = 0; i < turning_pt; ++i) {
                v_lut[i] = int(max_val_div * i / turning_pt);
            }
            for (int i = turning_pt; i < N; ++i) {
                v_lut[i] = int(max_val_div * (N - i) / turning_pt);
            }
        }
    };

    template <unsigned int N>
    struct SquareWave {
        public:
        std::array<int, N> v_lut = {};
        SquareWave(int amplitude_div, float low_fraction) {
            if (low_fraction < 0.0 || low_fraction > 1.0) {
                low_fraction = 0.5;
            }
            int turning_pt = int(N * low_fraction);
            int max_val_div = amplitude_div * 2;
            for (int i = 0; i < turning_pt; ++i) {
                v_lut[i] = 0;
            }
            for (int i = turning_pt; i < N; ++i) {
                v_lut[i] = max_val_div;
            }
        }
    };

}