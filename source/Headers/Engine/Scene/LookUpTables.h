#pragma once

#include <string>

//remove:
#include <unordered_map>

#include "Headers/Engine/ResourceContainers/MeshResourceContainer.h"
#include "Headers/Engine/ResourceContainers/TextureResourceContainer.h"

#include "Headers/Engine/Shader/Shader.h"

//@todo use contexts instead
namespace LookupTables {
    [[deprecated]]
    inline std::unordered_map<std::string, std::pair<MeshResourceContainer, std::size_t>> MeshCache;

    [[deprecated]]
    inline std::unordered_map<std::string, std::pair<TextureResourceContainer, std::size_t>> TextureCache;

    [[deprecated]]
    inline std::unordered_map<std::string, Shader, std::size_t> ShaderCache;
}
