#pragma once

#include <glad/glad.h>
#include <algorithm>
#include <iostream>
#include <cassert>

class TextureResourceContainer {
    friend void swap(TextureResourceContainer &texContainer1, TextureResourceContainer &texContainer2);

    unsigned int ID{};

public: //only movable (for putting into containers), no copies allowed
    TextureResourceContainer(const TextureResourceContainer&) = delete;
    TextureResourceContainer& operator=(const TextureResourceContainer&) = delete;

    TextureResourceContainer(TextureResourceContainer &&oldTexture) noexcept
    : ID(oldTexture.ID)
    {
        oldTexture.ID = 0;
    }
    TextureResourceContainer& operator=(TextureResourceContainer &&oldTexture) noexcept {
        TextureResourceContainer move(std::move(oldTexture));
        swap(*this, move);
        return *this;
    }

    TextureResourceContainer() = delete;

    explicit TextureResourceContainer(unsigned int IDPar) noexcept;

    [[nodiscard]] unsigned int getID() const noexcept;

    ~TextureResourceContainer() noexcept;
};
