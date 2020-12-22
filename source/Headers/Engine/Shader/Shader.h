#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <memory>

class Shader {
    // utility function for checking shader compilation/linking errors.
    // ------------------------------------------------------------------------
    static void checkCompileErrors(unsigned int shader, const std::string& type);

    friend void swap(Shader &shader1, Shader &shader2) {
        std::swap(shader1.ID, shader2.ID);
    }

public:
    std::shared_ptr<unsigned int> ID;

    Shader() = default;
    // constructor generates the shader on the fly
    // ------------------------------------------------------------------------
    Shader(const char* vertexPath, const char* fragmentPath);
    // activate the shader
    // ------------------------------------------------------------------------
    void use() const;

    // utility uniform functions
    // ------------------------------------------------------------------------
    void setBool(const std::string &name, bool value) const;
    // ------------------------------------------------------------------------
    void setInt(const std::string &name, int value) const;
    // ------------------------------------------------------------------------
    void setFloat(const std::string &name, float value) const;
    void setVec3(const std::string &name, glm::vec3 value) const;
    void setVec3(const std::string &name, float x, float y, float z) const;
    void setMat4(const std::string &name, glm::mat4 &mat) const;
};
