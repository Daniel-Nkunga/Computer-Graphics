#include <GL/gl.h>
#include <cmath>
#include <cstdlib>
#include <ghost.hpp>
#include <iostream>
#include <tuple>
#include <vector>

#ifndef M_PI
#define M_PI 3.14159265358979323846f
#endif
#define TWO_PI (2.0f * M_PI)

Ghost::Ghost(std::tuple<float, float, float> position, float radius,
             std::tuple<float, float, float> color)
    : Polygon({}, color) {
  this->position = position;
  this->radius = radius;
  this->color = color;
  this->eyeRadius = radius * 0.2f;
}

// ── Setters
// ───────────────────────────────────────────────────────────────────

void Ghost::setPosition(std::tuple<float, float, float> position) {
  this->position = position;
}
void Ghost::setRadius(float radius) { this->radius = radius; }
void Ghost::setColor(std::tuple<float, float, float> color) {
  this->color = color;
}
void Ghost::setEyeColor(std::tuple<float, float, float> color) {
  this->eyeColor = color;
}

// ── Getters
// ───────────────────────────────────────────────────────────────────

std::tuple<float, float, float> Ghost::getPosition() const {
  return this->position;
}
float Ghost::getRadius() const { return this->radius; }

// ── Movement helpers
// ──────────────────────────────────────────────────────────

static float randomVelocityFor(float speed) {
  int r = rand() % 5;
  if (r == 0)
    return 0.0f;
  else if (r <= 2)
    return speed;
  else
    return -speed;
}

static int randomIntervalFor(int targetFPS) {
  return targetFPS / 2 + rand() % (targetFPS * 2);
}

void Ghost::initMovement(float speed, int targetFPS) {
  dx = randomVelocityFor(speed);
  dy = randomVelocityFor(speed);
  framesUntilTurn = randomIntervalFor(targetFPS);
}

void Ghost::update(float speed, int targetFPS) {
  framesUntilTurn--;
  if (framesUntilTurn <= 0) {
    dx = randomVelocityFor(speed * 0.75f);
    dy = randomVelocityFor(speed * 0.75f);
    framesUntilTurn = randomIntervalFor(targetFPS);
  }

  float x = std::get<0>(this->position) + dx;
  float y = std::get<1>(this->position) + dy;

  // Screenwrap
  if (x < -1.0f)
    x = 1.0f;
  if (x > 1.0f)
    x = -1.0f;
  if (y < -1.0f)
    y = 1.0f;
  if (y > 1.0f)
    y = -1.0f;

  this->position = {x, y, 0.0f};
}

// ── Private helper
// ────────────────────────────────────────────────────────────

void Ghost::drawCircle(float cx, float cy, float cz, float r, int divisions,
                       std::tuple<float, float, float> col) {
  glColor3f(std::get<0>(col) / 255.0f, std::get<1>(col) / 255.0f,
            std::get<2>(col) / 255.0f);

  glBegin(GL_POLYGON);
  for (int i = 0; i < divisions; i++) {
    float t = (float)i / divisions * TWO_PI;
    glVertex3f(cx + r * cos(t), cy + r * sin(t), cz);
  }
  glEnd();
}

// ── Display
// ───────────────────────────────────────────────────────────────────

void Ghost::display() {
  float cx = std::get<0>(this->position);
  float cy = std::get<1>(this->position);
  float cz = std::get<2>(this->position);

  drawCircle(cx, cy, cz, radius, NUM_DIVISIONS, color);

  float eyeOffsetX = radius * 0.35f;
  float eyeOffsetY = radius * 0.25f;

  drawCircle(cx - eyeOffsetX, cy + eyeOffsetY, cz + 0.01f, eyeRadius,
             EYE_DIVISIONS, eyeColor);
  drawCircle(cx + eyeOffsetX, cy + eyeOffsetY, cz + 0.01f, eyeRadius,
             EYE_DIVISIONS, eyeColor);
}

// ── Debug
// ─────────────────────────────────────────────────────────────────────

void Ghost::debug() {
  std::cout << "\033[1;4;35mGhost:\033[0m "
            << "Position: (" << std::get<0>(position) << ", "
            << std::get<1>(position) << ", " << std::get<2>(position) << ") | "
            << "Radius: " << radius << " | "
            << "Color: (" << std::get<0>(color) << ", " << std::get<1>(color)
            << ", " << std::get<2>(color) << ")" << std::endl;
}
