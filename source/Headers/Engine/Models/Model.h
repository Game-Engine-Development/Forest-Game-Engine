#pragma once

#include <fstream>
#include <string>
#include <iostream>
#include <vector>
#include <glm/glm.hpp>

class Model {
public:
    Model(const char *filename);

    ~Model();

private:
    unsigned int VAO, VBO, EBO;
};
