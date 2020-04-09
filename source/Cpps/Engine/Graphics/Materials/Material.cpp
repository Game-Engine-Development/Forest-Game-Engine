
#include "Headers/Engine/Graphics/Materials/MaterialParameterSet.h"

MaterialParameterSet::MaterialParameterSet(const char* directory, int type) {
    std::string extension = type == Texture::PNG ? "png" : "jpg";
    this -> albedo = Texture(((std::string(directory) + std::string("/albedo."))+ extension).c_str() , type, 0, "albedo");
    this -> ao = Texture(((std::string(directory) + std::string("/ao."))+ extension).c_str() , type, 1, "ao");
    this -> metallic = Texture(((std::string(directory) + std::string("/metallic."))+ extension).c_str() , type, 2, "metallic");
    this -> normal = Texture(((std::string(directory) + std::string("/normal."))+ extension).c_str() , type, 3, "normal");
    this -> roughness = Texture(((std::string(directory) + std::string("/roughness."))+ extension).c_str() , type, 4, "roughness");
}

MaterialParameterSet::MaterialParameterSet() = default;

Texture &MaterialParameterSet::getAlbedo() {
    return albedo;
}

Texture &MaterialParameterSet::getAo() {
    return ao;
}

Texture &MaterialParameterSet::getMetallic() {
    return metallic;
}

Texture &MaterialParameterSet::getNormal() {
    return normal;
}

Texture &MaterialParameterSet::getRoughness() {
    return roughness;
}

