#pragma once

#include <string>
#include <iostream>
#include <utility>
#include <algorithm>

//remove:
#include <regex>

#include <glad/glad.h>

#ifndef STB_IMAGE_IMPLEMENTATION
#include "../../../../libraries/stb_image.h"
#define STB_IMAGE_IMPLEMENTATION
#endif

#include "Headers/Engine/Utils/CommonDeclarations.h"

struct FileInfo {
    int width{}, height{}, nrchannels{};
    unsigned char *data = nullptr;
    ImageType type{};
};

FileInfo loadFromDisk(const std::string &textureCacheKey);
unsigned int loadOnMain(const FileInfo &entry, int textureUnit);