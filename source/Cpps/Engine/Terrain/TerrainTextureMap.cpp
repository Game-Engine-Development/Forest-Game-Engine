#include "Headers/Engine/Terrain/TerrainTextureMap.h"

TerrainTextureMap::TerrainTextureMap(const char *const map, const char *const texture1,
        const char *const texture2, const char *const texture3, const char *const texture4) noexcept
{
    textures.reserve(5);
    textures.emplace_back(map, 0);
    textures.emplace_back(texture1, 1);
    textures.emplace_back(texture2, 2);
    textures.emplace_back(texture3, 3);
    textures.emplace_back(texture4, 4);
}

void TerrainTextureMap::bind(const Shader &shader) {
    for(Component::TextureComponent &texture : textures) {
        texture.bind(shader);
    }
}

void TerrainTextureMap::unbind() {
    for(Component::TextureComponent &texture : textures) {
        texture.unbind();
    }
}
