#pragma once

#include <glm/glm.hpp>
#include <glad/glad.h>
#include "Headers/Engine/Shader.h"
#include <Headers/Engine/IBL/HDR/HDRI.h>
#include <glm/gtc/matrix_transform.hpp>





class IBL {
public:
    IBL(HDRI hdr, Shader equirectangularToCubemapShader, Shader irradianceShader, Shader prefilterShader, Shader brdfShader, Shader backgroundShader);
    void bindMaps();
    void setProjection(glm::mat4 projection);
    void renderBackground(glm::mat4 view);


private:
    void renderQuad();
    void renderCube();

    unsigned int captureFBO;
    unsigned int captureRBO;

    unsigned int envCubemap;
    unsigned int irradianceMap;
    unsigned int prefilterMap;
    unsigned int brdfLUTTexture;

    unsigned int maxMipLevels = 5;

    unsigned int cubeVAO = 0;
    unsigned int cubeVBO = 0;

    unsigned int quadVAO = 0;
    unsigned int quadVBO;

    Shader backgroundShader;

};


