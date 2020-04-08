#pragma once

#include <regex>
#include <glad/glad.h>
#include <iostream>
#include <string>

class ImageFileReader {
public:
    ImageFileReader();
    explicit ImageFileReader(const char* filename);

    int getRawBufferWidth();
    int getRawBufferHeight();
    int getNR_Channels();
    unsigned char* getRawDataBuffer();
    unsigned int get_ID();

    ~ImageFileReader();

private:
    int width, height, nrchannels;
    unsigned char *data = nullptr;
    unsigned int ID;

    static constexpr int PNG = 0;
    static constexpr int JPG = 1;
};
