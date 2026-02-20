#pragma once
#include <iostream>
#include <tuple>
#include <vector>

class Pacman
{
    private:
        std::tuple<float, float, float> position; // Center coordinate of Pacman
        float radius;
        float mouthAngle; // Degrees for mouth opening (each side)
        std::tuple<float, float, float> color;
        static const int NUM_DIVISIONS = 20;

    public:
        Pacman(std::tuple<float, float, float> position, float radius,
               std::tuple<float, float, float> color = {255.0f, 255.0f, 0.0f},
               float mouthAngle = 30.0f);
        ~Pacman() {};

        void setPosition(std::tuple<float, float, float> position);
        void setRadius(float radius);
        void setMouthAngle(float angle);
        void setColor(std::tuple<float, float, float> color);

        std::tuple<float, float, float> getPosition() const;

        void display();
        void debug();
};