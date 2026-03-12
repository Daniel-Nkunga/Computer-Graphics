#include <perlin.hpp>

double PerlinNoise::fade(double t)
{
    return t * t * t * (t * (t * 6.0 - 15.0) + 10.0);
}

double PerlinNoise::lerp(double a, double b, double t)
{
    return a + t * (b - a);
}

double PerlinNoise::noise(double x, double y) const
{
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