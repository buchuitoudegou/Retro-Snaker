#include "snake.h"

float Snake::turnAngle = 0.5f;

Snake::Snake() {
  bodies.push_back(SnakeBody());
  bodies[0].position = glm::vec3(2, 0, 2);
}

void Snake::move(GLfloat dtime) {
  float offset = dtime * snakeSpeed;
  glm::vec3 snakeDir = glm::vec3(sin(glm::radians(bodies[0].bodyDir)), 0,
    cos(glm::radians(bodies[0].bodyDir)));
  glm::vec3 newPos = bodies[0].position + snakeDir * offset;
  for (auto i = 1; i < bodies.size(); ++i) {
    bodies[i].position = bodies[i - 1].position;
  }
  bodies[0].position = newPos;
}

void Snake::turn(float angle) {
  float newDir = bodies[0].bodyDir + angle;
  for (int i = 1; i< bodies.size(); ++i) {
    bodies[i].bodyDir = bodies[i - 1].bodyDir;
  }
  bodies[0].bodyDir = newDir;
}