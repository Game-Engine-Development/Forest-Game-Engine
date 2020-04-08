#include "Headers/Engine/Terrain/TerrainTextureMap.h"

TerrainTextureMap::TerrainTextureMap() = default;

TerrainTextureMap::TerrainTextureMap(const char *map, const char *texture1, const char *texture2, const char *texture3,
                                     const char *texture4) {
    textures.reserve(5);
    textures.emplace_back(map, 0);
    textures.emplace_back(texture1, 1);
    textures.emplace_back(texture2, 2);
    textures.emplace_back(texture3, 3);
    textures.emplace_back(texture4, 4);
}

TerrainTextureMap& TerrainTextureMap::operator=(TerrainTextureMap &&oldTerrainTextureMap) noexcept {
    textures = std::move(oldTerrainTextureMap.textures);
    oldTerrainTextureMap.textures.clear();

    return *this;
}

TerrainTextureMap::~TerrainTextureMap() = default;

void TerrainTextureMap::bindTextures(Shader &shader) {
    for(Texture &texture : textures) {
        texture.bind(shader);
    }
}

void TerrainTextureMap::unBindTextures() {
    for(Texture &texture : textures) {
        texture.unbind();
    }
}

TerrainTextureMap::TerrainTextureMap(TerrainTextureMap const &original) {
    textures = original.textures;
}
