#include "snake.h"

float Snake::turnAngle = 0.5f;

Snake::Snake() {
  bodies.push_back(SnakeBody());
  bodies[0].position = glm::vec3(4, 0, 12);
  opIndices.push_back(0);

  bodies.push_back(SnakeBody());
  bodies[1].position = glm::vec3(4, 0, 10);
  opIndices.push_back(0);

  bodies.push_back(SnakeBody());
  bodies[2].position = glm::vec3(4, 0, 8);
  opIndices.push_back(0);

  bodies.push_back(SnakeBody());
  bodies[3].position = glm::vec3(4, 0, 6);
  opIndices.push_back(0);

  bodies.push_back(SnakeBody());
  bodies[4].position = glm::vec3(4, 0, 4);
  opIndices.push_back(0);

  bodies.push_back(SnakeBody());
  bodies[5].position = glm::vec3(4, 0, 2);
  opIndices.push_back(0);

  headOps.push_back(make_pair(bodies[0].position, bodies[0].bodyDir));
  snakeSpeed = 10.0f;
}

void Snake::move(GLfloat dtime) {
  float offset = dtime * snakeSpeed;
  glm::vec3 bodyDir = glm::vec3(sin(glm::radians(bodies[0].bodyDir)), 0,
     cos(glm::radians(bodies[0].bodyDir)));
  bodies[0].position = bodies[0].position + bodyDir * offset;
  headOps.push_back(make_pair(bodies[0].position, bodies[0].bodyDir));
  for (int i = 1; i < bodies.size(); ++i) {
    if (bodies[i].isInTrack) {
      int idx = opIndices[i];
      bodies[i].position = headOps[idx].first;
      bodies[i].bodyDir = headOps[idx].second;
      opIndices[i] ++;
    } else {
      glm::vec3 bodyDir = glm::vec3(sin(glm::radians(bodies[i].bodyDir)), 0,
        cos(glm::radians(bodies[i].bodyDir)));
      bodies[i].position += bodyDir * offset;
      if (glm::distance2(bodies[i].position, headOps[0].first) < 0.5) {
        bodies[i].isInTrack = true;
        opIndices[i] ++;
      }
    }
  }
  if (bodies[bodies.size() - 1].isInTrack && headOps.size() > bodies.size() - 1) {
    vector<pair<glm::vec3, float> > newHeadOps;
    for (int i = opIndices[bodies.size() - 1]; i < headOps.size(); ++i) {
      newHeadOps.push_back(headOps[i]);
    }
    for (int i = 0; i < opIndices.size(); ++i) {
      opIndices[i] -= (headOps.size() - newHeadOps.size());
    }
    headOps = newHeadOps;
  }
}

void Snake::turn(float angle) {
  bodies[0].bodyDir += angle;
}

int Snake::getLength() const {
  return this->bodies.size();
}

void Snake::moveTo(glm::vec3 newPos) {
  bodies[0].position = newPos;
}

void Snake::lengthen() {
  SnakeBody newBody;
  newBody.bodyDir = bodies[bodies.size() - 1].bodyDir;
  glm::vec3 dir = glm::vec3(-2 * sin(glm::radians(bodies[bodies.size() - 1].bodyDir)), 0,
     -cos(glm::radians(bodies[bodies.size() - 1].bodyDir)) * 2);
  glm::vec3 pos = bodies[bodies.size() - 1].position + dir;
  newBody.position = pos;
  newBody.isInTrack = false;
  // cout << pos.x << " " << pos.y << " " << pos.z << endl;
  bodies.push_back(newBody);
  opIndices.push_back(0);

}