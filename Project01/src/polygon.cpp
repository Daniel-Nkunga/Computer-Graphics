#include "polygon.hpp"
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

void Polygon::display()
{
    // Display coordinates
    std::cout << "Coordinates: ";
    for (size_t i = 0; i < this->coordinates.size(); i++)
    {
        auto& coord = this->coordinates[i];
        std::cout << "(" << std::get<0>(coord) << ", "
                  << std::get<1>(coord) << ", "
                  << std::get<2>(coord) << ")";

        if (i < this->coordinates.size() - 1)
            std::cout << ", ";
    }

    // Display color
    std::cout << " | Color: RGB("
              << std::get<0>(this->color) << ", "
              << std::get<1>(this->color) << ", "
              << std::get<2>(this->color) << ")" << std::endl;
}