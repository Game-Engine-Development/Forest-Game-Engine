#pragma once

#include "Headers/Engine/Camera/Camera.h"
#include "CubeMapTexture.h"

class Skybox {
public:
    explicit Skybox(const CubeMapTexture& texture);

    ~Skybox();

    void render(Shader& shader, Camera& camera);
private:
    unsigned int VBO, VAO; //@todo write move constructor and move assignment operator or use pointer to container to protect opengl resources from being shallow copied and then destroyed accidentally
    CubeMapTexture skyboxTexture;
};
