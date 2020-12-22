#include "Headers/Engine/ResourceContainers/MeshResourceContainer.h"

void swap(MeshResourceContainer &meshContainer1, MeshResourceContainer &meshContainer2) {
    std::swap(meshContainer1.VAO, meshContainer2.VAO);
    std::swap(meshContainer1.VBO, meshContainer2.VBO);
    std::swap(meshContainer1.EBO, meshContainer2.EBO);

    std::swap(meshContainer1.numOfVertices, meshContainer2.numOfVertices);
}

MeshResourceContainer::MeshResourceContainer(
        const unsigned int VAOPar,
        const unsigned int VBOPar,
        const unsigned int EBOPar,

        const std::size_t numOfVerticesPar) noexcept
: VAO(VAOPar), VBO(VBOPar), EBO(EBOPar),

numOfVertices(numOfVerticesPar)
{}

[[nodiscard]] unsigned int MeshResourceContainer::getVAO() const noexcept {
    return VAO;
}
[[nodiscard]] std::size_t MeshResourceContainer::getNumOfVertices() const noexcept {
    return numOfVertices;
}


MeshResourceContainer::~MeshResourceContainer() noexcept {
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteVertexArrays(1, &VAO);
}


