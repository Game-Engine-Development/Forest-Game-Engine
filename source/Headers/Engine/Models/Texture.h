//
// Created by aekstrand7207 on 10/2/2019.
//

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


