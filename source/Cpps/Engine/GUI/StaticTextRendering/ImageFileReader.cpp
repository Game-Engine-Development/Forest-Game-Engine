#include <Headers/Engine/Models/stb_image.h>

#include "Headers/Engine/GUI/StaticTextRendering/ImageFileReader.h"

ImageFileReader::ImageFileReader() = default;

ImageFileReader::ImageFileReader(const char *filename) {
    int type = 0;
    if (std::regex_match(filename, std::regex("(.+)(\\.png)"))) {
        type = PNG;
    }
    else if (std::regex_match(filename, std::regex("(.+)(\\.jpg)"))) {
        type = JPG;
    }
    else {
        std::cerr << "File type not supported!. Please supply a JPG or PNG!" << std::endl;
    }

    if (type == PNG) {
        data = stbi_load(filename, &width, &height, &nrchannels, STBI_rgb_alpha);
    } else {
        data = stbi_load(filename, &width, &height, &nrchannels, STBI_rgb);
    }
}

int ImageFileReader::getRawBufferWidth() {
    return width;
}

int ImageFileReader::getRawBufferHeight() {
    return height;
}

int ImageFileReader::getNR_Channels() {
    return nrchannels;
}

unsigned char* ImageFileReader::getRawDataBuffer() {
    return data;
}

unsigned int ImageFileReader::get_ID() {
    return ID;
}

ImageFileReader::~ImageFileReader() {
    stbi_image_free(data); //deletes internal heap buffer to stop memory leaking
}
