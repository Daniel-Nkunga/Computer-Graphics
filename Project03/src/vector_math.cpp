#include <tuple>
#include <cmath>
#include "vector_math.hpp"

using namespace std;

tuple<float,float,float> cross(float ax, float ay, float az,
                                float bx, float by, float bz)
{
    return { ay*bz - az*by,
             az*bx - ax*bz,
             ax*by - ay*bx };
}

tuple<float,float,float> normalise(float x, float y, float z)
{
    float len = sqrtf(x*x + y*y + z*z);
    if (len < 1e-6f) return {0.f, 0.f, 1.f};
    return { x/len, y/len, z/len };
}

float dot(float ax, float ay, float az,
          float bx, float by, float bz)
{
    return ax*bx + ay*by + az*bz;
}