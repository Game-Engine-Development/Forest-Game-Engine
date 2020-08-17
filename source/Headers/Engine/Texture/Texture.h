#pragma once

#include <optional>
#include <utility>
#include <regex>
#include <algorithm>
#include <iostream>
#include <string>
#include <unordered_map>
#include <mutex>
#include <future>

#include <glad/glad.h>

#ifndef STB_IMAGE_IMPLEMENTATION
#include "../../../../libraries/stb_image.h"
#define STB_IMAGE_IMPLEMENTATION
#endif

#include "Headers/Engine/Shader/Shader.h"
#include "Headers/Engine/ResourceContainers/TextureResourceContainer.h"
#include "Headers/Engine/Constants.h"

class Texture {
    struct EntryType {
        int width{}, height{}, nrchannels{};
        unsigned char *data = nullptr;
        ImageType type{};
    };

    std::future<void> thread;

    friend void swap(Texture &tex1, Texture &tex2);

    static inline std::unordered_map<std::string, std::pair<std::optional<TextureResourceContainer>, std::size_t>> textureCache;
    std::string textureCacheKey;

    unsigned int IDCache{}; //denormalized data from textureCache

    bool isLoaded = false;

    std::mutex entryMutex;
    std::optional<EntryType> entry = std::nullopt;

    int textureUnit{};
    std::string shaderName;

    static void loadFromDisk(std::optional<EntryType> *entry, const std::string *textureCacheKey, std::mutex *entryMutex);
    void loadOnMain();

    void pollIsLoaded();
    void addNullEntry();

public:
    explicit Texture(const std::string& filename, int unit, const std::optional<std::string>& nameInShader = std::nullopt);

    Texture(const Texture &tex);
    Texture(Texture &&oldTexture) noexcept;

    Texture& operator=(const Texture &tex);
    Texture& operator=(Texture &&oldTexture) noexcept;

    void bind(const Shader& shader);
    void unbind();

    ~Texture();
};
