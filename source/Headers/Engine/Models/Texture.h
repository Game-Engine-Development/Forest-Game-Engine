//
// Created by aekstrand7207 on 10/2/2019.
//

#ifndef OPENGL_GAME_TEXTURE_H
#define OPENGL_GAME_TEXTURE_H


class Texture {
public:
    Texture(const char* filename);
    ~Texture();

    unsigned int get_ID();
private:
    unsigned int ID;
};


#endif //OPENGL_GAME_TEXTURE_H
