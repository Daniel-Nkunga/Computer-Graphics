#include <GL/freeglut_std.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <tuple>
#include <vector>
#ifdef MAC
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
using namespace std;

// Custom classes
#include <ghost.hpp>
#include <pacman.hpp>
#include <pellet.hpp>
#include <polygon.hpp>

// Functions
void init();
void display();
void update(int value);
void drawHUD();
bool checkCollision(tuple<float, float, float> posA, float rA,
                    tuple<float, float, float> posB, float rB);
void checkAllCollisions();
void gameOver(int code);

#define TARGET_FPS 30
#define FRAME_DELAY (1000 / TARGET_FPS)
#define PACMAN_SPEED 0.0085f
#define GHOST_SPEED 0.0075f

#define Blinky 0
#define Pinky 1
#define Inky 2
#define CLIDE 3

Pacman pacman({0.0f, 0.0f, 0.0f}, 0.0625f, {255.0f, 255.0f, 0.0f}, 30.0f);
vector<Ghost> ghosts;
vector<Pellet> pellets;
int pelletsEaten = 0;
int maxPellets = 100;

int main(int argc, char *argv[]) {
  //   pacman.debug();

  srand(time(0));

  for (int i = 0; i < maxPellets; i++) {
    float x = (static_cast<float>(rand()) / RAND_MAX) * 2.0f - 1.0f;
    float y = (static_cast<float>(rand()) / RAND_MAX) * 2.0f - 1.0f;
    pellets.push_back(Pellet({x, y, 0.0f}));
  }

  glutInit(&argc, argv);
  glutInitWindowSize(500, 500);
  glutInitWindowPosition(250, 250);
  glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE | GLUT_DEPTH);
  glutCreateWindow("Pacman - Nkunga");
  glutTimerFunc(FRAME_DELAY, update, 0);
  glutDisplayFunc(display);
  glutKeyboardFunc(PacmanInput::keyDown);
  glutKeyboardUpFunc(PacmanInput::keyUp);
  glutSpecialFunc(PacmanInput::specialDown);
  glutSpecialUpFunc(PacmanInput::specialUp);
  glutSetKeyRepeat(GLUT_KEY_REPEAT_OFF);
  init();
  glutMainLoop();
  return 0;
}

void init() {
  glClearColor(0.0, 0.0, 0.0, 1.0);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
  glMatrixMode(GL_MODELVIEW);
  glEnable(GL_DEPTH_TEST);

  ghosts.push_back(
      Ghost({-0.5f, -0.5f, 0.0f}, 0.0625f, {255.0f, 0.0f, 0.0f})); // Blinky
  ghosts.push_back(
      Ghost({-0.5f, 0.5f, 0.0f}, 0.0625f, {255.0f, 184.0f, 255.0f})); // Pinky
  ghosts.push_back(
      Ghost({0.5f, 0.5f, 0.0f}, 0.0625f, {0.0f, 255.0f, 255.0f})); // Inky
  ghosts.push_back(
      Ghost({0.5f, -0.5f, 0.0f}, 0.0625f, {255.0f, 184.0f, 82.0f})); // Clide

  for (size_t i = 0; i < ghosts.size(); i++)
    ghosts[i].initMovement(GHOST_SPEED, TARGET_FPS);
}

void display() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  pacman.display();
  for (size_t i = 0; i < pellets.size(); i++)
    pellets[i].display();
  for (size_t i = 0; i < ghosts.size(); i++)
    ghosts[i].display();

  drawHUD();
  glFlush();
}

void update(int value) {
  for (size_t i = 0; i < pellets.size(); i++)
    pellets[i].update();

  for (size_t i = 0; i < ghosts.size(); i++)
    ghosts[i].update(GHOST_SPEED, TARGET_FPS);

  pacman.update(PACMAN_SPEED);

  checkAllCollisions();

  if (pellets.empty()) {
    gameOver(0);
    cout << "YOU WIN! GREAT JOB!" << endl;
    exit(0);
  }

  glutPostRedisplay();
  glutTimerFunc(FRAME_DELAY, update, value);
}

