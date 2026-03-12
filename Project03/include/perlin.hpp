#ifndef PERLIN_H
#define PERLIN_H

// Imports
#include <iostream>
#include <vector>
#include <cmath>
#include <numeric>
#include <algorithm>
#include <random>

using namespace std;

class PerlinNoise
{
    public:
        explicit PerlinNoise(uint32_t seed = 42) {
            // Fill permutation table with 0..255, shuffle, then duplicate it
            perm_.resize(256);
            std::iota(perm_.begin(), perm_.end(), 0);
            std::default_random_engine rng(seed);
            std::shuffle(perm_.begin(), perm_.end(), rng);
            perm_.insert(perm_.end(), perm_.begin(), perm_.end()); // duplicate
        }

        vector<vector<double>> generate(
            int    size        = 256,
            double scale       = 50.0,
            int    octaves     = 6,
            double persistence = 0.5,
            double lacunarity  = 2.0) const
        {
            vector<vector<double>> grid(size, vector<double>(size, 0.0));

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
        vector<int> perm_;

        // Smoothstep
        static double fade(double t);

        // Linear interpolation
        static double lerp(double a, double b, double t);

        // Gradient map
        static double grad(int hash, double x, double y)
        {
            switch (hash & 7)
            {
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

        // Noise generation
        double noise(double x, double y) const;
};

#endif