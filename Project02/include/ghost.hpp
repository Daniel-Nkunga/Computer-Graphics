#pragma once
#include <iostream>
#include <polygon.hpp>
#include <tuple>
#include <vector>

class Ghost : public Polygon {
private:
  std::tuple<float, float, float> position;
  float radius;
  std::tuple<float, float, float> color;
  std::tuple<float, float, float> eyeColor = {255.0f, 255.0f, 255.0f};
  static const int NUM_DIVISIONS = 20;
  static const int EYE_DIVISIONS = 8;
  float eyeRadius;

  // Movement state
  float dx = 0.0f;
  float dy = 0.0f;
  int framesUntilTurn = 0;

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
  float getRadius() const;

  // Initialise the first random direction (call once after construction)
  void initMovement(float speed, int targetFPS);

  // Move the ghost one frame; picks new direction when timer expires
  void update(float speed, int targetFPS);

  void display();
  void debug();
};
