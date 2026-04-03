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
float rotatingCubeAngle = 0.0;

#define TARGET_FPS 60
#define FRAME_DELAY (1000 / TARGET_FPS)

using namespace std;

// Functions
void init();
void display();
void update(int value);
void keyboard(unsigned char key, int x, int y);
void initializeScene();
void special(int key, int x, int y);
void mouse(int button, int state, int x, int y);
// Cube createCube();

int main(int argc, char *argv[]) {
    cout << "Press \033[1;34m'x'\033[0m or \033[1;34m'left arrow'\033[0m to pan the camera to the left" << endl;
    cout << "Press \033[1;34m'X'\033[0m or \033[1;34m'right arrow'\033[0m to pan the camera to the right" << endl;
    cout << "Press \033[1;34m'y'\033[0m or \033[1;34m'down arrow'\033[0m to pan the camera to the down" << endl;
    cout << "Press \033[1;34m'Y'\033[0m or \033[1;34m'up arrow'\033[0m to pan the camera to the up" << endl;
    cout << "\033[1;34mClick the mouse\033[0m to add more dice to the scene" << endl;
    cout << "Press \033[1;34m'e'\033[0m or \033[1;34m'E'\033[0m to remove dice form the scene" << endl;
    initializeScene();

    glutInit(&argc, argv);
    glutInitWindowSize(1000, 1000);
    glutInitWindowPosition(250, 250);
    glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE | GLUT_DEPTH);
    glutCreateWindow("Dice Scene - Nkunga");
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(special);
    glutMouseFunc(mouse);
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
   switch(key)
    {
        case 'x':
            theta -= step;
            break;
        case 'X':
            theta += step;
            break;
        case 'y':
            theta_y -= step;
            break;
        case 'Y':
            theta_y += step;
            break;
        case 'e':
            if(cubes.size() > 500)
            {
                for(int i = 0; i < 500; i++)
                {
                    cubes.pop_back();
                }
                cout << cubes.size() << " dice exist in this scene" << endl;
            }
            break;
        case 'E':
            if(cubes.size() > 500)
            {
                for(int i = 0; i < 500; i++)
                {
                    cubes.pop_back();
                }
                cout << cubes.size() << " dice exist in this scene" << endl;
            }
            break;
    }

   // Redraw objects
   glutPostRedisplay();
}

void mouse(int button, int state, int x, int y)
{
    // Handle mouse down
   if (state == GLUT_DOWN)
   {
      for(int i = 0; i < 500; i++)
    {
        cubes.push_back(Cube::createRandomCube());
    }
   }
   cout << cubes.size() << " dice exist in this scene" << endl;
   glutPostRedisplay();
}

void special(int key, int x, int y)
{
   switch(key)
    {
        case GLUT_KEY_LEFT:
            theta += step;
            break;
        case GLUT_KEY_RIGHT:
            theta -= step;
            break;
        case GLUT_KEY_UP:
            theta_y += step;
            break;
        case GLUT_KEY_DOWN:
            theta_y -= step;
            break;
    }

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
    for(int i = 0; i < 1000; i++)
    {
        cubes.push_back(Cube::createRandomCube());
    }

    // Stack of three dice
    set_dressing.push_back(StationaryCube(-1.25, 0.0625, -1.25, 0.125));
    set_dressing.push_back(StationaryCube(-1.25, 0.0625, -1.05, 0.125, 180.0, 30.0, 180.0));
    set_dressing.push_back(StationaryCube(-1.25, 0.18, -1.15, 0.125, 5.0, 60.0, 0.0));

    // Tower of dice
    set_dressing.push_back(StationaryCube(-0.8, 0.0635, 1.25, 0.125, 0.0, 45.0, 180.0));
    set_dressing.push_back(StationaryCube(-0.78, 0.18, 1.25, 0.125, 180.0, 255.0, 0.0));
    set_dressing.push_back(StationaryCube(-0.82, 0.30, 1.25, 0.125, 180.0, 125.0, 180.0));
    set_dressing.push_back(StationaryCube(-0.8, 0.42, 1.25, 0.125, 0.0, -225.0, 0.0));
    set_dressing.push_back(StationaryCube(-0.8, 0.59, 1.25, 0.125, 45.0, 0.0, 45.0)); // Diagonal [7]
    set_dressing.push_back(StationaryCube(-0.8, 0.76, 1.25, 0.125, 180.0, 175.0, 0.0));

    // Pyramid
    set_dressing.push_back(StationaryCube(1.25, 0.0635, 1.05, 0.125, 0.0, -30.0, 0.0));
    set_dressing.push_back(StationaryCube(1.05, 0.0635, 1.105, 0.125, 180.0, 45.0, 0.0));
    set_dressing.push_back(StationaryCube(1.1, 0.0635, 0.95, 0.125, 0.0, 65.0, 180.0));
    set_dressing.push_back(StationaryCube(1.15, 0.18, 1.05, 0.125, 0.0, 165.0, 90.0));

    // Single dice
    set_dressing.push_back(StationaryCube(1.35, 0.0635, -1.05, 0.125, 270, 0.0, 0.0));
}