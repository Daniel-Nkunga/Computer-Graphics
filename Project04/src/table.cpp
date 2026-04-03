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
    glBegin(GL_POLYGON);
    glColor3f(0.0, 1.0, 1.0);
    for(int i = 0; i < 8; i++)
    {
        float theta = (M_PI/4)*i;
        float x = size*cos(theta);
        float z = size*sin(theta);
        glVertex3f(x, midy, z);
        cout << size << " (" << x << ", " << "0.0" << ", " << z << ")" << ", " << theta << endl;
    }
    glEnd();
}