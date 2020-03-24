#include "Headers/Engine/Skybox/CubeMapTexture.h"

CubeMapTexture::CubeMapTexture() = default;

CubeMapTexture::~CubeMapTexture() = default;

void CubeMapTexture::bind(Shader &shader) {
    glActiveTexture(GL_TEXTURE0 + texUnit);
    glBindTexture(GL_TEXTURE_CUBE_MAP, ID);
}

void CubeMapTexture::unBind() {
    glActiveTexture(GL_TEXTURE0 + texUnit);
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}