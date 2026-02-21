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
float randomVelocity();
int   randomInterval();
// Ghost logic
void clideUpdate(Ghost & ghost, int i);
// Pacman logic
void pacmanKeyDown(unsigned char key, int x, int y);
void pacmanKeyUp(unsigned char key, int x, int y);
void pacmanSpecialDown(int key, int x, int y);
void pacmanSpecialUp(int key, int x, int y);
// HUD
void drawHUD();
// Collision
bool checkCollision(tuple<float,float,float> posA, float rA,
                    tuple<float,float,float> posB, float rB);
void checkAllCollisions();

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

// Pacman key state: tracks which directions are currently held
bool key_up    = false;
bool key_down  = false;
bool key_left  = false;
bool key_right = false;

int pelletsEaten = 0;

#define PACMAN_SPEED  0.0085f
#define PACMAN_RADIUS 0.0625f
#define GHOST_RADIUS  0.0625f
#define PELLET_RADIUS 0.020832f

int main(int argc, char *argv[])
{
    pacman.debug();

    srand(time(0));

    // Generate pellets
    for (int i = 0; i < 100; i++)
    {
        float x = (static_cast<float>(rand()) / static_cast<float>(RAND_MAX)) * 2.0f - 1.0f;
        float y = (static_cast<float>(rand()) / static_cast<float>(RAND_MAX)) * 2.0f - 1.0f;
        pellets.push_back(Pellet({x, y, 0.0f}));
    }

    glutInit(&argc, argv);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(250, 250);
    glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE | GLUT_DEPTH);
    glutCreateWindow("Pacman - Nkunga");
    glutTimerFunc(FRAME_DELAY, update, 0);
    glutDisplayFunc(display);
    glutKeyboardFunc(pacmanKeyDown);
    glutKeyboardUpFunc(pacmanKeyUp);
    glutSpecialFunc(pacmanSpecialDown);
    glutSpecialUpFunc(pacmanSpecialUp);
    glutSetKeyRepeat(GLUT_KEY_REPEAT_OFF);
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

    drawHUD();

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

    // Move pacman based on currently held keys — supports diagonal movement
    float dx = 0.0f, dy = 0.0f;
    if (key_up)    dy += PACMAN_SPEED;
    if (key_down)  dy -= PACMAN_SPEED;
    if (key_left)  dx -= PACMAN_SPEED;
    if (key_right) dx += PACMAN_SPEED;

    if (dx != 0.0f || dy != 0.0f)
    {
        float x = std::get<0>(pacman.getPosition()) + dx;
        float y = std::get<1>(pacman.getPosition()) + dy;

        // Screenwrap
        if (x < -1.0f) x =  1.0f;
        if (x >  1.0f) x = -1.0f;
        if (y < -1.0f) y =  1.0f;
        if (y >  1.0f) y = -1.0f;

        pacman.setPosition({x, y, 0});
    }

    checkAllCollisions();

    if(pellets.size() <= 0)
    {
        cout << "YOU WIN! GREAT JOB!" << endl;
        exit(0);
    }

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

void drawHUD()
{
    // Switch to pixel-space so we can position text precisely on screen
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, 500, 0, 500);  // matches window dimensions
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glDisable(GL_DEPTH_TEST);  // always render on top

    // Lime green
    glColor3f(0.0f, 1.0f, 0.0f);

    string score = "Pellets: " + to_string(pelletsEaten);

    // Approximate right-align: GLUT_BITMAP_HELVETICA_18 is ~10px per char
    int textWidth = score.size() * 10;
    int x = 500 - textWidth - 10;  // 10px padding from right edge
    int y = 500 - 24;              // 24px padding from top edge

    glRasterPos2i(x, y);
    for (char c : score)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);

    glEnable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
}

// Returns true if two circles overlap.
// Collision occurs when distance between centers < sum of radii.
bool checkCollision(tuple<float,float,float> posA, float rA,
                    tuple<float,float,float> posB, float rB)
{
    float dx = std::get<0>(posA) - std::get<0>(posB);
    float dy = std::get<1>(posA) - std::get<1>(posB);
    float dz = std::get<2>(posA) - std::get<2>(posB);
    float distance = std::sqrt(dx*dx + dy*dy + dz*dz);
    return distance < (rA + rB);
}

void checkAllCollisions()
{
    const string ghostNames[] = {"Blinky", "Pinky", "Inky", "Clide"};

    // Pacman vs ghosts
    for (size_t i = 0; i < ghosts.size(); i++)
    {
        if (checkCollision(pacman.getPosition(), PACMAN_RADIUS,
                           ghosts[i].getPosition(), GHOST_RADIUS))
        {
            cout << "Pacman collided with ghost: " << ghostNames[i] << endl;
            exit(0);
        }
    }

    // Pacman vs pellets
    for (size_t i = 0; i < pellets.size(); i++)
    {
        if (checkCollision(pacman.getPosition(), PACMAN_RADIUS,
                        pellets[i].getPosition(), PELLET_RADIUS))
        {
            // cout << "Pacman collided with pellet #" << i << endl;
            pelletsEaten++;
            pellets.erase(pellets.begin() + i);
            i--; // step back so the shifted element isn't skipped
        }
    }

    // // Ghost vs ghost
    // for (size_t i = 0; i < ghosts.size(); i++)
    // {
    //     for (size_t j = i + 1; j < ghosts.size(); j++)
    //     {
    //         if (checkCollision(ghosts[i].getPosition(), GHOST_RADIUS,
    //                            ghosts[j].getPosition(), GHOST_RADIUS))
    //         {
    //             cout << "Ghost collision: " << ghostNames[i]
    //                  << " x " << ghostNames[j] << endl;
    //         }
    //     }
    // }

    // // Ghost vs pellets
    // for (size_t i = 0; i < ghosts.size(); i++)
    // {
    //     for (size_t j = 0; j < pellets.size(); j++)
    //     {
    //         if (checkCollision(ghosts[i].getPosition(), GHOST_RADIUS,
    //                            pellets[j].getPosition(), PELLET_RADIUS))
    //         {
    //             cout << ghostNames[i] << " collided with pellet #" << j << endl;
    //         }
    //     }
    // }
}

// WASD key down
void pacmanKeyDown(unsigned char key, int x, int y)
{
    if (key == 'w' || key == 'W') key_up    = true;
    if (key == 's' || key == 'S') key_down  = true;
    if (key == 'a' || key == 'A') key_left  = true;
    if (key == 'd' || key == 'D') key_right = true;
}

// WASD key up
void pacmanKeyUp(unsigned char key, int x, int y)
{
    if (key == 'w' || key == 'W') key_up    = false;
    if (key == 's' || key == 'S') key_down  = false;
    if (key == 'a' || key == 'A') key_left  = false;
    if (key == 'd' || key == 'D') key_right = false;
}

// Arrow key down
void pacmanSpecialDown(int key, int x, int y)
{
    if (key == GLUT_KEY_UP)    key_up    = true;
    if (key == GLUT_KEY_DOWN)  key_down  = true;
    if (key == GLUT_KEY_LEFT)  key_left  = true;
    if (key == GLUT_KEY_RIGHT) key_right = true;
}

// Arrow key up
void pacmanSpecialUp(int key, int x, int y)
{
    if (key == GLUT_KEY_UP)    key_up    = false;
    if (key == GLUT_KEY_DOWN)  key_down  = false;
    if (key == GLUT_KEY_LEFT)  key_left  = false;
    if (key == GLUT_KEY_RIGHT) key_right = false;
}

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