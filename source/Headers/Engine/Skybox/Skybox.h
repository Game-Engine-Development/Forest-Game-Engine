#pragma once

#include <memory>
#include <array>

#include "Headers/Engine/ResourceContainers/SkyboxResourceContainer.h"
#include "Headers/Engine/Camera/Camera.h"
#include "Headers/Engine/Skybox/CubeMapTexture.h"

class Skybox { //@todo find out if moving should be allowed or not
private:
    std::unique_ptr<SkyboxResourceContainer> skyboxBufferContainer;
    CubeMapTexture skyboxTexture;

public:
    Skybox(const Skybox&) = delete;
    Skybox& operator=(const Skybox&) = delete;

    explicit Skybox(const CubeMapTexture& texture);

    void render(Shader& shader, Camera& camera);
};
