#include "Headers/Engine/Terrain/TerrainTextureMap.h"

TerrainTextureMap::TerrainTextureMap() = default;

TerrainTextureMap::TerrainTextureMap(const char *map, const char *texture1, const char *texture2, const char *texture3,
                                     const char *texture4) {
    textures.emplace_back(Texture(map, 0));
    textures.emplace_back(Texture(texture1, 1));
    textures.emplace_back(Texture(texture2, 2));
    textures.emplace_back(Texture(texture3, 3));
    textures.emplace_back(Texture(texture4, 4));
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