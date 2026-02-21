#pragma once
#include <iostream>
#include <tuple>
#include <vector>

class Pacman {
private:
  std::tuple<float, float, float> position;
  float radius;
  float mouthAngle; // Degrees for mouth opening (each side)
  std::tuple<float, float, float> color;
  static const int NUM_DIVISIONS = 20;

  float facingAngle = 0.0f; // Degrees; 0 = right, 90 = up, etc.

  // Key state
  bool key_up = false;
  bool key_down = false;
  bool key_left = false;
  bool key_right = false;

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
  float getRadius() const;

  // Called every frame to apply movement from held keys
  void update(float speed);

  // GLUT keyboard callbacks — register these with glutKeyboardFunc etc.
  void keyDown(unsigned char key, int x, int y);
  void keyUp(unsigned char key, int x, int y);
  void specialDown(int key, int x, int y);
  void specialUp(int key, int x, int y);

  void display();
  void debug();
};

// Static trampolines so GLUT callbacks can reach the global pacman instance
namespace PacmanInput {
void keyDown(unsigned char key, int x, int y);
void keyUp(unsigned char key, int x, int y);
void specialDown(int key, int x, int y);
void specialUp(int key, int x, int y);
} // namespace PacmanInput