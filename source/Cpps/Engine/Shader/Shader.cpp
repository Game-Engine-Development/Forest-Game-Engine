#include "Headers/Engine/Shader/Shader.h"

// constructor generates the shader on the fly
// ------------------------------------------------------------------------
Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
    // 1. retrieve the vertex/fragment source code from filePath
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    // ensure ifstream objects can throw exceptions:
    vShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        // open files
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        std::stringstream vShaderStream, fShaderStream;
        // read file's buffer contents into streams
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        // close file handlers
        vShaderFile.close();
        fShaderFile.close();
        // convert stream into string
        vertexCode   = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    }
    catch (const std::ifstream::failure& e)
    {
        std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
    }
    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();
    // 2. compile shaders
    unsigned int vertex, fragment;
    // vertex shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, nullptr);
    glCompileShader(vertex);
    checkCompileErrors(vertex, "VERTEX");
    // fragment Shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, nullptr);
    glCompileShader(fragment);
    checkCompileErrors(fragment, "FRAGMENT");
    // shader Program
    Shader::ID = glCreateProgram();
    glAttachShader(Shader::ID, vertex);
    glAttachShader(Shader::ID, fragment);
    glLinkProgram(Shader::ID);
    checkCompileErrors(Shader::ID, "PROGRAM");
    // delete the shaders as they're linked into our program now and no longer necessary
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}
// activate the shader
// ------------------------------------------------------------------------

Shader::Shader() = default;

void Shader::use() const
{
    glUseProgram(Shader::ID);
}
// utility uniform functions
// ------------------------------------------------------------------------
void Shader::setBool(const std::string &name, bool value) const
{
    glUniform1i(glGetUniformLocation(Shader::ID, name.c_str()), (int)value);
}
// ------------------------------------------------------------------------
void Shader::setInt(const std::string &name, int value) const
{
    glUniform1i(glGetUniformLocation(Shader::ID, name.c_str()), value);
}
// ------------------------------------------------------------------------
void Shader::setFloat(const std::string &name, float value) const
{
    glUniform1f(glGetUniformLocation(Shader::ID, name.c_str()), value);
}
void Shader::setVec3(const std::string &name, glm::vec3 value) const {
    glUniform3f(glGetUniformLocation(Shader::ID, name.c_str()), value.x, value.y, value.z);
}
void Shader::setVec3(const std::string &name, float x, float y, float z) const {
    glUniform3f(glGetUniformLocation(Shader::ID, name.c_str()), x, y, z);
}

void Shader::setMat4(const std::string &name, glm::mat4 &mat) const {
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);;
}

// utility function for checking shader compilation/linking errors.
// ------------------------------------------------------------------------
void Shader::checkCompileErrors(unsigned int shader, const std::string& type) {
    int success;
    char infoLog[1024];
    if (type != "PROGRAM") {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 1024, nullptr, infoLog);
            std::cerr << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
    else {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shader, 1024, nullptr, infoLog);
            std::cerr << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
}
