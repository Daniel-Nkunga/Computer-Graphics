#include <GL/freeglut_std.h>
#include <GL/gl.h>
#include <cstddef>
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

// Functions
void init();
void display();
void update(int value);
// Ghost logic
void clideUpdate(Ghost & ghost, int i);

#define TARGET_FPS    30
#define FRAME_DELAY   (1000 / TARGET_FPS)  // ~33 ms per frame

#define Blinky 0
#define Pinky 1
#define Inky 2
#define CLIDE 3

// Pacman centered at origin, classic yellow, 30-degree mouth opening per side
Pacman pacman({0.0f, 0.0f, 0.0f}, 0.0625f, {255.0f, 255.0f, 0.0f}, 30.0f);
vector<Ghost> ghosts;
vector<Pellet> pellets;

// Ghost logic details
float ghost_speed = 0.0075f;

// Per-ghost movement state (one entry per ghost)
float ghost_dx[4];
float ghost_dy[4];
int   ghost_frames_until_turn[4];

// Pick a random velocity component: -speed, 0, or +speed
// Weighted so 0 (stop on axis) is less likely than moving
float randomVelocity()
{
    int r = rand() % 5;
    if      (r == 0) return 0.0f;         // 20% chance: stopped on this axis
    else if (r <= 2) return  ghost_speed; // 40% chance: positive
    else             return -ghost_speed; // 40% chance: negative
}

// Pick a random interval between direction changes: 0.5-2.5 seconds
int randomInterval()
{
    return TARGET_FPS / 2 + rand() % (TARGET_FPS * 2); // 15-75 frames
}

int main(int argc, char *argv[])
{
    pacman.debug();

    srand(time(0));

    // Generate pellets
    for (int i = 0; i < 100; i++)
    {
        float x = (static_cast<float>(rand()) / static_cast<float>(RAND_MAX)) * 2.0f - 1.0f;
        float y = (static_cast<float>(rand()) / static_cast<float>(RAND_MAX)) * 2.0f - 1.0f;
        float z = (static_cast<float>(rand()) / static_cast<float>(RAND_MAX)) * 2.0f - 1.0f;
        pellets.push_back(Pellet({x, y, z}));
    }

    glutInit(&argc, argv);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(250, 250);
    glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE | GLUT_DEPTH);
    glutCreateWindow("Pacman - Nkunga");
    glutTimerFunc(FRAME_DELAY, update, 0);
    glutDisplayFunc(display);
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

    ghosts.push_back(Ghost({-0.25f, -0.25f, -0.25f}, 0.0625f, {255.0f, 0.0f, 0.0f}));    // Blinky
    ghosts.push_back(Ghost({-0.25f,  0.25f, -0.25f}, 0.0625f, {255.0f, 184.0f, 255.0f})); // Pinky
    ghosts.push_back(Ghost({ 0.25f,  0.25f, -0.25f}, 0.0625f, {0.0f, 255.0f, 255.0f}));   // Inky
    ghosts.push_back(Ghost({ 0.25f, -0.25f, -0.25f}, 0.0625f, {255.0f, 184.0f, 82.0f}));  // Clide

    // Kick off each ghost's first random direction independently
    for (int i = 0; i < 4; i++)
    {
        ghost_dx[i] = randomVelocity();
        ghost_dy[i] = randomVelocity();
        ghost_frames_until_turn[i] = randomInterval();
    }
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    pacman.display();
    for(size_t i = 0; i < pellets.size(); i++)
        pellets[i].display();
    for(size_t i = 0; i < ghosts.size(); i++)
        ghosts[i].display();

    glFlush();
}

void update(int value)
{
    for(size_t i = 0; i < pellets.size(); i++)
        pellets[i].update();

    clideUpdate(ghosts[Blinky], Blinky);
    clideUpdate(ghosts[Pinky],  Pinky);
    clideUpdate(ghosts[Inky],   Inky);
    clideUpdate(ghosts[CLIDE],  CLIDE);

    glutPostRedisplay();
    glutTimerFunc(FRAME_DELAY, update, value);
}

void clideUpdate(Ghost & ghost, int i)
{
    // Count down; pick a new direction when the timer expires
    ghost_frames_until_turn[i]--;
    if (ghost_frames_until_turn[i] <= 0)
    {
        ghost_dx[i] = randomVelocity() * 0.75f;
        ghost_dy[i] = randomVelocity() * 0.75f;
        ghost_frames_until_turn[i] = randomInterval();
    }

    // Apply velocity
    float x = std::get<0>(ghost.getPosition()) + ghost_dx[i];
    float y = std::get<1>(ghost.getPosition()) + ghost_dy[i];

    // Screenwrap
    if (x < -1.0f) x =  1.0f;
    if (x >  1.0f) x = -1.0f;
    if (y < -1.0f) y =  1.0f;
    if (y >  1.0f) y = -1.0f;

    ghost.setPosition({x, y, 0});
}