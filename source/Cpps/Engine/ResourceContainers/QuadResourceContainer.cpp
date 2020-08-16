#include "Headers/Engine/ResourceContainers/QuadResourceContainer.h"

void swap(QuadResourceContainer &quadContainer1, QuadResourceContainer &quadContainer2) {
    std::swap(quadContainer1.VAO, quadContainer2.VAO);
    std::swap(quadContainer1.VBO, quadContainer2.VBO);
    std::swap(quadContainer1.TBO, quadContainer2.TBO);
    std::swap(quadContainer1.IBO, quadContainer2.IBO);
}

QuadResourceContainer::QuadResourceContainer(const unsigned int VAOPar, const unsigned int VBOPar, const unsigned int TBOPar, const unsigned int IBOPar) noexcept
: VAO(VAOPar), VBO(VBOPar), TBO(TBOPar), IBO(IBOPar)
{}

[[nodiscard]] unsigned int QuadResourceContainer::getVAO() const noexcept {
    return VAO;
}

QuadResourceContainer::~QuadResourceContainer() noexcept {
    glDeleteBuffers(1, &IBO);
    glDeleteBuffers(1, &TBO);
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);
}