#include "Headers/Engine/Models/Entity.h"

Entity::Entity(const Mesh &mesh, const std::vector<Texture> &textures, const glm::vec3 &position = glm::vec3(0,0,0),
               const glm::vec3 &rotation = glm::vec3(0,0,0), const glm::vec3& scale = glm::vec3(1,1,1)) {
    this->mesh = mesh;
    this->textures = textures;
    this->position = position;
    this->rotation = rotation;
    this->scale = scale;
}

Entity::~Entity() = default;

void Entity::render(Camera& camera, Shader& shader, glm::vec3& lightPos, glm::vec3& lightColor) {
    shader.use();

    mesh.bindVAO();
    for(Texture texture : textures) {
        texture.bind(shader);
    }

    camera.setMatrices(shader);

    int modelLoc = glGetUniformLocation(shader.ID, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(createModelMatrix()));

    int lightLoc = glGetUniformLocation(shader.ID, "lightPos");
    glUniform3fv(lightLoc, 1, glm::value_ptr(lightPos));

    int lightCol = glGetUniformLocation(shader.ID, "lightColor");
    glUniform3fv(lightCol, 1, glm::value_ptr(lightColor));

    int viewLoc = glGetUniformLocation(shader.ID, "viewPos");
    glUniform3fv(viewLoc, 1, glm::value_ptr(camera.getPos()));

    glDrawArrays(GL_TRIANGLES, 0, mesh.getNumOfVertices());

    for (Texture texture : textures) {
        texture.unbind();
    }
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
    limitRotation();
}

void Entity::rotate(float x, float y, float z) {
    rotation += glm::vec3(x,y,z);
    limitRotation();
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

void Entity::limitRotation() {
    while(rotation.x >= 360) {
        rotation.x -= 360;
    }
    while(rotation.y >= 360) {
        rotation.y -= 360;
    }
    while(rotation.z >= 360) {
        rotation.z -= 360;
    }
    while(rotation.x <= 0) {
        rotation.x += 360;
    }
    while(rotation.y <= 0) {
        rotation.y += 360;
    }
    while(rotation.z <= 0) {
        rotation.z += 360;
    }
}

glm::vec3 Entity::getScale() {
    return scale;
}