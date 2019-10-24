#pragma once


#include <vector>
#include <Headers/stb_image.h>
#include <Headers/Engine/Shader.h>

class CubeMapTexture {
public:
    CubeMapTexture();
    explicit CubeMapTexture(std::vector<const char*> files);

    ~CubeMapTexture();

    void bind(Shader& shader);

    void unBind();
private:
    unsigned int ID;
};
