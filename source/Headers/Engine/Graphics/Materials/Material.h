#pragma once

#include "Headers/Engine/Texture/Texture.h"
#include "Headers/Engine/Constants.h"

class Material {
    Texture albedo;
    Texture ao;
    Texture metallic;
    Texture normal;
    Texture roughness;

    static std::string typeToString(ImageType type);
public:
    //takes the directory of a folder containing textures with correct names
    Material(const char* directory, ImageType type);

    //@todo fix escaping references
    [[nodiscard]] Texture& getAlbedo();
    [[nodiscard]] Texture& getAo();
    [[nodiscard]] Texture& getMetallic();
    [[nodiscard]] Texture& getNormal();
    [[nodiscard]] Texture& getRoughness();
};
