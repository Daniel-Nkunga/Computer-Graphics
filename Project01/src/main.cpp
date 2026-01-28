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
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);

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

    for(int i = 0; i < 25; i++) // TODO: change to random number to generate
    {
        std::vector<std::tuple<float, float, float>> coordinates;
        for(int j = 0; j < (rand() % 4) + 3; j++)
        {
            float x = (static_cast<float>(rand()) / static_cast<float>(RAND_MAX)) * 2.0f - 1.0f;
            float y = (static_cast<float>(rand()) / static_cast<float>(RAND_MAX)) * 2.0f - 1.0f;
            float z = (static_cast<float>(rand()) / static_cast<float>(RAND_MAX)) * 2.0f - 1.0f;
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
    glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
    glutCreateWindow("Polygons - Nkunga");
    // polygons[i].display();
    glutDisplayFunc(display);
    init();
    glutMainLoop();
    return 0;
}