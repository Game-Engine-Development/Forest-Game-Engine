#pragma once

#include <vector>
#include <glm/gtc/type_ptr.hpp>
#include "Headers/Engine/Models/Mesh.h"
#include "Headers/Engine/Models/Texture.h"
#include "Headers/Engine/Shader/Shader.h"
#include "Headers/Engine/Camera/Camera.h"
#include "Headers/Engine/Collisions/Plane.h"

class Entity {
public:
    Entity();

    Entity(
            Mesh &mesh,
            const std::vector<Texture>& textures,
            const glm::vec3& position,
            const glm::vec3& rotation,
            const glm::vec3& scale
    );

    void create(
            Mesh &mesh,
            const std::vector<Texture>& textures,
            const glm::vec3& position,
            const glm::vec3& rotation,
            const glm::vec3& scale
    );

    ~Entity();

    void render(Camera& camera, Shader& shader, glm::vec3& lightPos, glm::vec3& lightColor);

    glm::vec3 getScale();
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
    void setAsPlayerEntity();
    bool checkIfPlayerEntity();
    void setAsBullet();
    bool checkIfBullet();
    void setAsAnimal();
    bool checkIfAnimal();
    void setFlipped();
    glm::mat4 createModelMatrix();
    std::vector<Plane> planes;
    bool hit = false;
private:
    Mesh mesh;
    std::vector<Texture> textures;
    glm::vec3 rotation, position, scale;
    glm::mat4 modelMatrix;
    bool isPlayerEntity = false;
    bool isBullet = false;
    bool isAnimal = false;
    float flipped = -1;
    void limitRotation();
    void moveEntityPlanes(std::vector<glm::vec3>& vertices);
};


