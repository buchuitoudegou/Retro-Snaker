#ifndef ENTITY_H
#define ENTITY_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "../shaders/shader.h"

class Entity {
public:
  virtual void draw(Shader* shader = nullptr) = 0;
};

#endif