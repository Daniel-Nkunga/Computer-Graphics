#include <GL/gl.h>
#include <ghost.hpp>
#include <iostream>
#include <tuple>
#include <vector>
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846f
#endif
#define TWO_PI (2.0f * M_PI)

Ghost::Ghost(std::tuple<float, float, float> position, float radius,
             std::tuple<float, float, float> color)
    : Polygon({}, color) // Pass empty coords up to Polygon base; Ghost draws itself
{
    this->position  = position;
    this->radius    = radius;
    this->color     = color;
    this->eyeRadius = radius * 0.2f; // Eyes are 20% of the body radius
}

// ── Setters ───────────────────────────────────────────────────────────────────

void Ghost::setPosition(std::tuple<float, float, float> position) { this->position  = position; }
void Ghost::setRadius(float radius)                               { this->radius    = radius;   }
void Ghost::setColor(std::tuple<float, float, float> color)       { this->color     = color;    }
void Ghost::setEyeColor(std::tuple<float, float, float> color)    { this->eyeColor  = color;    }

// ── Getters ───────────────────────────────────────────────────────────────────

std::tuple<float, float, float> Ghost::getPosition() const { return this->position; }

// ── Private helper: draw a filled circle ─────────────────────────────────────

void Ghost::drawCircle(float cx, float cy, float cz, float r, int divisions,
                       std::tuple<float, float, float> col)
{
    glColor3f(std::get<0>(col) / 255.0f,
              std::get<1>(col) / 255.0f,
              std::get<2>(col) / 255.0f);

    glBegin(GL_POLYGON);
        for (int i = 0; i < divisions; i++)
        {
            float t  = (float)i / divisions * TWO_PI;
            glVertex3f(cx + r * cos(t), cy + r * sin(t), cz);
        }
    glEnd();
}

// ── Display ───────────────────────────────────────────────────────────────────

void Ghost::display()
{
    float cx = std::get<0>(this->position);
    float cy = std::get<1>(this->position);
    float cz = std::get<2>(this->position);

    // Body — full circle
    drawCircle(cx, cy, cz, radius, NUM_DIVISIONS, color);

    // Eyes sit in the upper half, offset left and right by 35% of radius
    float eyeOffsetX = radius * 0.35f;
    float eyeOffsetY = radius * 0.25f;

    // Left eye (white) — nudged toward camera so depth test passes over the body
    drawCircle(cx - eyeOffsetX, cy + eyeOffsetY, cz + 0.01f, eyeRadius, EYE_DIVISIONS, eyeColor);

    // Right eye (white)
    drawCircle(cx + eyeOffsetX, cy + eyeOffsetY, cz + 0.01f, eyeRadius, EYE_DIVISIONS, eyeColor);
}

// ── Debug ─────────────────────────────────────────────────────────────────────

void Ghost::debug()
{
    std::cout << "\033[1;4;35mGhost:\033[0m "
              << "Position: (" << std::get<0>(position) << ", "
              << std::get<1>(position) << ", " << std::get<2>(position) << ") | "
              << "Radius: "    << radius << " | "
              << "Color: ("    << std::get<0>(color) << ", "
              << std::get<1>(color) << ", " << std::get<2>(color) << ")"
              << std::endl;
}