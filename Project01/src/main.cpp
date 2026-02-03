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
float theta = 0.0f;

/* Init Function



*/
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

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glRotatef(theta, 1, 1, 1);

    for(size_t i = 0; i < polygons.size(); i++)
    {
        polygons[i].display();
    }

    theta += 0.5f;
    if(theta >= 360.0f) theta -= 360.0f;

    glFlush();
}

int main(int argc, char *argv[])
{
    srand(time(0));
    // srand(88);

    // Generate polygons; capped at 150 for visual clutter
    for(int i = 0; i < 150; i++)
    {
        // Geenrate a random number of points between [3, 7]
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
                // Forces the generation of the next point to be somewhat next to the previous point
                x = std::get<0>(coordinates[j-1]) + ((static_cast<float>(rand()) / static_cast<float>(RAND_MAX)) * 0.2f - 0.1f);
                y = std::get<1>(coordinates[j-1]) + ((static_cast<float>(rand()) / static_cast<float>(RAND_MAX)) * 0.2f - 0.1f);
                z = std::get<2>(coordinates[j-1]) + ((static_cast<float>(rand()) / static_cast<float>(RAND_MAX)) * 0.2f - 0.1f);
            }
            coordinates.push_back({x, y, z});
        }
        float color1 = rand() % 255;
        float color2 = rand() % 255;
        float color3 = rand() % 255;
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