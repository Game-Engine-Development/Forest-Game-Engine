#pragma once

#include <vector>
#include <optional>

#include <glm/glm.hpp>

#include "Headers/Engine/Models/Mesh.h"
#include "Headers/Engine/Texture/Texture.h"
#include "Headers/Engine/Scene/ENTTWrapper.h"

namespace Component {
    struct Drawable {
        Mesh mesh;
        std::vector<Texture> textures;
        Transform modelMatrix;
    };
}
