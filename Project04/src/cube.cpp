// Basic imports
#include <cmath>

// OpenGL Functions
#include <GL/freeglut_std.h>
#include <GL/gl.h>
#include <GL/glu.h>
#ifdef MAC
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include "libim/im_color.h"

// Custom classes
#include <cube.hpp>

float GRAVITY  = 0.1f;
int SPAWN_RADIUS = 15;

// Texture mapping variables (shared via extern in cube.hpp)
int xdim, ydim;// add with the other globals
unsigned char *texture;
GLuint textureID = 0;

Cube::Cube(float _midx, float _midy, float _midz, float _size)
{
    midx = _midx;
    midy = _midy;
    midz = _midz;
    size = _size;
    rotationX = 0.0f;
    thetaX = 0.75 + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(2.25-(0.75))));
    rotationY = 0.0f;
    thetaY = 0.75 + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(2.25-(0.75))));
    gravityModifier = 0.75 + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(1.25-(0.75))));

}

StationaryCube::StationaryCube(float _midx, float _midy, float _midz, float _size,
                                float _rotationX, float _rotationY, float _rotationZ)
    : Cube(_midx, _midy, _midz, _size)
{
    rotationX = _rotationX;
    rotationY = _rotationY;
    rotationZ = _rotationZ;
}

void Cube::draw()
{
    glBindTexture(GL_TEXTURE_2D, textureID);

    // Each face is 1/6th of the sprite strip width
    float w = 1.0f / 6.0f;

    glPushMatrix();
    glTranslatef(midx, midy, midz);
    glRotatef(rotationX, 1.0f, 0.0f, 0.0f);
    glRotatef(rotationY, 0.0f, 1.0f, 0.0f);
    glRotatef(rotationZ, 0.0f, 0.0f, 1.0f);
    glTranslatef(-midx, -midy, -midz);

    // Define 8 vertices
    float ax = midx - size / 2, ay = midy - size / 2, az = midz + size / 2;
    float bx = midx + size / 2, by = midy - size / 2, bz = midz + size / 2;
    float cx = midx + size / 2, cy = midy + size / 2, cz = midz + size / 2;
    float dx = midx - size / 2, dy = midy + size / 2, dz = midz + size / 2;
    float ex = midx - size / 2, ey = midy - size / 2, ez = midz - size / 2;
    float fx = midx + size / 2, fy = midy - size / 2, fz = midz - size / 2;
    float gx = midx + size / 2, gy = midy + size / 2, gz = midz - size / 2;
    float hx = midx - size / 2, hy = midy + size / 2, hz = midz - size / 2;

    // Side 1 - top
    glBegin(GL_POLYGON);
    glColor3f(1.0, 1.0, 1.0);
    glTexCoord2f(0*w, 0.0); glVertex3f(ax, ay, az);
    glTexCoord2f(1*w, 0.0); glVertex3f(bx, by, bz);
    glTexCoord2f(1*w, 1.0); glVertex3f(cx, cy, cz);
    glTexCoord2f(0*w, 1.0); glVertex3f(dx, dy, dz);
    glEnd();

    // Side 5 - left
    glBegin(GL_POLYGON);
    glColor3f(1.0, 1.0, 1.0);
    glTexCoord2f(4*w, 0.0); glVertex3f(ax, ay, az);
    glTexCoord2f(5*w, 0.0); glVertex3f(dx, dy, dz);
    glTexCoord2f(5*w, 1.0); glVertex3f(hx, hy, hz);
    glTexCoord2f(4*w, 1.0); glVertex3f(ex, ey, ez);
    glEnd();

    // Side 6 - back
    glBegin(GL_POLYGON);
    glColor3f(1.0, 1.0, 1.0);
    glTexCoord2f(5*w, 0.0); glVertex3f(fx, fy, fz);
    glTexCoord2f(6*w, 0.0); glVertex3f(ex, ey, ez);
    glTexCoord2f(6*w, 1.0); glVertex3f(hx, hy, hz);
    glTexCoord2f(5*w, 1.0); glVertex3f(gx, gy, gz);
    glEnd();

    // Side 2 - right
    glBegin(GL_POLYGON);
    glColor3f(1.0, 1.0, 1.0);
    glTexCoord2f(1*w, 0.0); glVertex3f(bx, by, bz);
    glTexCoord2f(2*w, 0.0); glVertex3f(cx, cy, cz);
    glTexCoord2f(2*w, 1.0); glVertex3f(gx, gy, gz);
    glTexCoord2f(1*w, 1.0); glVertex3f(fx, fy, fz);
    glEnd();

    // Side 4 - bottom
    glBegin(GL_POLYGON);
    glColor3f(1.0, 1.0, 1.0);
    glTexCoord2f(3*w, 0.0); glVertex3f(ax, ay, az);
    glTexCoord2f(4*w, 0.0); glVertex3f(bx, by, bz);
    glTexCoord2f(4*w, 1.0); glVertex3f(fx, fy, fz);
    glTexCoord2f(3*w, 1.0); glVertex3f(ex, ey, ez);
    glEnd();

    // Side 3 - top
    glBegin(GL_POLYGON);
    glColor3f(1.0, 1.0, 1.0);
    glTexCoord2f(2*w, 0.0); glVertex3f(dx, dy, dz);
    glTexCoord2f(3*w, 0.0); glVertex3f(cx, cy, cz);
    glTexCoord2f(3*w, 1.0); glVertex3f(gx, gy, gz);
    glTexCoord2f(2*w, 1.0); glVertex3f(hx, hy, hz);
    glEnd();

    glPopMatrix();
}

