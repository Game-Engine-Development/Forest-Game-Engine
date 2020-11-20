
#include "Headers/Engine/Graphics/Materials/Material.h"

std::string Material::typeToString(const ImageType type) { //@todo find a way to cache this value in the constructor to avoid extra heap allocations
    return (type == ImageType::RGBA_IMG) ? "bmp" : "jpg";
}

Material::Material(const char *const directory, const ImageType type)
: albedo(((std::string(directory) + std::string("/albedo.")) + typeToString(type)), 0, "albedo"),
ao(((std::string(directory) + std::string("/ao.")) + typeToString(type)) , 1, "ao"),
metallic(((std::string(directory) + std::string("/metallic.")) + typeToString(type)) , 2, "metallic"),
normal(((std::string(directory) + std::string("/normal.")) + typeToString(type)) , 3, "normal"),
roughness(((std::string(directory) + std::string("/roughness.")) + typeToString(type)) , 4, "roughness")
{}

Component::TextureComponent& Material::getAlbedo() {
    return albedo;
}

Component::TextureComponent& Material::getAo() {
    return ao;
}

Component::TextureComponent& Material::getMetallic() {
    return metallic;
}

Component::TextureComponent& Material::getNormal() {
    return normal;
}

Component::TextureComponent& Material::getRoughness() {
    return roughness;
}
