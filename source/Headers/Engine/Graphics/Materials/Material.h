#pragma once

#include "Headers/Engine/Scene/Components.h"
#include "Headers/Engine/Utils/CommonDeclarations.h"

class Material {
    Component::TextureComponent albedo;
    Component::TextureComponent ao;
    Component::TextureComponent metallic;
    Component::TextureComponent normal;
    Component::TextureComponent roughness;

    static std::string typeToString(ImageType type);
public:
    //takes the directory of a folder containing textures with correct names
    Material(const char* directory, ImageType type);

    //@todo fix escaping references
    [[nodiscard]] Component::TextureComponent& getAlbedo();
    [[nodiscard]] Component::TextureComponent& getAo();
    [[nodiscard]] Component::TextureComponent& getMetallic();
    [[nodiscard]] Component::TextureComponent& getNormal();
    [[nodiscard]] Component::TextureComponent& getRoughness();
};
