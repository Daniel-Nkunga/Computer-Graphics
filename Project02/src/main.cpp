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

// Funcs
void init();
void display();
void update(int value);

#define TARGET_FPS    30
#define FRAME_DELAY   (1000 / TARGET_FPS)  // ~33 ms per frame

// Pacman centered at origin, classic yellow, 30-degree mouth opening per side
Pacman pacman({0.0f, 0.0f, 0.0f}, 0.0625f, {255.0f, 255.0f, 0.0f}, 30.0f);
Pellet pellet({0.25f, 0.25f, 0.25f});
Ghost  ghost({-0.25f, -0.25f, -0.25f}, 0.0625f, {100.0f, 80.0f, 60.0f}); // Opposite side from pellet; default blue
vector<Pellet> pellets;

int main(int argc, char *argv[])
{
    pacman.debug();
    ghost.debug();

    srand(time(0));
    // srand(88);

    // Generate pellets
    for (int i = 0; i < 100; i++)
    {
        float x = (static_cast<float>(rand()) / static_cast<float>(RAND_MAX)) * 2.0f - 1.0f;
        float y = (static_cast<float>(rand()) / static_cast<float>(RAND_MAX)) * 2.0f - 1.0f;
        float z = (static_cast<float>(rand()) / static_cast<float>(RAND_MAX)) * 2.0f - 1.0f;
        pellets.push_back(Pellet({x, y, z}));
        // pellets.back().debug();
    }

    glutInit(&argc, argv);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(250, 250);
    glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE | GLUT_DEPTH);
    glutCreateWindow("Pacman - Nkunga");
    glutDisplayFunc(display);
    glutTimerFunc(FRAME_DELAY, update, 0); // Kick off the timer loop
    init();
    glutMainLoop();
    return 0;
}

void init()
{
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glEnable(GL_DEPTH_TEST);

    // for(size_t i = 0; i < pellets.size(); i++)
    // {
    //     pellets[i].debug();
    // }
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    pacman.display();
    // pellet.display();
    for(size_t i = 0; i < pellets.size(); i++)
    {
        pellets[i].display();
    }
    ghost.display();

    glFlush();
}

void update(int value)
{
    // pellet.update();
    for(size_t i = 0; i < pellets.size(); i++)
    {
        pellets[i].update();
    }

    glutPostRedisplay();                       // Request a redraw
    glutTimerFunc(FRAME_DELAY, update, value); // Re-register for next frame
}