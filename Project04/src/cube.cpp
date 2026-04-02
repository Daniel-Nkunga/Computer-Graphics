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

float GRAVITY  = 0.1f;
int SPAWN_RADIUS = 15;


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

void Cube::draw()
{
    glPushMatrix();
    glTranslatef(midx, midy, midz);
    glRotatef(rotationX, 1.0f, 0.0f, 0.0f);
    glRotatef(rotationY, 0.0f, 1.0f, 0.0f);
    glTranslatef(-midx, -midy, -midz);

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