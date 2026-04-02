#ifndef CUBE_H
#define CUBE_H

#include <iostream>
#include <vector>
#include <cmath>
#include <numeric>
#include <algorithm>
#include <random>

using namespace std;

// Texture mapping variables (defined in cube.cpp, shared via extern)
extern int xdim, ydim;
extern unsigned char *texture;

void init_texture(char *name, unsigned char *&texture, int &xdim, int &ydim);

class Cube
{
protected:
    float midx, midy, midz, size;
    float rotationX, thetaX, rotationY, thetaY;
    float gravityModifier;

    void applyGravity();
public:
    Cube(float _midx, float _midy, float _midz, float _size);
    virtual ~Cube() {};

    void draw();
    virtual void update();
    static Cube createRandomCube();
};

class StationaryCube : public Cube
{
public:
    using Cube::Cube;
    void update() override;
};

#endif