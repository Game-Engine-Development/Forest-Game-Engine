#pragma once

#include <vector>
#include <iostream>
#include <string>
#include <optional>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags
#include <assimp/DefaultLogger.hpp>

#include "Headers/Engine/Utils/CommonDeclarations.h"
#include "Headers/Engine/Utils/MiscUtils.h"

namespace Component {
    struct Model;

    //struct MeshComponent;
    struct Mesh;

    struct TextureComponent;
}

unsigned int generateMesh();

std::vector<Component::TextureComponent> loadMaterialTextures(Component::Model &model, aiMaterial *mat, aiTextureType type, const std::string &typeNameconst, bool reset = true);

Component::Mesh processMesh(Component::Model &model, aiMesh *mesh, const aiScene *scene);

void processNode(Component::Model &model, aiNode *node, const aiScene *scene);

std::optional<Component::Model> loadModel(const std::string &path);
