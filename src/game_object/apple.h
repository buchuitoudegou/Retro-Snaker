#ifndef APPLE_H
#define APPLE_H

#include "entity.h"
#include "glm/glm.hpp"
#include <time.h> 
#include <cstdlib>
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "../model_loader/loader.h"

class Apple : public Entity {
public:
  void draw(Shader* shader);
  Apple();
  ~Apple();
  void randPosition(float, float);
  glm::vec3 position;
  ModelLoader* loader;
};

#endif