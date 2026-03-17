// Basic imports
#include <iostream>
#include <vector>
#include <tuple>
#include <cmath>
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
#include <perlin.hpp>
#include <vector_math.hpp>

using namespace std;

// Functions
void init();
void display();
void update(int value);
void keyboard(unsigned char key, int x, int y);
void generateTerrain(uint32_t seed);
tuple<float, float, float> generateColor(float height);


// Globals needed by display()
const int SIZE = 256;
vector<vector<tuple<float, float, float>>> coordinates;
vector<vector<tuple<float, float, float>>> normals;
float theta = 0.0f;
float rotation_amount = 0.15f;
bool phongLightingEnabled = true;
float max_height;

// Generation variables
double SCALE       = 60.0;
int    OCTAVES     = 30;
const double PERSISTENCE = 0.5;
const double LACUNARITY  = 2.0;

// Light direction (unit vector pointing FROM the surface TOWARD the light)
// Positioned above-front-right
const float Lx =  0.575f;   // 1/sqrt(3)
const float Ly = -0.575f;
const float Lz =  0.575f;

// ---------------------------------------------------------------------------

int main(int argc, char *argv[]) {
    cout << "Press 'r' or 'R' to regenerate terrain" << endl;
    cout << "Press \"space\" to pause rotation" << endl;
    cout << "Press 's' to decrease scale" << endl;
    cout << "Press 'S' to increase scale" << endl;
    cout << "Press 'o' to decrease octaves" << endl;
    cout << "Press 'O' to increase octaves" << endl;
    cout << "Press 'l' or 'L' to turn off Phong lighting" << endl;

    generateTerrain((uint32_t)rand());

    glutInit(&argc, argv);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(250, 250);
    glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE | GLUT_DEPTH);
    glutCreateWindow("Terrain - Nkunga");
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutSetKeyRepeat(GLUT_KEY_REPEAT_OFF);
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
    glEnable(GL_DEPTH_TEST);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(0.0, -1.5, 1.0,
              0.0,  0.0, 0.0,
              0.0,  0.0, 1.0);

    glRotatef(theta, 0, 0, 1);

    for (int y = 0; y < SIZE - 1; y++)
    {
        glBegin(GL_TRIANGLE_STRIP);

        for (int x = 0; x < SIZE; x++)
        {
            // Upper vertex (y+1)
            auto [x1, y1, z1] = coordinates[y + 1][x];
            auto [nx1, ny1, nz1] = normals[y + 1][x];

            // Diffuse term: N . L  (clamped so back-faces stay dark)
            float diff1  = max(0.0f, dot(nx1, ny1, nz1, Lx, Ly, Lz));
            float light1 = 0.15f + 0.85f * diff1;  // 0.15 ambient floor

            auto [r1, g1, b1] = generateColor(z1);
            // Turn off diffusion
            if (phongLightingEnabled)
            {
                glColor3f(r1 * light1, g1 * light1, b1 * light1);
            } else {
                glColor3f(r1, g1 , b1 );
            }
            glVertex3f(x1, y1, z1);

            // Lower vertex (y)
            auto [x0, y0, z0] = coordinates[y][x];
            auto [nx0, ny0, nz0] = normals[y][x];

            float diff0  = max(0.0f, dot(nx0, ny0, nz0, Lx, Ly, Lz));
            float light0 = 0.15f + 0.85f * diff0;

            auto [r0, g0, b0] = generateColor(z0);
            glColor3f(r0 * light0, g0 * light0, b0 * light0);
            glVertex3f(x0, y0, z0);
        }

        glEnd();
    }

    theta += rotation_amount;
    if(theta >= 360.0f) theta -= 360.0f;

    glFlush();
}

