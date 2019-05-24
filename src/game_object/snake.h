#ifndef SNAKE_H
#define SNAKE_H

#include "snakeBody.h"
#include <vector>

using namespace std;

class Snake {
public:
  vector<SnakeBody> bodies;
  int length = 1;
  float snakeSpeed = 0.5;
  static float turnAngle;
  Snake();
  void move(GLfloat dtime);
  void turn(float);
};

#endif