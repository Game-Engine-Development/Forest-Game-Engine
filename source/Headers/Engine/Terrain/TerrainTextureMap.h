#pragma once

#include <vector>
#include "Headers/Engine/Texture/Texture.h"
#include "Headers/Engine/Shader/Shader.h"

class TerrainTextureMap {
public:
    TerrainTextureMap() noexcept = default;
    explicit TerrainTextureMap(const char* map, const char* texture1, const char* texture2, const char* texture3, const char* texture4) noexcept;

    void bind(const Shader& shader);

    void unbind();
private:
    std::vector<Texture> textures;
};