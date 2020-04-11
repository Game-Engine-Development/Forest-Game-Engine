#pragma once

#include <memory>
#include <vector>
#include <glm/gtc/type_ptr.hpp>

#include "Headers/Engine/Models/Mesh.h"
#include "Headers/Engine/Models/Texture.h"
#include "Headers/Engine/Shader/Shader.h"
#include "Headers/Engine/Camera/Camera.h"
#include "Headers/Engine/Collisions/Plane.h"
#include "Headers/Engine/Graphics/Materials/Material.h"
#include <Headers/Engine/Graphics/Lighting/PointLight.h>


class Entity {
public:
    Entity();

    Entity(
            const std::shared_ptr<Mesh> &mesh,
            const std::vector<Texture>& textures,
            const glm::vec3& position,
            const glm::vec3& rotation,
            const glm::vec3& scale
    );

    Entity(
            const std::shared_ptr<Mesh> &mesh,
            const Material &material,
            const Shader &shader,
            const glm::vec3 &position,
            const glm::vec3 &rotation,
            const glm::vec3& scale
    );
    void create(
            const std::shared_ptr<Mesh> &mesh,
            const std::vector<Texture>& textures,
            const glm::vec3& position,
            const glm::vec3& rotation,
            const glm::vec3& scale
    );

    ~Entity();

    void render(
            Camera& camera,
            Shader& shader,
            glm::vec3& lightPos,
            glm::vec3& lightColor
    );

    void render(
            Camera &camera,
            std::vector<PointLight> pointLights
    );


    glm::vec3 getScale();
    glm::vec3 getPos();
    glm::vec3 getRotation();
    void setPos(glm::vec3& newPos);
    void setPos(glm::vec3&& newPos);
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
    void setAsItem();
    bool checkIfItem();
    void setFlipped();
    glm::mat4 createModelMatrix();
    std::vector<Plane> planes;
    bool hit = false;
    bool pickedUp = false;
    double verticalOffset = 0;
private:
    std::shared_ptr<Mesh> mesh = nullptr;
    std::vector<Texture> textures;
    Shader shader;
    Material material;
    glm::vec3 rotation, position, scale;
    glm::mat4 modelMatrix;
    bool isPlayerEntity = false;
    bool isBullet = false;
    bool isAnimal = false;
    bool isItem = false;
    float flipped = -1;
    void limitRotation();
    void moveEntityPlanes(std::vector<glm::vec3>& vertices);

};


