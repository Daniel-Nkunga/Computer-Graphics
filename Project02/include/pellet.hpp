#pragma once
#include <iostream>
#include <tuple>
#include <vector>

class Pellet
{
    private:
        std::tuple<float, float, float> position; // Center coordinate of Pellet
        float radius = 0.020832f;
        std::tuple<float, float, float> color = {255.0f, 255.0f, 255.0f};
        static const int NUM_DIVISIONS = 12;

    public:
        Pellet(std::tuple<float, float, float> position, float radius,
               std::tuple<float, float, float> color = {255.0f, 255.0f, 255.0f});
        Pellet(std::tuple<float, float, float> position);
        ~Pellet() {};

        void setPosition(std::tuple<float, float, float> position);
        void setRadius(float radius);
        void setColor(std::tuple<float, float, float> color);

        std::tuple<float, float, float> getPosition() const;

        void display();
        void debug();
};