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

// Custom classes
#include <cube.hpp>

#define GRAVITY 0.15f;
int SPAWN_RADIUS = 15;


Cube::Cube(float _midx, float _midy, float _midz, float _size)
{
    midx = _midx;
    midy = _midy;
    midz = _midz;
    size = _size;
    rotation = 0.0f;
    gravityModifier = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/2));

}

void Cube::draw()
{
    // Define 8 vertices
   float ax = midx - size / 2;
   float ay = midy - size / 2;
   float az = midz + size / 2;
   float bx = midx + size / 2;
   float by = midy - size / 2;
   float bz = midz + size / 2;
   float cx = midx + size / 2;
   float cy = midy + size / 2;
   float cz = midz + size / 2;
   float dx = midx - size / 2;
   float dy = midy + size / 2;
   float dz = midz + size / 2;
   float ex = midx - size / 2;
   float ey = midy - size / 2;
   float ez = midz - size / 2;
   float fx = midx + size / 2;
   float fy = midy - size / 2;
   float fz = midz - size / 2;
   float gx = midx + size / 2;
   float gy = midy + size / 2;
   float gz = midz - size / 2;
   float hx = midx - size / 2;
   float hy = midy + size / 2;
   float hz = midz - size / 2;

    // Draw 6 faces
    glBegin(GL_POLYGON);
    glColor3f(1.0, 0.0, 0.0);
    glVertex3f(ax, ay, az);
    glVertex3f(bx, by, bz);
    glVertex3f(cx, cy, cz);
    glVertex3f(dx, dy, dz);
    glEnd();

    glBegin(GL_POLYGON);
    glColor3f(0.0, 1.0, 0.0);
    glVertex3f(ax, ay, az);
    glVertex3f(dx, dy, dz);
    glVertex3f(hx, hy, hz);
    glVertex3f(ex, ey, ez);
    glEnd();

    glBegin(GL_POLYGON);
    glColor3f(0.0, 0.0, 1.0);
    glVertex3f(ax, ay, az);
    glVertex3f(ex, ey, ez);
    glVertex3f(fx, fy, fz);
    glVertex3f(bx, by, bz);
    glEnd();

    glBegin(GL_POLYGON);
    glColor3f(0.0, 1.0, 1.0);
    glVertex3f(gx, gy, gz);
    glVertex3f(fx, fy, fz);
    glVertex3f(ex, ey, ez);
    glVertex3f(hx, hy, hz);
    glEnd();

    glBegin(GL_POLYGON);
    glColor3f(1.0, 0.0, 1.0);
    glVertex3f(gx, gy, gz);
    glVertex3f(cx, cy, cz);
    glVertex3f(bx, by, bz);
    glVertex3f(fx, fy, fz);
    glEnd();

    glBegin(GL_POLYGON);
    glColor3f(1.0, 1.0, 0.0);
    glVertex3f(gx, gy, gz);
    glVertex3f(hx, hy, hz);
    glVertex3f(dx, dy, dz);
    glVertex3f(cx, cy, cz);
    glEnd();
}

void Cube::apply_gravity()
{
    midy -= GRAVITY;
}

void Cube::update()
{
    apply_gravity();
    if(midy <= -10)
    {
        *this = createRandomCube();
    }
    // draw();
}

Cube Cube::createRandomCube()
{
    int low = -8;
    int high = 6;
    float initX, initY, initZ, size;
    initX = (-1*SPAWN_RADIUS) + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/((SPAWN_RADIUS*3))));
    initY = low + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(high-(low))));
    initZ = (-1*SPAWN_RADIUS) + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/((SPAWN_RADIUS*3))));
    size = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/0.5));

    // cout << "(" << initX << ", " << initY << ", " << initZ << ", " << size << ")" << endl;
    return Cube(initX, initY, initZ, size);
}