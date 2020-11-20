#pragma once

#include <string>

#include <unordered_map> //@deprecated

#include "Headers/Engine/ResourceContainers/MeshResourceContainer.h"

namespace LookupTables {
    //@deprecated
    inline std::unordered_map<std::string, std::pair<MeshResourceContainer, std::size_t>> MeshCache;
}
