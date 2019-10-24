#pragma once


#include <Headers/Engine/Camera.h>
#include "CubeMapTexture.h"

class Skybox {
public:
    Skybox(CubeMapTexture& texture);

    ~Skybox();

    void render(Shader& shader, Camera& camera);
private:
    unsigned int VBO, VAO;
    CubeMapTexture skyboxTexture;
};
