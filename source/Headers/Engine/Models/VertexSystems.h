#pragma once

#include <vector>
#include <iostream>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include "Headers/Engine/Utils/MiscUtils.h"

void CalculateTangentsAndBitangents(
        std::vector<glm::vec3> &vertices,
        std::vector<glm::vec2> &uvs,
        std::vector<glm::vec3> &normals,
        std::vector<glm::vec3> &tangents,
        std::vector<glm::vec3> &bitangents
);

void updateCube(glm::vec3 &vertex, glm::vec3 greatest = glm::vec3{}, glm::vec3 smallest = glm::vec3{});

[[deprecated]]
void loadOBJ(
        const char *filename,
        std::vector<glm::vec3> &vertices,
        std::vector<glm::vec2> &textureCoords,
        std::vector<glm::vec3> &normals
);
