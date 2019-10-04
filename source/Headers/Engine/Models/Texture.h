#pragma once

class Texture {
public:
    Texture(const char* filename);
    ~Texture();

    unsigned int get_ID();
private:
    unsigned int ID;
};

