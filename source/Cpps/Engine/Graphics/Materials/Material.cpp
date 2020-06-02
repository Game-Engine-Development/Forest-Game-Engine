
#include "Headers/Engine/Graphics/Materials/Material.h"

Material::Material(const char* directory, int type) {
    std::string extension = type == Texture::PNG ? "png" : "jpg";
    this -> albedo = Texture(((std::string(directory) + std::string("/albedo."))+ extension).c_str(), 0, "albedoMap");
    this -> ao = Texture(((std::string(directory) + std::string("/ao."))+ extension).c_str() , 1, "aoMap");
    this -> metallic = Texture(((std::string(directory) + std::string("/metallic."))+ extension).c_str() , 2, "metallicMap");
    this -> normal = Texture(((std::string(directory) + std::string("/normal."))+ extension).c_str() , 3, "normalMap");
    this -> roughness = Texture(((std::string(directory) + std::string("/roughness."))+ extension).c_str() , 4, "roughnessMap");
}

Material::Material() = default;

Texture &Material::getAlbedo() {
    return albedo;
}

Texture &Material::getAo() {
    return ao;
}

Texture &Material::getMetallic() {
    return metallic;
}

Texture &Material::getNormal() {
    return normal;
}

Texture &Material::getRoughness() {
    return roughness;
}

