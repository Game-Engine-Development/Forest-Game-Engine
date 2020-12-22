#pragma once

#include <glad/glad.h>
#include <algorithm>

class MeshResourceContainer {
    friend void swap(MeshResourceContainer &meshContainer1, MeshResourceContainer &meshContainer2);

    unsigned int VAO{}, VBO{}, EBO{};

    std::size_t numOfVertices{}; //not RAII resource, is a normal integral value

public:
    MeshResourceContainer(const MeshResourceContainer&) = delete;
    MeshResourceContainer& operator=(const MeshResourceContainer&) = delete;

    MeshResourceContainer(MeshResourceContainer &&oldMesh) noexcept
    : VAO(oldMesh.VAO), VBO(oldMesh.VBO), EBO(oldMesh.EBO),

    numOfVertices(oldMesh.numOfVertices)
    {
        oldMesh.VAO = 0;
        oldMesh.VBO = 0;
        oldMesh.EBO = 0;

        oldMesh.numOfVertices = 0;
    }
    MeshResourceContainer& operator=(MeshResourceContainer &&oldMesh) noexcept {
        MeshResourceContainer move(std::move(oldMesh));
        swap(*this, move);
        return *this;
    }

    MeshResourceContainer() = delete;

    MeshResourceContainer(
            unsigned int VAOPar,
            unsigned int VBOPar,
            unsigned int EBOPar,

            std::size_t numOfVerticesPar
    ) noexcept;

    [[nodiscard]] unsigned int getVAO() const noexcept;

    [[nodiscard]] std::size_t getNumOfVertices() const noexcept;

    ~MeshResourceContainer() noexcept;
};
