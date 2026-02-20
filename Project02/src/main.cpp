#include <GL/freeglut_std.h>
#include <GL/gl.h>
#include <iostream>
#include <tuple>
#include <vector>
#include <cmath>
#include <stdio.h>
#include <stdlib.h>
#ifdef MAC
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
using namespace std;

// Custom classes
#include <polygon.hpp>
#include <pacman.hpp>
#include <pellet.hpp>
#include <ghost.hpp>

// Pacman centered at origin, classic yellow, 30-degree mouth opening per side
Pacman pacman({0.0f, 0.0f, 0.0f}, 0.0625f, {255.0f, 255.0f, 0.0f}, 30.0f);
// Pellet pellet({0.25f, 0.25f, 0.25f});
// Ghost  ghost({-0.25f, -0.25f, -0.25f}, 0.0625f); // Opposite side from pellet; default blue

void init()
{
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glEnable(GL_DEPTH_TEST);
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    pacman.display();
    pellet.display();
    ghost.display();

    glFlush();
}

int main(int argc, char *argv[])
{
    pacman.debug();
    ghost.debug();

    glutInit(&argc, argv);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(250, 250);
    glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE | GLUT_DEPTH);
    glutCreateWindow("Pacman - Nkunga");
    glutDisplayFunc(display);
    init();
    glutMainLoop();
    return 0;
}