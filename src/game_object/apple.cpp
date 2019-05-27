#include "apple.h"

Apple::Apple() {
  loader = new ModelLoader("./assets/Apple.obj");
  srand(time(NULL));
  position = glm::vec3(4.0, 0, 16.0);
}

Apple::~Apple() {
  delete loader;
}

void Apple::draw(Shader* shader) {
  shader->setBool("useMeshColor", true);
  loader->draw(*shader);
}

void Apple::randPosition(float width, float height) {
  float randX = 2 + (width - 8) * rand() / (float)(RAND_MAX);
  float randZ = 2 + (height - 8) * rand() / (float)(RAND_MAX);
  position = glm::vec3(randX, 0, randZ);
}