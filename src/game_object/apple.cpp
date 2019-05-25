#include "apple.h"

Apple::Apple() {
  loader = new ModelLoader("./assets/Apple.obj");
  srand(time(NULL));
}

Apple::~Apple() {
  delete loader;
}

void Apple::draw(Shader* shader) {
  shader->setBool("useMeshColor", true);
  loader->draw(*shader);
}

void Apple::randPosition(float width, float height) {
  float randX = 0 + width * rand() / (float)(RAND_MAX + 1);
  float randZ = 0 + height * rand() / (float)(RAND_MAX + 1);
  position = glm::vec3(randX, 0, randZ);
}