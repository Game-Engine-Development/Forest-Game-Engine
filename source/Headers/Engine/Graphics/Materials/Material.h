#include "Headers/Engine/Models/Texture.h"

#pragma once

class MaterialParameterSet {
private:
    Texture albedo;
    Texture ao;
    Texture metallic;
    Texture normal;
    Texture roughness;
public:
    //takes the directory of a folder containing textures with correct names
    MaterialParameterSet(const char* directory, int type);
    MaterialParameterSet();

    Texture &getAlbedo();

    Texture &getAo();

    Texture &getMetallic();

    Texture &getNormal();

    Texture &getRoughness();

};

