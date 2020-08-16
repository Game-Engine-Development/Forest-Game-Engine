#pragma once

#include <glad/glad.h>
#include <algorithm>

class QuadResourceContainer {
    friend void swap(QuadResourceContainer &quadContainer1, QuadResourceContainer &quadContainer2);

    unsigned int VAO{}, VBO{}, TBO{}, IBO{};

public:
    QuadResourceContainer(const QuadResourceContainer&) = delete;
    QuadResourceContainer& operator=(const QuadResourceContainer&) = delete;

    QuadResourceContainer(QuadResourceContainer &&oldQuad) noexcept
    : VAO(oldQuad.VAO), VBO(oldQuad.VBO), TBO(oldQuad.TBO), IBO(oldQuad.IBO)
    {
        oldQuad.VAO = 0;
        oldQuad.VBO = 0;
        oldQuad.TBO = 0;
        oldQuad.IBO = 0;
    }
    QuadResourceContainer& operator=(QuadResourceContainer &&oldQuad) noexcept {
        QuadResourceContainer move(std::move(oldQuad));
        swap(*this, move);
        return *this;
    }

    QuadResourceContainer() = default;

    QuadResourceContainer(unsigned int VAOPar, unsigned int VBOPar, unsigned int TBOPar, unsigned int IBOPar) noexcept;

    [[nodiscard]] unsigned int getVAO() const noexcept;

    ~QuadResourceContainer() noexcept;
};
