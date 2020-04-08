#pragma once

#include <iostream>
#include <cassert>
#include <glad/glad.h>

class TextureResourceContainer {
private:
    unsigned int ID = 0;

public:
    explicit TextureResourceContainer(unsigned int IDPar);

    unsigned int getID();

    ~TextureResourceContainer();
};
