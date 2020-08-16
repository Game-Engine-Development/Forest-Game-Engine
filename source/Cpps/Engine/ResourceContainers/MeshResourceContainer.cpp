#include "Headers/Engine/ResourceContainers/MeshResourceContainer.h"

void swap(MeshResourceContainer &meshContainer1, MeshResourceContainer &meshContainer2) {
    std::swap(meshContainer1.VAO, meshContainer2.VAO);
    std::swap(meshContainer1.VBO, meshContainer2.VBO);
    std::swap(meshContainer1.texCoordBuffer, meshContainer2.texCoordBuffer);
    std::swap(meshContainer1.normalBuffer, meshContainer2.normalBuffer);
    std::swap(meshContainer1.tangentBuffer, meshContainer2.tangentBuffer);
    std::swap(meshContainer1.bitangentBuffer, meshContainer2.bitangentBuffer);
}

MeshResourceContainer::MeshResourceContainer(
        const unsigned int VAOPar,
        const unsigned int VBOPar,
        const unsigned int texCoordBufferPar,
        const unsigned int normalBufferPar,
        const unsigned int tangentBufferPar,
        const unsigned int bitangentBufferPar,

        const std::size_t numOfVerticesPar) noexcept
: VAO(VAOPar), VBO(VBOPar), texCoordBuffer(texCoordBufferPar),
normalBuffer(normalBufferPar), tangentBuffer(tangentBufferPar),
bitangentBuffer(bitangentBufferPar),

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
    glDeleteBuffers(1, &texCoordBuffer);
    glDeleteBuffers(1, &normalBuffer);
    glDeleteBuffers(1, &tangentBuffer);
    glDeleteBuffers(1, &bitangentBuffer);
    glDeleteVertexArrays(1, &VAO);
}


