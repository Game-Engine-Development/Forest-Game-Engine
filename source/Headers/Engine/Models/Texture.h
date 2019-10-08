#pragma once

class Texture {
public:
    Texture();
    explicit Texture(const char* filename);
    ~Texture();
    void bind();
    void unbind();

    unsigned int get_ID();
private:
    unsigned int ID;
};