void Cube::applyGravity()
{
    midy -= GRAVITY * gravityModifier;
}

void Cube::update()
{
    rotationX += thetaX;
    rotationY += thetaY;
    applyGravity();
    if(midy <= -10)
    {
        *this = createRandomCube();
    }
}

void StationaryCube::update()
{
    rotationX += thetaX / 3;
    // rotationY += thetaY;
    // applyGravity();
    // if(midy <= -10)
    // {
    //     *this = createRandomCube();
    // }
}

Cube Cube::createRandomCube()
{
    int low = -8;
    int high = 6;
    float INNER_RADIUS = 5.0f;

    float initX, initY, initZ, size;

    do {
        initX = (-1*SPAWN_RADIUS) + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (SPAWN_RADIUS * 3)));
        initZ = (-1*SPAWN_RADIUS) + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (SPAWN_RADIUS * 3)));
    } while (initX * initX + initZ * initZ < INNER_RADIUS * INNER_RADIUS);

    initY = low + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (high - low)));
    size  = static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / 0.5));

    return Cube(initX, initY, initZ, size);
}

void init_texture(char *name, unsigned char *&texture, int &xdim, int &ydim)
{
   // Read jpg image
   im_color image;
   image.ReadJpg(name);
   printf("Reading image %s\n", name);
   xdim = 1; while (xdim < image.R.Xdim) xdim*=2; xdim /=2;
   ydim = 1; while (ydim < image.R.Ydim) ydim*=2; ydim /=2;
   image.Interpolate(xdim, ydim);
   printf("Interpolating %dx%d to %dx%d\n", 
      image.R.Xdim, image.R.Ydim, xdim, ydim);

   // Copy image into texture array
   texture = (unsigned char *)malloc((unsigned int)(xdim*ydim*3));
   int index = 0;
   for (int y = 0; y < ydim; y++)
        for (int x = 0; x < xdim; x++)
        {
            texture[index++] = (unsigned char)(image.R.Data2D[y][x]);
            texture[index++] = (unsigned char)(image.G.Data2D[y][x]);
            texture[index++] = (unsigned char)(image.B.Data2D[y][x]);
        }
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, xdim, ydim, 0, GL_RGB, GL_UNSIGNED_BYTE, texture);
}