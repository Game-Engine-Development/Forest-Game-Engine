#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <iostream>

struct PointLight{
public:
    glm::vec3 position{};
    glm::vec3 color{};
    float intensity;

    PointLight(glm::vec3 position, glm::vec3 color, float intensity);

};

