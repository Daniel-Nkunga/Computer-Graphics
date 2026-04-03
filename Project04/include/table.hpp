#ifndef TABLE_H
#define TABLE_H

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
extern GLuint textureID;

void init_texture(char *name, unsigned char *&texture, int &xdim, int &ydim);

class Table
{
protected:
    float midx, midy, midz, size;
public:
    Table(float _midx, float _midy, float _midz, float _size);
    ~Table () {};

    void draw();
};
#endif