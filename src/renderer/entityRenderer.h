#ifndef ENTITY_RENDERER_H
#define ENTITY_RENDERER_H

#include "../shaders/shader.h"
#include "../game_object/entity.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

class EntityRenderer {
public:
  static void renderEntity(
    Shader* shader,
    Entity* entity,
    bool useVertColor    = false,
    glm::mat4 projection = glm::mat4(1.0f),
    glm::mat4 view       = glm::mat4(1.0f),
    glm::mat4 model      = glm::mat4(1.0f),
    glm::vec3 lightPos   = glm::vec3(0.0f),
    glm::vec3 viewPos    = glm::vec3(0.0f),
    glm::vec3 vertColor  = glm::vec3(-1.0f));
};

#endif