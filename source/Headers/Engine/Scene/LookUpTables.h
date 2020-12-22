#pragma once

#include <string>

//remove:
#include <unordered_map> //@todo replace with abseil swiss tables

#include "Headers/Engine/ResourceContainers/MeshResourceContainer.h"
#include "Headers/Engine/ResourceContainers/TextureResourceContainer.h"

#include "Headers/Engine/Shader/Shader.h"


//@todo use contexts instead
namespace LookupTables {
    [[deprecated]]
    inline std::unordered_map<std::string, std::pair<TextureResourceContainer, std::size_t>> TextureCache;

    //[[deprecated]]
    //inline std::unordered_map<std::string, std::pair<ShaderResourceContainer, std::size_t>> ShaderCache;
}
