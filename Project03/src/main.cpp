// Basic imports
#include <iostream>
#include <vector>
#include <tuple>
#include <cmath>
#include <cstdlib>
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
#include <perlin.hpp>

using namespace std;

// Functions
void init();
void display();

// Globals needed by display()
const int SIZE = 64;
vector<vector<tuple<float, float, float>>> coordinates;

int main(int argc, char *argv[]) {
    srand(88);

    // Perlin noise generation parameters
    const double SCALE       = 20.0;
    const int    OCTAVES     = 5;
    const double PERSISTENCE = 0.5;
    const double LACUNARITY  = 2.0;
    const uint32_t SEED      = rand();

    PerlinNoise pn(SEED);
    vector<vector<double>> perlin_grid = pn.generate(SIZE, SCALE, OCTAVES, PERSISTENCE, LACUNARITY);

    // Grid of coordinates — map grid indices to [-1, 1] in XY, noise drives Z
    coordinates.assign(SIZE, vector<tuple<float, float, float>>(SIZE, {0.f, 0.f, 0.f}));

    for (int y = 0; y < SIZE; y++)
    {
        for (int x = 0; x < SIZE; x++)
        {
            float nx = (float)x / (SIZE - 1) * 2.0f - 1.0f; // [-1,  1]
            float ny = (float)y / (SIZE - 1) * 2.0f - 1.0f; // [-1,  1]
            float nz = (float)perlin_grid[y][x] * 0.5f;      // [ 0, 0.5]
            coordinates[y][x] = { nx, ny, nz };
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
    // Perspective projection so the terrain looks 3-D
    gluPerspective(60.0, 1.0, 0.1, 100.0);
    glMatrixMode(GL_MODELVIEW);
    glEnable(GL_DEPTH_TEST);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Pull the camera back and tilt down so we see the terrain
    gluLookAt(0.0, -2.5, 2.0,   // eye
              0.0,  0.0, 0.0,   // centre
              0.0,  0.0, 1.0);  // up

    // Draw one GL_TRIANGLE_STRIP per row.
    // Each strip covers the quad band between row y and row y+1:
    //
    //   (x, y+1)----(x+1, y+1)
    //      |      \      |
    //   (x,  y)----(x+1,  y)
    //
    // Alternating vertices from the two rows automatically tiles triangles.

    for (int y = 0; y < SIZE - 1; y++)
    {
        glBegin(GL_TRIANGLE_STRIP);

        for (int x = 0; x < SIZE; x++)
        {
            // Vertex on the upper row (y+1)
            tuple<float, float, float> vertex1 = coordinates[y + 1][x];
            float x1 = get<0>(vertex1);
            float y1 = get<1>(vertex1);
            float z1 = get<2>(vertex1);
            glColor3f(z1 * 2.0f, 0.4f + z1, 0.2f);
            glVertex3f(x1, y1, z1);

            // Vertex on the lower row (y)
            tuple<float, float, float> vertex0 = coordinates[y][x];
            float x0 = get<0>(vertex0);
            float y0 = get<1>(vertex0);
            float z0 = get<2>(vertex0);
            glColor3f(z1 * 2.0f, 0.4f + z1, 0.2f);
            glVertex3f(x0, y0, z0);
        }

        glEnd();
    }

    glFlush();
}

void update(int value) {
}