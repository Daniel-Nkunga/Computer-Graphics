#ifndef VECTOR_MATH_H
#define VECTOR_MATH_H

#include <tuple>

using namespace std;

tuple<float,float,float> cross(float ax, float ay, float az,
                                float bx, float by, float bz);

tuple<float,float,float> normalise(float x, float y, float z);

float dot(float ax, float ay, float az,
          float bx, float by, float bz);

#endif