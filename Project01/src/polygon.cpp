#include <GL/gl.h>
#include <polygon.hpp>
#include <iostream>
#include <tuple>
#include <vector>

Polygon::Polygon(std::vector<std::tuple<float, float, float>> coordinates, std::tuple<int, int, int> color)
{
    this->coordinates = coordinates;
    this->color = color;
}

// Setting functions
void Polygon::set(std::vector<std::tuple<float, float, float>> coordinates, std::tuple<int, int, int> color)
{
    this->coordinates = coordinates;
    this->color = color;
}

void Polygon::set(std::vector<std::tuple<float, float, float>> coordinates) // Set coordinates
{
    this->coordinates = coordinates;
}

void Polygon::set(std::tuple<int, int, int> color) // Set color
{
    this->color = color;
}

void Polygon::debug()
{
    // Display coordinates
    std::cout << "\033[1;4;33mCoordinates:\033[0m ";
    for (size_t i = 0; i < this->coordinates.size(); i++)
    {
        auto& coord = this->coordinates[i];
        std::cout << "(" << std::get<0>(coord) << ", "
                  << std::get<1>(coord) << ", "
                  << std::get<2>(coord) << ")";

        if (i < this->coordinates.size() - 1)
            std::cout << ",\033[0m ";
    }

    // Display color
    std::cout << " | \033[1;4;34mColor:\033[0m ("
              << std::get<0>(this->color) << ", "
              << std::get<1>(this->color) << ", "
              << std::get<2>(this->color) << ")\033[0m" << std::endl;
}

void Polygon::display()
{
    glColor3f(std::get<0>(this->color) / 255.0f,
              std::get<1>(this->color) / 255.0f,
              std::get<2>(this->color) / 255.0f);  // Set the polygon's color!

    glBegin(GL_POLYGON);
    for(size_t i = 0; i < this->coordinates.size(); i++)
    {
        float x = std::get<0>(this->coordinates[i]);
        float y = std::get<1>(this->coordinates[i]);
        float z = std::get<2>(this->coordinates[i]);

        glVertex3f(x, y, z);
    }
    glEnd();
}