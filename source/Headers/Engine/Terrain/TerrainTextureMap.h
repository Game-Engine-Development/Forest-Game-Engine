#pragma once
#include "Headers/Engine/Models/Texture.h"
#include <vector>
#include "Headers/Engine/Shader/Shader.h"

class TerrainTextureMap {
public:
    TerrainTextureMap();
    explicit TerrainTextureMap(const char* map, const char* texture1, const char* texture2, const char* texture3, const char* texture4);

    ~TerrainTextureMap();

    void bindTextures(Shader& shader);

    void unBindTextures();
private:
    std::vector<Texture> textures;
};