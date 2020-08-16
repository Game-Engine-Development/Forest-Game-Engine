#include "Headers/Engine/Skybox/CubeMapTexture.h"

void CubeMapTexture::bind(const Shader &shader) const noexcept {
    glActiveTexture(GL_TEXTURE0 + texUnit);
    glBindTexture(GL_TEXTURE_CUBE_MAP, IDContainer->getID());
}

void CubeMapTexture::unBind() const noexcept {
    glActiveTexture(GL_TEXTURE0 + texUnit);
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}