#pragma once

class Model {
public:
    Model(const char *filename);

    ~Model();

private:
    unsigned int VAO, VBO, EBO;
};