void generateTerrain(uint32_t seed)
{
    PerlinNoise pn(seed);
    vector<vector<double>> perlin_grid = pn.generate(SIZE, SCALE, OCTAVES, PERSISTENCE, LACUNARITY);

    // Build coordinate grid
    coordinates.assign(SIZE, vector<tuple<float,float,float>>(SIZE, {0.f,0.f,0.f}));

    for (int y = 0; y < SIZE; y++)
        for (int x = 0; x < SIZE; x++)
        {
            float nx = (float)x / (SIZE - 1) * 2.0f - 1.0f;
            float ny = (float)y / (SIZE - 1) * 2.0f - 1.0f;
            float nz = (float)perlin_grid[y][x];
            coordinates[y][x] = { nx, ny, nz };
        }

    // Compute per-vertex surface normals
    normals.assign(SIZE, vector<tuple<float,float,float>>(SIZE, {0.f,0.f,1.f}));

    for (int y = 0; y < SIZE; y++)
    {
        for (int x = 0; x < SIZE; x++)
        {
            // X-axis tangent (central difference, clamped)
            int xL = max(x - 1, 0);
            int xR = min(x + 1, SIZE - 1);
            auto [xlLx, xlLy, zlL] = coordinates[y][xL];
            auto [xlRx, xlRy, zlR] = coordinates[y][xR];
            float t1x = xlRx - xlLx;
            float t1y = xlRy - xlLy;
            float t1z = zlR  - zlL;

            // Y-axis tangent (central difference, clamped)
            int yD = max(y - 1, 0);
            int yU = min(y + 1, SIZE - 1);
            auto [xuDx, xuDy, zuD] = coordinates[yD][x];
            auto [xuUx, xuUy, zuU] = coordinates[yU][x];
            float t2x = xuUx - xuDx;
            float t2y = xuUy - xuDy;
            float t2z = zuU  - zuD;

            // N = T1 x T2, then normalise
            auto [cx, cy, cz] = cross(t1x, t1y, t1z,
                                      t2x, t2y, t2z);
            normals[y][x] = normalise(cx, cy, cz);
        }
    }

    for (const auto& row : coordinates)
        for (const auto& coord : row)
        {
            max_height = max(max_height, get<2>(coord));
        }
}

void keyboard(unsigned char key, int x, int y)
{
    uint32_t seed = (uint32_t)time(0);
    if (key == 'r' || key == 'R')
    {
        cout << "Regenerating terrain with seed: " << seed << endl;
        generateTerrain(seed);
    }
    if (key == 's' && SCALE >= 1)  { SCALE -= 1; generateTerrain(seed); }
    if (key == 'S' && SCALE <= 200){ SCALE += 1; generateTerrain(seed); }
    if (key == 'o' && OCTAVES >= 1) { OCTAVES -= 1; generateTerrain(seed); }
    if (key == 'O' && OCTAVES <= 100){ OCTAVES += 1; generateTerrain(seed); }
    if (key == ' ')
    {
        if (rotation_amount != 0) rotation_amount = 0;
        else rotation_amount = 0.15;
    }
    if (key == 'l' || key == 'L'){ phongLightingEnabled = !phongLightingEnabled; }

    glutPostRedisplay();
}

void update(int value) {
    glutPostRedisplay();
    glutTimerFunc(16, update, 0);
}

tuple<float, float, float> generateColor(float height)
{
    if      (height <= ((max_height / 11) * 6)) return { 0.40f, 0.25f, 0.10f }; // deep brown  (lowland); ~0.300
    else if (height <= ((max_height / 11) * 6.5)) return { 0.20f, 0.55f, 0.15f }; // low  green; ~0.325
    else if (height <= ((max_height / 11) * 7.5)) return { 0.15f, 0.65f, 0.15f }; // mid  green; ~0.375
    else if (height <= ((max_height / 11) * 8.5)) return { 0.50f, 0.45f, 0.30f }; // rocky tan; ~0.425
    else if (height <= ((max_height / 11) * 9.5)) return { 0.55f, 0.50f, 0.45f }; // grey rock; ~0.475
    else                       return { 0.95f, 0.95f, 0.95f }; // snow white  (peaks)
}