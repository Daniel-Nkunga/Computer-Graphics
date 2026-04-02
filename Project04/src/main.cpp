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
// #include <matrix.hpp>

// Display variables
float xpos = 2.1;
float ypos = 2.1;
float zpos = 1.0f;
float step = 0.1;
float theta = 0.0f;
float theta_z = 0.0f;
float angle = 0.0f;
Cube cube1(0.0, 0.0, 0.0, 0.5);
Cube cube2(1.0, 0.0, 1.0, 0.5);
Cube cube3(-1.0, 0.0, -1.0, 0.5);
vector<Cube> cubes = {cube1, cube2, cube3};
int SPAWN_RADIUS = 15;

#define TARGET_FPS 60
#define FRAME_DELAY (1000 / TARGET_FPS)

using namespace std;

// Functions
void init();
void display();
void update(int value);
void keyboard(unsigned char key, int x, int y);
void initializeScene();
Cube createCube();

int main(int argc, char *argv[]) {
    initializeScene();
    cout << cubes.size() << endl;

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
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(3*cos(theta), 0.5f, 3*sin(theta),
              0, 0, 0,
              0, 1, 0);

    for(size_t i = 0; i < cubes.size(); i++)
    {
        cubes[i].draw();
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
        theta_z -= step;
   else if (key == 'z')
        zpos -= step;
   else if (key == 'X')
    {
        // xpos += step;
        theta += step;
    }
   else if (key == 'Y')
        // ypos += step;
        theta_z += step;
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
    for(int i = 0; i < 500; i++)
    {
        cubes.push_back(createCube());
    }
}

Cube createCube()
{
    float initX, initY, initZ, size;
    initX = (-1*SPAWN_RADIUS) + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/((SPAWN_RADIUS*3))));
    initY = static_cast <float> (rand()) / static_cast <float> (RAND_MAX) + 1.25;
    initZ = (-1*SPAWN_RADIUS) + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/((SPAWN_RADIUS*3))));
    size = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/0.5));

    cout << "(" << initX << ", " << initY << ", " << initZ << ", " << size << ")" << endl;
    return Cube(initX, initY, initZ, size);
}