#ifndef CUBE_H
#define CUBE_H

// Imports
#include <iostream>
#include <vector>
#include <cmath>
#include <numeric>
#include <algorithm>
#include <random>

using namespace std;

class Cube
{
private:
    float midx, midy, midz, size;
    float rotation;

    void apply_gravity();
public:
    Cube(float _midx, float _midy, float _midz, float _size);
    ~Cube() {};

    void draw();
    void update();

};

#endif