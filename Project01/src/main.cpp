#include <GL/freeglut_std.h>
#include <GL/gl.h>
#include <iostream>
#include <tuple>
#include <vector>
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

std::vector<Polygon> polygons;

void init()
{
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glEnable(GL_DEPTH_TEST);
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for(size_t i = 0; i < polygons.size(); i++)
    {
        polygons[i].display();
    }

    glFlush();
}

int main(int argc, char *argv[])
{
    // srand(time(0));
    srand(88);

    for(int i = 0; i < 50; i++)
    {
        std::vector<std::tuple<float, float, float>> coordinates;
        for(int j = 0; j < (rand() % 4) + 3; j++)
        {
            float x, y, z;
            if (coordinates.empty())
            {
                x = (static_cast<float>(rand()) / static_cast<float>(RAND_MAX)) * 2.0f - 1.0f;
                y = (static_cast<float>(rand()) / static_cast<float>(RAND_MAX)) * 2.0f - 1.0f;
                z = (static_cast<float>(rand()) / static_cast<float>(RAND_MAX)) * 2.0f - 1.0f;
            } else {
                x = std::get<0>(coordinates[j-1]) + ((static_cast<float>(rand()) / static_cast<float>(RAND_MAX)) * 0.2f - 0.1f);
                y = std::get<1>(coordinates[j-1]) + ((static_cast<float>(rand()) / static_cast<float>(RAND_MAX)) * 0.2f - 0.1f);
                z = std::get<2>(coordinates[j-1]) + ((static_cast<float>(rand()) / static_cast<float>(RAND_MAX)) * 0.2f - 0.1f);
            }
            coordinates.push_back({x, y, z});
        }
        int color1 = rand() % 255;
        int color2 = rand() % 255;
        int color3 = rand() % 255;
        polygons.push_back(Polygon(coordinates, {color1, color2, color3}));
        polygons[i].debug();
    }

    // Display function
    glutInit(&argc, argv);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(250, 250);
    glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE |GLUT_DEPTH);
    glutCreateWindow("Polygons - Nkunga");
    glutDisplayFunc(display);
    init();
    glutMainLoop();
    return 0;
}

//TODO:
// GL_ENABLE DEPTH BUFFER, DEPTH BUFFER CLEAR