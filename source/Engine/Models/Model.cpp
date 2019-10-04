#include <fstream>
#include <string>
#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include "Headers/Engine/Models/Model.h"

Model::Model(const char *filename) {
    std::ifstream file;
    file.open(filename);
    if(file.is_open()) {
        std::string line;
        std::vector<glm::vec3> vertices;
        std::vector<glm::vec2> textureCoords;
        std::vector<glm::vec3> normals;
        std::vector<int> indices;
        while (std::getline(file, line)){
            
            if(line.rfind("v ", 0) == 0) {
                vertices.push_back(glm::vec3());
            } else if(line.rfind("vt ", 0) == 0) {

            } else if(line.rfind("vn ", 0) == 0) {

            } else if(line.rfind("f ", 0) == 0) {

            }
        }
    } else {
        std::cout << "Failed to open " << filename << " model file!" << std::endl;
    }
}