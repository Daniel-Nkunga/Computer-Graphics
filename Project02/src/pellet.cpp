#include <GL/gl.h>
#include <pellet.hpp>
#include <iostream>
#include <tuple>
#include <vector>
#include <cmath>
#include <math.h>

#define TWO_PI (2.0f * M_PI)

Pellet::Pellet(std::tuple<float, float, float> position, float radius,
               std::tuple<float, float, float> color)
{
    this->position   = position;
    this->radius     = radius;
    this->color      = color;
}

Pellet::Pellet(std::tuple<float, float, float> position)
{
    this->position = position;
}

// ── Setters ──────────────────────────────────

void Pellet::setPosition(std::tuple<float, float, float> position)  { this->position = position; }
void Pellet::setRadius(float radius)                                 { this->radius   = radius;   }
void Pellet::setColor(std::tuple<float, float, float> color)         { this->color    = color;    }

// ── Getters ──────────────────────────────────

std::tuple<float, float, float> Pellet::getPosition() const { return this->position; }

// ── Display ──────────────────────────────────

void Pellet::display()
{
    float cx = std::get<0>(this->position);
    float cy = std::get<1>(this->position);
    float cz = std::get<2>(this->position);

    glColor3f(std::get<0>(this->color) / 255.0f,
              std::get<1>(this->color) / 255.0f,
              std::get<2>(this->color) / 255.0f);

    glBegin(GL_POLYGON);
        for (int i = 0; i < NUM_DIVISIONS; i++)
        {
            float t  = (float)i / NUM_DIVISIONS * TWO_PI;
            float vx = cx + radius * cos(t);
            float vy = cy + radius * sin(t);
            glVertex3f(vx, vy, cz);
        }
    glEnd();
}

// ── Debug ─────────────────────────────────────

void Pellet::debug()
{
    std::cout << "\033[1;4;37mPacman:\033[0m "
              << "Position: (" << std::get<0>(position) << ", "
              << std::get<1>(position) << ", " << std::get<2>(position) << ") | "
              << "Radius: " << radius << " | "
              << "Color: (" << std::get<0>(color) << ", "
              << std::get<1>(color) << ", " << std::get<2>(color) << ")"
              << std::endl;
}