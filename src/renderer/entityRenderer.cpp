#include "entityRenderer.h"

void EntityRenderer::renderEntity(
    Shader* shader,
    Entity* entity,
    bool useVertColor,
    glm::mat4 projection,
    glm::mat4 view,
    glm::mat4 model,
    glm::vec3 lightPos,
    glm::vec3 viewPos,
    glm::vec3 vertColor) {
  shader->use();
  shader->setBool("useVertColor", useVertColor);
  shader->setMat4("projection", projection);
  shader->setMat4("view", view);
  shader->setMat4("model", model);
  shader->setVec3("lightPos", lightPos);
  shader->setVec3("viewPos", viewPos);
  shader->setVec3("vertColor", vertColor);
  entity->draw();
  shader->unuse();
} 