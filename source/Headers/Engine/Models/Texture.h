#pragma once

#include <glad/glad.h>
#include <iostream>

class Texture {
public:
    Texture(const char* filename);
    ~Texture();

    unsigned int get_ID();
private:
    unsigned int ID;
};

