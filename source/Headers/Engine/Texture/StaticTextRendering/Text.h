#pragma once

#include <regex>
#include <iostream>
#include <fstream>
#include <tuple>
#include <array>
#include <string>
#include <unordered_map>
#include <optional>
#include <memory>
#include <vector>
#include <cstring> //memcpy & memset

#include <glad/glad.h>

#ifndef STB_IMAGE_IMPLEMENTATION
#include "../../../../../libraries/stb_image.h"
#define STB_IMAGE_IMPLEMENTATION
#endif

#include "Headers/Engine/Utils/RefCountedHashmap.h"
#include "Headers/Engine/ResourceContainers/stb_PointerContainer.h"
#include "Headers/Engine/ResourceContainers/TextureResourceContainer.h"
#include "Headers/Engine/Utils/MiscUtils.h"
#include "Headers/Engine/Shader/Shader.h"
#include "Headers/Engine/Utils/CommonDeclarations.h"

struct Character {
    explicit Character(const std::array<int, 8> &numbersInLine)
        : id(static_cast<char>(numbersInLine[0])),
        x(numbersInLine[1]),
        y(numbersInLine[2]),
        width(numbersInLine[3]),
        height(numbersInLine[4]),
        xOffset(numbersInLine[5]),
        yOffset(numbersInLine[6]),
        xAdvance(numbersInLine[7])
    {} //@todo try to make this less verbose

    char id;

    int x;
    int y;
    int width;
    int height;
    int xOffset;
    int yOffset;
    int xAdvance;
};

class FontFileReader {
    static constexpr unsigned int numInvisibleChars = 32;
    //256 = number of characters in extended ascii
    using Value = std::array<std::optional<Character>, 256-numInvisibleChars>;
    Value *currentFontFile = nullptr;
    RefCountedHashmap<Filename, Value> fontFileCache;

public:
    explicit FontFileReader(const std::string &filename) : fontFileCache(filename) {
        currentFontFile = &fontFileCache.getElement(filename);

        std::ifstream file(filename); //automatically closes file in std::ifstream's destructor

        std::array<int, 8> numbersForChar{};

        std::string line{};

        for(int i = 0; i < 3; ++i) {
            getline(file, line, '\n');
        }

        while(getline(file, line, '\n')) {
            int numbersForCharIndex{};
            std::string numberAsString{};
            for(int i = 0; i < (line.size()-1) && numbersForCharIndex < numbersForChar.size(); ++i) {
                if(isIntegral(line[i])) {
                    numberAsString += line[i];

                    if(!isIntegral(line[i+1])) {
                        numbersForChar[numbersForCharIndex] = std::stoi(numberAsString);

                        numberAsString.clear();
                        ++numbersForCharIndex;
                    }
                }
            }
            currentFontFile->at(numbersForChar[0] - numInvisibleChars) = Character(numbersForChar);
        }
    }

    Character getChar(const char id) {
        return currentFontFile->at(id-numInvisibleChars).value();
    }
};

class ImageFileReader {
    static ImageType getType(const std::string &filenamePar) {
        if (std::regex_match(filenamePar, std::regex("(.+)(\\.png)"))) {
            return ImageType::RGBA_IMG;
        }
        else if (std::regex_match(filenamePar, std::regex("(.+)(\\.jpg)"))) {
            return ImageType::RGB_IMG;
        }
        else {
            std::cerr << "File type not supported!. Please supply a JPG or PNG!\n";
            return ImageType::RGBA_IMG; //return to avoid UB
        }
    }

    using refCount = std::size_t;
    static inline std::unordered_map<std::string, std::pair<stb_PointerContainer, refCount>> imageCache;
    int widthCache{}, heightCache{}, nrchannelsCache{};
    const unsigned char *dataCache = nullptr;

    std::string filename;

public:
    explicit ImageFileReader(const std::string &filenamePar) : filename(std::string(filenamePar)) {
        if(imageCache.count(filename)) {
            imageCache.at(filename).second += 1;
            widthCache = imageCache.at(filename).first.getWidth();
            heightCache = imageCache.at(filename).first.getHeight();
            nrchannelsCache = imageCache.at(filename).first.getChannels();
            dataCache = imageCache.at(filename).first.getData();
        }
        else {
            const ImageType type = getType(filenamePar);
            const auto desiredChannels = (type == ImageType::RGBA_IMG) ? STBI_rgb_alpha : STBI_rgb;
            int width{}, height{}, nrchannels{};
            unsigned char *data = stbi_load(filenamePar.c_str(), &width, &height, &nrchannels, desiredChannels);
            imageCache.emplace(std::string(filename), std::make_pair(stb_PointerContainer(data, width, height, nrchannels), 1));
            widthCache = width;
            heightCache = height;
            nrchannelsCache = nrchannels;
            dataCache = data;
        }
    }

    ImageFileReader(const ImageFileReader&) = default;
    ImageFileReader(ImageFileReader&&) = default;

    ImageFileReader& operator=(const ImageFileReader&) = default;
    ImageFileReader& operator=(ImageFileReader&&) = default;

    [[nodiscard]] int getRawBufferWidth() const {
        return widthCache;
    }
    [[nodiscard]] int getNR_Channels() const {
        return nrchannelsCache;
    }
    [[nodiscard]] const unsigned char* getRawDataBuffer() const {
        return dataCache;
    }

    ~ImageFileReader() {
        if(imageCache.count(filename)) {
            imageCache.at(filename).second -= 1;

            //if the refcount drops to 0, delete the element
            if(!imageCache.at(filename).second) {
                imageCache.erase(filename);
            }
        }
    }
};

class Text {
    std::vector<Character> text;

    FontFileReader fontFile;
    ImageFileReader imageFile;

    std::unique_ptr<TextureResourceContainer> IDContainer;
    static constexpr int textureUnit = 0; //@todo temporary, will change later

public:
    Text(const Text &) = delete;
    Text& operator=(const Text &) = delete;

    Text(Text &&) = default;
    Text& operator=(Text &&) = default;

    Text(const std::string &fontFilename, const std::string &imageFilename, const std::string &textString);

    void bind(const Shader &shader) const noexcept;
    static void unbind() noexcept;

    [[nodiscard]] unsigned int get_ID() const;
};
