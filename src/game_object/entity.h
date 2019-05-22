#ifndef ENTITY_H
#define ENTITY_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Entity {
public:
  virtual void draw() = 0;
};

#endif