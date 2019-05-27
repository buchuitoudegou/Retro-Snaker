#ifndef SNAKE_H
#define SNAKE_H

#include "snakeBody.h"
#include <vector>
#include <iostream>
#include <utility>
#include <glm/gtx/norm.hpp> 

using namespace std;

class Snake {
public:
  vector<SnakeBody> bodies;
  float snakeSpeed;
  static float turnAngle;
  static unsigned int MAX_LENGTH;
  vector<pair<glm::vec3, float> > headOps;
  vector<int> opIndices;
  Snake();
  void moveTo(glm::vec3);
  void lengthen();
  void move(GLfloat dtime);
  void turn(float);
  int getLength() const;
};

#endif