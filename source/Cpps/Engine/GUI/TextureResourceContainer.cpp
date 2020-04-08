#include "Headers/Engine/GUI/TextureResourceContainer.h"

TextureResourceContainer::TextureResourceContainer(unsigned int IDPar) {
    assert(!ID); //makes sure ID is 0

    ID = IDPar;
}

unsigned int TextureResourceContainer::getID() {
    return ID;
}

TextureResourceContainer::~TextureResourceContainer() {
    glDeleteTextures(1, &ID);
}
