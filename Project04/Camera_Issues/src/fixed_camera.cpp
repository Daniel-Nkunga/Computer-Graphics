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
// Cube Class ===============================================================

class Cube
{
private:
    float midx, midy, midz, size;
    float rotation;
public:
    Cube(float _midx, float _midy, float _midz, float _size);
    ~Cube() {};

    void draw();

};

Cube::Cube(float _midx, float _midy, float _midz, float _size)
{
    midx = _midx;
    midy = _midy;
    midz = _midz;
    size = _size;
    rotation = 0.0f;
}

void Cube::draw()
{
    // Define 8 vertices
   float ax = midx - size / 2;
   float ay = midy - size / 2;
   float az = midz + size / 2;
   float bx = midx + size / 2;
   float by = midy - size / 2;
   float bz = midz + size / 2;
   float cx = midx + size / 2;
   float cy = midy + size / 2;
   float cz = midz + size / 2;
   float dx = midx - size / 2;
   float dy = midy + size / 2;
   float dz = midz + size / 2;
   float ex = midx - size / 2;
   float ey = midy - size / 2;
   float ez = midz - size / 2;
   float fx = midx + size / 2;
   float fy = midy - size / 2;
   float fz = midz - size / 2;
   float gx = midx + size / 2;
   float gy = midy + size / 2;
   float gz = midz - size / 2;
   float hx = midx - size / 2;
   float hy = midy + size / 2;
   float hz = midz - size / 2;

    // Draw 6 faces
    glBegin(GL_POLYGON);
    glColor3f(1.0, 0.0, 0.0);
    glVertex3f(ax, ay, az);
    glVertex3f(bx, by, bz);
    glVertex3f(cx, cy, cz);
    glVertex3f(dx, dy, dz);
    glEnd();

    glBegin(GL_POLYGON);
    glColor3f(0.0, 1.0, 0.0);
    glVertex3f(ax, ay, az);
    glVertex3f(dx, dy, dz);
    glVertex3f(hx, hy, hz);
    glVertex3f(ex, ey, ez);
    glEnd();

    glBegin(GL_POLYGON);
    glColor3f(0.0, 0.0, 1.0);
    glVertex3f(ax, ay, az);
    glVertex3f(ex, ey, ez);
    glVertex3f(fx, fy, fz);
    glVertex3f(bx, by, bz);
    glEnd();

    glBegin(GL_POLYGON);
    glColor3f(0.0, 1.0, 1.0);
    glVertex3f(gx, gy, gz);
    glVertex3f(fx, fy, fz);
    glVertex3f(ex, ey, ez);
    glVertex3f(hx, hy, hz);
    glEnd();

    glBegin(GL_POLYGON);
    glColor3f(1.0, 0.0, 1.0);
    glVertex3f(gx, gy, gz);
    glVertex3f(cx, cy, cz);
    glVertex3f(bx, by, bz);
    glVertex3f(fx, fy, fz);
    glEnd();

    glBegin(GL_POLYGON);
    glColor3f(1.0, 1.0, 0.0);
    glVertex3f(gx, gy, gz);
    glVertex3f(hx, hy, hz);
    glVertex3f(dx, dy, dz);
    glVertex3f(cx, cy, cz);
    glEnd();
}

// ===================================================================

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
std::vector<Cube> cubes = {cube1, cube2, cube3};

using namespace std;

// Functions
void init();
void display();
void update(int value);
void keyboard(unsigned char key, int x, int y);

int main(int argc, char *argv[]) {
    // Cube(0.1, 0.2, 0.3, 0.5).rotateZ(8.0f);
    cout << "Press 'x' or 'X' to rotate around the x-axis" << endl;

    glutInit(&argc, argv);
    glutInitWindowSize(1000, 1000);
    glutInitWindowPosition(250, 250);
    glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE | GLUT_DEPTH);
    glutCreateWindow("Fixed Camera - Nkunga");
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

    glEnable(GL_DEPTH_TEST); //TODO:
    glDepthFunc(GL_LESS);
}

// cout << "(" << 3*sin(theta) << ", " << 3*cos(theta) << ", " << 3*sin(theta_z) << ")" << endl;
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(3*cos(theta), 0.5f, 3*sin(theta),
              0, 0, 0,
              0, 1, 0);

    for(int i = 0; i < cubes.size(); i++)
    {
        cubes[i].draw();
    }
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