void drawHUD() {
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  gluOrtho2D(0, 500, 0, 500);
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity();

  glDisable(GL_DEPTH_TEST);
  glColor3f(0.0f, 1.0f, 0.0f); // Lime green

  string score = "Pellets: " + to_string(pelletsEaten);
  int x = 500 - (int)score.size() * 10 - 10;
  int y = 500 - 24;

  glRasterPos2i(x, y);
  for (char c : score)
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);

  glEnable(GL_DEPTH_TEST);
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  glMatrixMode(GL_MODELVIEW);
  glPopMatrix();
}

// Returns true if two circles overlap (2D — x and y only).
bool checkCollision(tuple<float, float, float> posA, float rA,
                    tuple<float, float, float> posB, float rB) {
  float dx = get<0>(posA) - get<0>(posB);
  float dy = get<1>(posA) - get<1>(posB);
  return sqrt(dx * dx + dy * dy) < (rA + rB);
}

void checkAllCollisions() {
  const string ghostNames[] = {"Blinky", "Pinky", "Inky", "Clide"};

  // Pacman vs ghosts
  for (size_t i = 0; i < ghosts.size(); i++) {
    if (checkCollision(pacman.getPosition(), pacman.getRadius(),
                       ghosts[i].getPosition(), ghosts[i].getRadius())) {
      gameOver(1);
      cout << "Pacman collided with ghost: " << ghostNames[i] << endl;
      cout << "Pellets remaining: " << maxPellets - pelletsEaten << endl;
      exit(0);
    }
  }

  // Pacman vs pellets
  for (size_t i = 0; i < pellets.size(); i++) {
    if (checkCollision(pacman.getPosition(), pacman.getRadius(),
                       pellets[i].getPosition(), 0.020832f)) {
      pelletsEaten++;
      pellets.erase(pellets.begin() + i);
      i--;
    }
  }
}

void gameOver(int code) {
  switch (code) {
  case 0:
    cout << "\033[1;32m";
    break;
  case 1:
    cout << "\033[1;31m";
    break;
  default:
    cout << "Something when terribly wrong" << endl;
  }
  cout << " $$$$$$\\   $$$$$$\\  $$\\      $$\\ $$$$$$$$\\     $$$$$$\\  $$\\  "
          "  $$\\ $$$$$$$$\\ $$$$$$$\\  $$\\ \n"
       << "$$  __$$\\ $$  __$$\\ $$$\\    $$$ |$$  _____|   $$  __$$\\ $$ |   "
          "$$ |$$  _____|$$  __$$\\ $$ |\n"
       << "$$ /  \\__|$$ /  $$ |$$$$\\  $$$$ |$$ |         $$ /  $$ |$$ |   $$ "
          "|$$ |      $$ |  $$ |$$ |\n"
       << "$$ |$$$$\\ $$$$$$$$ |$$\\$$\\$$ $$ |$$$$$\\       $$ |  $$ |\\$$\\  "
          "$$  |$$$$$\\    $$$$$$$  |$$ |\n"
       << "$$ |\\_$$ |$$  __$$ |$$ \\$$$  $$ |$$  __|      $$ |  $$ | \\$$\\$$ "
          " / $$  __|   $$  __$$< \\__|\n"
       << "$$ |  $$ |$$ |  $$ |$$ |\\$  /$$ |$$ |         $$ |  $$ |  \\$$$  / "
          " $$ |      $$ |  $$ |    \n"
       << "\\$$$$$$  |$$ |  $$ |$$ | \\_/ $$ |$$$$$$$$\\    $$$$$$  |   \\$  / "
          "  $$$$$$$$\\ $$ |  $$ |$$\\ \n"
       << " \\______/ \\__|  \\__|\\__|     \\__|\\________|    \\______/     "
          "\\_/    \\________|\\__|  \\__|\\__|\n"
       << endl;

  cout << "\033[0m";
}