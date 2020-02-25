#pragma once


#include <vector>
#include <Headers/Engine/Shader/Shader.h>

class CubeMapTexture {
public:
    CubeMapTexture();
    explicit CubeMapTexture(std::vector<const char*>& files, int textureUnit);

    ~CubeMapTexture();

    void bind(Shader& shader);

    void unBind();
private:
    unsigned int ID;
    int texUnit;
};
