//
// Created by Alan on 10/4/2019.
//

#pragma once
#include "Headers/Engine/Models/Mesh.h"
#include "Headers/Engine/Models/Texture.h"
#include "Headers/Shader.h"
#include "Headers/Camera.h"

class Entity {

public:
    Entity(const Mesh& mesh, const Texture& texture, const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale);

    ~Entity();

    void render(Camera& camera, Shader& shader);

    glm::vec3 getPos();
    glm::vec3 getRotation();
    void setPos(glm::vec3& newPos);
    void setRotation(glm::vec3& newRotation);
    void rotate(glm::vec3& rotation);
    void rotate(float x, float y, float z);
    void translate(glm::vec3& translation);
    void translate(float x, float y, float z);
    void addScale(glm::vec3& scale);
    void addScale(float x, float y, float z);
    glm::mat4 createModelMatrix();

private:
    Mesh mesh;
    Texture texture;
    glm::vec3 rotation, position, scale;
    glm::mat4 modelMatrix;
};


