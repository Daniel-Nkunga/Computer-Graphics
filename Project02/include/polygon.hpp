#pragma once
#include <iostream>
#include <tuple>
#include <vector>

class Polygon
{
    private:
        std::vector<std::tuple<float, float, float>> coordinates;
        std::tuple<float, float, float> color;
    public:
        Polygon(std::vector<std::tuple<float, float, float>>, std::tuple<float, float, float>);
        ~Polygon() {};
        void set(std::vector<std::tuple<float, float, float>>, std::tuple<float, float, float>);
        void set(std::vector<std::tuple<float, float, float>>);
        void set(std::tuple<float, float, float>);
        void add(std::tuple<float, float, float>);
        void debug();
        void display();
};