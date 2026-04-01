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

// Display variables
float xpos = 2.1;
float ypos = 2.1;
float zpos = 1.0f;
float step = 0.1;
float theta = 0.0f;
float theta_z = 0.0f;

using namespace std;

// Functions
void init();
void display();
void update(int value);
void keyboard(unsigned char key, int x, int y);

int main(int argc, char *argv[]) {
    glutInit(&argc, argv);
    glutInitWindowSize(1000, 1000);
    glutInitWindowPosition(250, 250);
    glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE | GLUT_DEPTH);
    glutCreateWindow("Dice Scene - Nkunga");
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    init();
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
}

void display() {
    cout << "(" << 3*sin(theta) << ", " << 3*cos(theta) << ", " << 3*sin(theta_z) << ")" << endl;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(3*cos(theta),3*sin(theta),3*sin(theta_z),
         0,0,0,
          0,1,0);
    Cube(0.0, 0.0, 0.0, 0.5).draw();
    glFlush();
}

void keyboard(unsigned char key, int x, int y)
{
   // Quit
   if (key == 'q')
      exit(0);

   // Update camera position
   else if (key == 'x')
    {
        // xpos -= step;
        theta -= step;
    }
   else if (key == 'y')
      ypos -= step;
   else if (key == 'z')
        // zpos -= step;
        theta_z -= step;
   else if (key == 'X')
    {
        // xpos += step;
        theta += step;
    }
   else if (key == 'Y')
      ypos += step;
   else if (key == 'Z')
        // zpos += step;
        theta_z += step;

   // Redraw objects
   glutPostRedisplay();
}

void update(int value) {
    glutPostRedisplay();
    glutTimerFunc(16, update, 0);
}