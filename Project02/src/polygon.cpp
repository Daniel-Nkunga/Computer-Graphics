#include <GL/gl.h>
#include <polygon.hpp>
#include <iostream>
#include <tuple>
#include <vector>
#include <cmath>

Polygon::Polygon(std::vector<std::tuple<float, float, float>> coordinates,
                 std::tuple<float, float, float> color)
{
    this->coordinates = coordinates;
    this->color = color;
}

void Polygon::set(std::vector<std::tuple<float, float, float>> coordinates,
                  std::tuple<float, float, float> color)
{
    this->coordinates = coordinates;
    this->color = color;
}

void Polygon::set(std::vector<std::tuple<float, float, float>> coordinates)
{
    this->coordinates = coordinates;
}

void Polygon::set(std::tuple<float, float, float> color)
{
    this->color = color;
}

void Polygon::add(std::tuple<float, float, float> point)
{
    this->coordinates.push_back(point);
}

void Polygon::debug()
{
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
    std::cout << " | \033[1;4;34mColor:\033[0m ("
              << std::get<0>(this->color) << ", "
              << std::get<1>(this->color) << ", "
              << std::get<2>(this->color) << ")\033[0m" << std::endl;
}

void Polygon::display()
{
    glColor3f(std::get<0>(this->color) / 255.0f,
              std::get<1>(this->color) / 255.0f,
              std::get<2>(this->color) / 255.0f);

    glBegin(GL_POLYGON);
    for (size_t i = 0; i < this->coordinates.size(); i++)
    {
        glVertex3f(std::get<0>(this->coordinates[i]),
                   std::get<1>(this->coordinates[i]),
                   std::get<2>(this->coordinates[i]));
    }
    glEnd();
}