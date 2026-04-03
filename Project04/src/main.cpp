// Basic imports
#include <iostream>
#include <vector>
#include <tuple>
#include <cstdlib>
#include <ctime>
#include <typeinfo>
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
#include <table.hpp>
// #include <matrix.hpp>

// Display variables
float xpos = 2.1;
float ypos = 2.1;
float zpos = 1.0f;
float step = 0.1;
float theta = 0.0f;
float theta_y = 15.0f;
float angle = 0.0f;
vector<Cube> cubes;
Table table(0, 0, 0, 2.0);
vector<StationaryCube> set_dressing;

#define TARGET_FPS 60
#define FRAME_DELAY (1000 / TARGET_FPS)

using namespace std;

// Functions
void init();
void display();
void update(int value);
void keyboard(unsigned char key, int x, int y);
void initializeScene();
// Cube createCube();

int main(int argc, char *argv[]) {
    initializeScene();

    glutInit(&argc, argv);
    glutInitWindowSize(1000, 1000);
    glutInitWindowPosition(250, 250);
    glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE | GLUT_DEPTH);
    glutCreateWindow("Dice Scene - Nkunga");
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    init();
    glutTimerFunc(16, update, 0);
    glutMainLoop();

    return 0;
}

void init() {
    glClearColor(0.0, 0.0, 0.0, 1.0);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, 1.0, 0.1, 100.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // Texturing
    init_texture((char *)"img/Dice_Sprite.jpg", texture, xdim, ydim);
    glEnable(GL_TEXTURE_2D);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(3*cos(theta), sin(theta_y), 3*sin(theta),
              0, 0, 0,
              0, 1, 0);

    for(size_t i = 0; i < cubes.size(); i++)
    {
        cubes[i].draw();
    }
    table.draw();
    for(size_t i= 0; i < set_dressing.size(); i++)
    {
        set_dressing[i].draw();
    }
    glFlush();
}

void keyboard(unsigned char key, int x, int y)
{
   // Update camera position
   if (key == 'x')
    {
        // xpos -= step;
        theta -= step;
    }
   else if (key == 'y')
        // ypos -= step;
        theta_y -= step;
   else if (key == 'z')
        zpos -= step;
   else if (key == 'X')
    {
        // xpos += step;
        theta += step;
    }
   else if (key == 'Y')
        // ypos += step;
        theta_y += step;
   else if (key == 'Z')
        zpos += step;

   // Redraw objects
   glutPostRedisplay();
}

void update(int value) {
    for(size_t i = 0; i < cubes.size(); i++)
    {
        cubes[i].update();
    }

    glutPostRedisplay();
    glutTimerFunc(16, update, 0);
}

void initializeScene()
{
    for(int i = 0; i < 2000; i++)
    {
        cubes.push_back(Cube::createRandomCube());
    }

    // Stack of three dice
    set_dressing.push_back(StationaryCube(-1.25, 0.0625, -1.25, 0.125));
    set_dressing.push_back(StationaryCube(-1.25, 0.0625, -1.05, 0.125, 0.0, 30.0, 0.0));
    set_dressing.push_back(StationaryCube(-1.25, 0.18, -1.15, 0.125, 5.0, 60.0, 0.0));

    // Tower of dice
    set_dressing.push_back(StationaryCube(-0.8, 0.0635, 1.25, 0.125, 0.0, 45.0, 180.0));
    set_dressing.push_back(StationaryCube(-0.78, 0.18, 1.25, 0.125, 0.0, 255.0, 0.0));
    set_dressing.push_back(StationaryCube(-0.82, 0.30, 1.25, 0.125, 180.0, 125.0, 0.0));
    set_dressing.push_back(StationaryCube(-0.8, 0.42, 1.25, 0.125, 0.0, -125.0, 0.0));
    set_dressing.push_back(StationaryCube(-0.8, 0.59, 1.25, 0.125, 45.0, 0.0, 45.0)); // Diagonal
    set_dressing.push_back(StationaryCube(-0.8, 0.76, 1.25, 0.125, 180.0, -275.0, 0.0));

    // Pyramid
    set_dressing.push_back(StationaryCube(1.25, 0.0635, 1.05, 0.125, 0.0, -30.0, 0.0));
    set_dressing.push_back(StationaryCube(1.05, 0.0635, 1.105, 0.125, 180.0, 45.0, 0.0));
    set_dressing.push_back(StationaryCube(1.1, 0.0635, 0.95, 0.125, 0.0, 65.0, 180.0));
    set_dressing.push_back(StationaryCube(1.15, 0.18, 1.05, 0.125, 0.0, 165.0, 90.0));
}