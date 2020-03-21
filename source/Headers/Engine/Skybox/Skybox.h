#pragma once

#include "Headers/Engine/Camera/Camera.h"
#include "CubeMapTexture.h"

class Skybox {
public:
    explicit Skybox(CubeMapTexture& texture);

    ~Skybox();

    void render(Shader& shader, Camera& camera);
private:
    unsigned int VBO, VAO;
    CubeMapTexture skyboxTexture;
};
