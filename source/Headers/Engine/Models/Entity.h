#pragma once
#include "Headers/Engine/Models/Mesh.h"
#include "Headers/Engine/Models/Texture.h"
#include "Headers/Engine/Shader.h"
#include "Headers/Engine/Camera.h"
#include <glm/gtc/type_ptr.hpp>

class Entity {

public:
    Entity(const Mesh& mesh, const std::vector<Texture>& textures, const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale);

    ~Entity();

    void render(Camera& camera, Shader& shader, glm::vec3& lightPos, glm::vec3& lightColor, glm::mat3x3 modelViewMatrix3x3);

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
    std::vector<Texture> textures;
    glm::vec3 rotation, position, scale;
    glm::mat4 modelMatrix;
    void limitRotation();
};


