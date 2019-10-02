//
// Created by aekstrand7207 on 10/2/2019.
//

#ifndef OPENGL_GAME_MODEL_H
#define OPENGL_GAME_MODEL_H


class Model {
public:
    Model(std::string filename);

    ~Model();

private:
    unsigned int VAO, VBO, EBO;
};


#endif //OPENGL_GAME_MODEL_H
