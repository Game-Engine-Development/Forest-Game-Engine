#include "Headers/Engine/ResourceContainers/SkyboxResourceContainer.h"

SkyboxResourceContainer::SkyboxResourceContainer(const unsigned int VAOPar, const unsigned int VBOPar) noexcept
: VAO(VAOPar), VBO(VBOPar)
{}

[[nodiscard]] unsigned int SkyboxResourceContainer::getVAO() const noexcept {
    return VAO;
}

SkyboxResourceContainer::~SkyboxResourceContainer() noexcept {
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);
}
