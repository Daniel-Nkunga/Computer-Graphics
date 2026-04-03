// Basic imports
#include <cmath>
#include <math.h>

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
#include <table.hpp>

Table::Table(float _midx, float _midy, float _midz, float _size)
{
    midx = _midx;
    midy = _midy;
    midz = _midz;
    size = _size;
}

void Table::draw()
{
    const int segments = 8;
    const float topY    = midy;
    const float bottomY = midy - 0.2f;

    // Precompute vertices once, reuse across all three surfaces
    float x[segments], z[segments];
    for (int i = 0; i < segments; i++)
    {
        float theta = (2*M_PI / segments) * i;
        x[i] = size * cos(theta);
        z[i] = size * sin(theta);
    }

    // Table Top
    glBegin(GL_POLYGON);
    glColor3f(180.0, 97.0, 1.0);
    for (int i = 0; i < segments; i++)
        glVertex3f(x[i], topY, z[i]);
    glEnd();

    // Table Sides — GL_QUAD_STRIP pairs top/bottom vertices naturally
    glBegin(GL_QUAD_STRIP);
    glColor3f(180.0*.9, 97.0*.9, 1.0*.9);
    for (int i = 0; i < segments; i++)
    {
        glVertex3f(x[i], topY,    z[i]);  // top edge
        glVertex3f(x[i], bottomY, z[i]);  // bottom edge
    }
    // Close the strip by repeating the first two vertices
    glVertex3f(x[0], topY,    z[0]);
    glVertex3f(x[0], bottomY, z[0]);
    glEnd();

    // Table Bottom
    glBegin(GL_POLYGON);
    for (int i = 0; i < segments; i++)
        glVertex3f(x[i], bottomY, z[i]);
    glEnd();
}