#include <GL/gl.h>
#include <GL/glut.h>
#include <cmath>
#include <iostream>
#include <math.h>
#include <pacman.hpp>
#include <tuple>
#include <vector>

#define TWO_PI (2.0f * M_PI)

// Forward declaration of the global instance defined in main.cpp
extern Pacman pacman;

Pacman::Pacman(std::tuple<float, float, float> position, float radius,
               std::tuple<float, float, float> color, float mouthAngle) {
  this->position = position;
  this->radius = radius;
  this->color = color;
  this->mouthAngle = mouthAngle;
}

// ── Setters ──────────────────────────────────────────────────────────────────

void Pacman::setPosition(std::tuple<float, float, float> position) {
  this->position = position;
}
void Pacman::setRadius(float radius) { this->radius = radius; }
void Pacman::setMouthAngle(float angle) { this->mouthAngle = angle; }
void Pacman::setColor(std::tuple<float, float, float> color) {
  this->color = color;
}

// ── Getters ──────────────────────────────────────────────────────────────────

std::tuple<float, float, float> Pacman::getPosition() const {
  return this->position;
}
float Pacman::getRadius() const { return this->radius; }

// ── Update (movement)
// ─────────────────────────────────────────────────────────

void Pacman::update(float speed) {
  float dx = 0.0f, dy = 0.0f;
  if (key_up)
    dy += speed;
  if (key_down)
    dy -= speed;
  if (key_left)
    dx -= speed;
  if (key_right)
    dx += speed;

  if (dx == 0.0f && dy == 0.0f)
    return;

  facingAngle = atan2(dy, dx) * 180.0f / M_PI;

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

// ── Keyboard handlers
// ─────────────────────────────────────────────────────────

void Pacman::keyDown(unsigned char key, int x, int y) {
  if (key == 'w' || key == 'W')
    key_up = true;
  if (key == 's' || key == 'S')
    key_down = true;
  if (key == 'a' || key == 'A')
    key_left = true;
  if (key == 'd' || key == 'D')
    key_right = true;
}

void Pacman::keyUp(unsigned char key, int x, int y) {
  if (key == 'w' || key == 'W')
    key_up = false;
  if (key == 's' || key == 'S')
    key_down = false;
  if (key == 'a' || key == 'A')
    key_left = false;
  if (key == 'd' || key == 'D')
    key_right = false;
}

void Pacman::specialDown(int key, int x, int y) {
  if (key == GLUT_KEY_UP)
    key_up = true;
  if (key == GLUT_KEY_DOWN)
    key_down = true;
  if (key == GLUT_KEY_LEFT)
    key_left = true;
  if (key == GLUT_KEY_RIGHT)
    key_right = true;
}

void Pacman::specialUp(int key, int x, int y) {
  if (key == GLUT_KEY_UP)
    key_up = false;
  if (key == GLUT_KEY_DOWN)
    key_down = false;
  if (key == GLUT_KEY_LEFT)
    key_left = false;
  if (key == GLUT_KEY_RIGHT)
    key_right = false;
}

// ── Static trampolines
// ────────────────────────────────────────────────────────

void PacmanInput::keyDown(unsigned char key, int x, int y) {
  pacman.keyDown(key, x, y);
}
void PacmanInput::keyUp(unsigned char key, int x, int y) {
  pacman.keyUp(key, x, y);
}
void PacmanInput::specialDown(int key, int x, int y) {
  pacman.specialDown(key, x, y);
}
void PacmanInput::specialUp(int key, int x, int y) {
  pacman.specialUp(key, x, y);
}

// ── Display
// ───────────────────────────────────────────────────────────────────

void Pacman::display() {
  float cx = std::get<0>(this->position);
  float cy = std::get<1>(this->position);
  float cz = std::get<2>(this->position);

  glColor3f(std::get<0>(this->color) / 255.0f,
            std::get<1>(this->color) / 255.0f,
            std::get<2>(this->color) / 255.0f);

  float mouthRad = mouthAngle * M_PI / 180.0f;
  float startAngle = mouthRad;
  float endAngle = TWO_PI - mouthRad;

  glPushMatrix();
  glTranslatef(cx, cy, cz);
  glRotatef(facingAngle, 0.0f, 0.0f, 1.0f);
  glTranslatef(-cx, -cy, -cz);

  glBegin(GL_POLYGON);
  glVertex3f(cx, cy, cz);
  for (int i = 0; i <= NUM_DIVISIONS; i++) {
    float t = startAngle + (float)i / NUM_DIVISIONS * (endAngle - startAngle);
    glVertex3f(cx + radius * cos(t), cy + radius * sin(t), cz);
  }
  glEnd();
  glPopMatrix();
}

// ── Debug
// ─────────────────────────────────────────────────────────────────────

void Pacman::debug() {
  std::cout << "\033[1;4;33mPacman:\033[0m "
            << "Position: (" << std::get<0>(position) << ", "
            << std::get<1>(position) << ", " << std::get<2>(position) << ") | "
            << "Radius: " << radius << " | "
            << "Mouth: " << mouthAngle << " deg | "
            << "Color: (" << std::get<0>(color) << ", " << std::get<1>(color)
            << ", " << std::get<2>(color) << ")" << std::endl;
}