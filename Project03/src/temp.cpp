#include <iostream>
#include <vector>
#include <cmath>
#include <numeric>
#include <algorithm>
#include <random>

// ---------------------------------------------------------------------------
// Perlin Noise Generator
// ---------------------------------------------------------------------------
// Generates a 2-D grid of Perlin noise values in the range [-1, 1].
//
// Usage:
//   PerlinNoise pn(seed);
//   std::vector<std::vector<double>> grid = pn.generate(size, scale, octaves,
//                                                       persistence, lacunarity);
// ---------------------------------------------------------------------------

class PerlinNoise {
public:
    explicit PerlinNoise(uint32_t seed = 42) {
        // Fill permutation table with 0..255, shuffle, then duplicate it
        perm_.resize(256);
        std::iota(perm_.begin(), perm_.end(), 0);
        std::default_random_engine rng(seed);
        std::shuffle(perm_.begin(), perm_.end(), rng);
        perm_.insert(perm_.end(), perm_.begin(), perm_.end()); // duplicate
    }

    // Generate a (size x size) grid of noise values.
    //
    // Parameters:
    //   size        – number of pixels per side
    //   scale       – "zoom level"; smaller = more zoomed out (more features)
    //   octaves     – layers of noise stacked together (detail levels)
    //   persistence – amplitude multiplier per octave  (typically 0.5)
    //   lacunarity  – frequency multiplier per octave  (typically 2.0)
    //
    // Returns values normalised to [0, 1] for convenience.
    std::vector<std::vector<double>> generate(
        int    size        = 256,
        double scale       = 50.0,
        int    octaves     = 6,
        double persistence = 0.5,
        double lacunarity  = 2.0) const
    {
        std::vector<std::vector<double>> grid(size, std::vector<double>(size, 0.0));

        double maxAmp = 0.0;
        double amp    = 1.0;
        for (int o = 0; o < octaves; ++o) { maxAmp += amp; amp *= persistence; }

        for (int y = 0; y < size; ++y) {
            for (int x = 0; x < size; ++x) {
                double value    = 0.0;
                double freq     = 1.0;
                amp             = 1.0;

                for (int o = 0; o < octaves; ++o) {
                    double sx = (x / scale) * freq;
                    double sy = (y / scale) * freq;
                    value += noise(sx, sy) * amp;
                    amp   *= persistence;
                    freq  *= lacunarity;
                }

                // Normalise to [0, 1]
                grid[y][x] = (value / maxAmp + 1.0) * 0.5;
            }
        }
        return grid;
    }

private:
    std::vector<int> perm_;

    // --- internal helpers ---------------------------------------------------

    static double fade(double t) {
        // Smoothstep: 6t^5 - 15t^4 + 10t^3
        return t * t * t * (t * (t * 6.0 - 15.0) + 10.0);
    }

    static double lerp(double a, double b, double t) {
        return a + t * (b - a);
    }

    // Gradient: map hash to one of 8 directions in 2-D
    static double grad(int hash, double x, double y) {
        switch (hash & 7) {
            case 0: return  x + y;
            case 1: return -x + y;
            case 2: return  x - y;
            case 3: return -x - y;
            case 4: return  x;
            case 5: return -x;
            case 6: return  y;
            case 7: return -y;
            default: return 0.0; // unreachable
        }
    }

    double noise(double x, double y) const {
        // Grid cell coordinates
        int xi = static_cast<int>(std::floor(x)) & 255;
        int yi = static_cast<int>(std::floor(y)) & 255;

        // Local coordinates inside the cell
        double xf = x - std::floor(x);
        double yf = y - std::floor(y);

        // Fade curves
        double u = fade(xf);
        double v = fade(yf);

        // Hash the four corners
        int aa = perm_[perm_[xi    ] + yi    ];
        int ab = perm_[perm_[xi    ] + yi + 1];
        int ba = perm_[perm_[xi + 1] + yi    ];
        int bb = perm_[perm_[xi + 1] + yi + 1];

        // Interpolate
        double x1 = lerp(grad(aa, xf,       yf    ),
                         grad(ba, xf - 1.0, yf    ), u);
        double x2 = lerp(grad(ab, xf,       yf - 1.0),
                         grad(bb, xf - 1.0, yf - 1.0), u);
        return lerp(x1, x2, v);
    }
};

// ---------------------------------------------------------------------------
// Demo: print a small ASCII visualisation and dump raw values
// ---------------------------------------------------------------------------
// int main() {
//     const int    SIZE        = 64;
//     const double SCALE       = 20.0;
//     const int    OCTAVES     = 5;
//     const double PERSISTENCE = 0.5;
//     const double LACUNARITY  = 2.0;
//     const uint32_t SEED      = 1337;

//     PerlinNoise pn(SEED);
//     auto grid = pn.generate(SIZE, SCALE, OCTAVES, PERSISTENCE, LACUNARITY);

//     // ASCII visualisation (64×64 fits nicely in a terminal)
//     const std::string CHARS = " .:-=+*#%@";
//     std::cout << "Perlin Noise (" << SIZE << "x" << SIZE << "):\n";
//     for (const auto& row : grid) {
//         for (double v : row) {
//             int idx = static_cast<int>(v * (CHARS.size() - 1));
//             std::cout << CHARS[idx] << CHARS[idx]; // doubled for aspect ratio
//         }
//         std::cout << '\n';
//     }

//     // Print the first 5×5 raw values
//     std::cout << "\nRaw values [0,1] (top-left 5x5):\n";
//     for (int y = 0; y < 5; ++y) {
//         for (int x = 0; x < 5; ++x)
//             std::cout << grid[y][x] << '\t';
//         std::cout << '\n';
//     }

//     return 0;
// }