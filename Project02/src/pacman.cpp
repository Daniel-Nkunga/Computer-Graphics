#include <GL/gl.h>
#include <pacman.hpp>
#include <iostream>
#include <tuple>
#include <vector>
#include <cmath>
#include <math.h>

#define TWO_PI (2.0f * M_PI)

Pacman::Pacman(std::tuple<float, float, float> position, float radius,
               std::tuple<float, float, float> color, float mouthAngle)
{
    this->position   = position;
    this->radius     = radius;
    this->color      = color;
    this->mouthAngle = mouthAngle;
}

// ── Setters ──────────────────────────────────

void Pacman::setPosition(std::tuple<float, float, float> position)  { this->position   = position;   }
void Pacman::setRadius(float radius)                                 { this->radius     = radius;     }
void Pacman::setMouthAngle(float angle)                              { this->mouthAngle = angle;      }
void Pacman::setColor(std::tuple<float, float, float> color)         { this->color      = color;      }

// ── Getters ──────────────────────────────────

std::tuple<float, float, float> Pacman::getPosition() const { return this->position; }

// ── Display ──────────────────────────────────

void Pacman::display()
{
    float cx = std::get<0>(this->position);
    float cy = std::get<1>(this->position);
    float cz = std::get<2>(this->position);

    glColor3f(std::get<0>(this->color) / 255.0f,
              std::get<1>(this->color) / 255.0f,
              std::get<2>(this->color) / 255.0f);

    // Convert mouth angle to radians
    float mouthRad = mouthAngle * M_PI / 180.0f;

    // Start angle (right-facing mouth: 0 rad is the right side)
    float startAngle = mouthRad;          // lower jaw edge
    float endAngle   = TWO_PI - mouthRad; // upper jaw edge

    glBegin(GL_POLYGON);
        // Center point — required to form the pie-slice / mouth shape
        glVertex3f(cx, cy, cz);

        for (int i = 0; i <= NUM_DIVISIONS; i++)
        {
            float t     = startAngle + (float)i / NUM_DIVISIONS * (endAngle - startAngle);
            float vx    = cx + radius * cos(t);
            float vy    = cy + radius * sin(t);
            glVertex3f(vx, vy, cz);
        }
    glEnd();
}

// ── Debug ─────────────────────────────────────

void Pacman::debug()
{
    std::cout << "\033[1;4;33mPacman:\033[0m "
              << "Position: (" << std::get<0>(position) << ", "
              << std::get<1>(position) << ", " << std::get<2>(position) << ") | "
              << "Radius: " << radius << " | "
              << "Mouth: " << mouthAngle << " deg | "
              << "Color: (" << std::get<0>(color) << ", "
              << std::get<1>(color) << ", " << std::get<2>(color) << ")"
              << std::endl;
}