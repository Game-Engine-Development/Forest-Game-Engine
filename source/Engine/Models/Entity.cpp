//
// Created by Alan on 10/4/2019.
//

#include "Headers/Engine/Models/Entity.h"

Entity::Entity(const Mesh &mesh, const Texture &texture, const glm::vec3 &position = glm::vec3(0,0,0),
               const glm::vec3 &rotation = glm::vec3(0,0,0), const glm::vec3& scale = glm::vec3(1,1,1)) {
    this->mesh = mesh;
    this->texture = texture;
    this->position = position;
    this->rotation = rotation;
    this->scale = scale;
}

Entity::~Entity() = default;

void Entity::render(Camera& camera, Shader& shader) {
    shader.use();
    mesh.bindVAO();
    texture.bind();
    camera.setMatrices(shader);
    int modelLoc = glGetUniformLocation(shader.ID, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(createModelMatrix()));
    glDrawArrays(GL_TRIANGLES, 0, mesh.getNumOfVertices());
    texture.unbind();
    mesh.unbindVAO();
}

glm::mat4 Entity::createModelMatrix() {
    modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, position);
    modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation.x), glm::vec3(1,0,0));
    modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation.y), glm::vec3(0,1,0));
    modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation.z), glm::vec3(0,0,1));
    modelMatrix = glm::scale(modelMatrix, scale);
    return modelMatrix;
}

glm::vec3 Entity::getPos(){
    return position;
}

glm::vec3 Entity::getRotation() {
    return rotation;
}

void Entity::setRotation(glm::vec3& newRotation) {
    rotation = newRotation;
}

void Entity::setPos(glm::vec3& newPos) {
    position = newPos;
}

void Entity::rotate(glm::vec3& rotation) {
    this->rotation += rotation;
}

void Entity::rotate(float x, float y, float z) {
    rotation += glm::vec3(x,y,z);
}

void Entity::translate(glm::vec3 &translation) {
    position += translation;
}

void Entity::translate(float x, float y, float z) {
    position += glm::vec3(x,y,z);
}

void Entity::addScale(glm::vec3 &scale) {
    this->scale += scale;
}

void Entity::addScale(float x, float y, float z) {
    this->scale += glm::vec3(x,y,z);
}