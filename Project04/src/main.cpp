// Basic imports
#include <iostream>
#include <vector>
#include <tuple>
#include <cstdlib>
#include <ctime>
#include <typeinfo>

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
float xpos = 0.1;
float ypos = 0.1;
float zpos = 1.1;
float step = 0.01;

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
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(xpos,ypos,zpos, 0,0,0, 0,1,0);
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
      xpos -= step;
   else if (key == 'y')
      ypos -= step;
   else if (key == 'z')
      zpos -= step;

   else if (key == 'X')
      xpos += step;
   else if (key == 'Y')
      ypos += step;
   else if (key == 'Z')
      zpos += step;

   // Redraw objects
   glutPostRedisplay();
}

void update(int value) {
    glutPostRedisplay();
    glutTimerFunc(16, update, 0);
}