// Basic inports
#include <iostream>
#include <vector>
#include <tuple>
#include <cmath>

// OpenGL Fuctions
#include <GL/freeglut_std.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <cstdlib>
#include <vector>
#ifdef MAC
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

// Custom classes
#include <perlin.hpp>

using namespace std;

// Funcitons
void init();
void display();

int main(int argc, char *argv[]) {
    srand(88);

    // Perline noise generation parameters
    const int    SIZE        = 64;
    const double SCALE       = 20.0;
    const int    OCTAVES     = 5;
    const double PERSISTENCE = 0.5;
    const double LACUNARITY  = 2.0;
    const uint32_t SEED      = rand();

    PerlinNoise pn(SEED);
    vector<vector<double>> perline_grid = pn.generate(SIZE, SCALE, OCTAVES, PERSISTENCE, LACUNARITY);

    // Grid of coordinates
    vector<vector<tuple<float, float, float>>> coordinates( SIZE, vector<tuple<float, float, float>>(SIZE, {0.f, 0.f, 0.f}));

    for (int x = 0; y < SIZE; y++)
    {
        for (int y = 0; x < SIZE; x++)
        {
            coordinates[x][y] = { (float)x, (float)y, (float)perline_grid[y][x] };
            // cout << coordinates[x][y] << endl;
        }
    }

    glutInit(&argc, argv);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(250, 250);
    glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE | GLUT_DEPTH);
    glutCreateWindow("Terrain - Nkunga");
    glutDisplayFunc(display);
    glutSetKeyRepeat(GLUT_KEY_REPEAT_OFF);
    init();
    glutMainLoop();
    return 0;
}

void init() {
  glClearColor(0.0, 0.0, 0.0, 1.0);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
  glMatrixMode(GL_MODELVIEW);
  glEnable(GL_DEPTH_TEST);
}

void display() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glFlush();
}

void update(int value) {
}