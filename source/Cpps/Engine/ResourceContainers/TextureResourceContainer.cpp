#include "Headers/Engine/ResourceContainers/TextureResourceContainer.h"

void swap(TextureResourceContainer &texContainer1, TextureResourceContainer &texContainer2) {
    std::swap(texContainer1.ID, texContainer2.ID);
}

TextureResourceContainer::TextureResourceContainer(const unsigned int IDPar) noexcept : ID(IDPar)
{
    //assert(!ID);
}

[[nodiscard]] unsigned int TextureResourceContainer::getID() const noexcept {
    return ID;
}

TextureResourceContainer::~TextureResourceContainer() noexcept {
    glDeleteTextures(1, &ID);
}
