#pragma once
#include <polygon.hpp>
#include <iostream>
#include <tuple>
#include <vector>

class Ghost : public Polygon
{
    private:
        std::tuple<float, float, float> position;
        float radius;
        std::tuple<float, float, float> color;
        std::tuple<float, float, float> eyeColor = {255.0f, 255.0f, 255.0f};
        static const int NUM_DIVISIONS = 20;
        static const int EYE_DIVISIONS = 8;
        float eyeRadius;

        void drawCircle(float cx, float cy, float cz, float r, int divisions,
                        std::tuple<float, float, float> col);

    public:
        Ghost(std::tuple<float, float, float> position, float radius,
              std::tuple<float, float, float> color);
        ~Ghost() {};

        void setPosition(std::tuple<float, float, float> position);
        void setRadius(float radius);
        void setColor(std::tuple<float, float, float> color);
        void setEyeColor(std::tuple<float, float, float> color);

        std::tuple<float, float, float> getPosition() const;

        void display();
        void debug();
};