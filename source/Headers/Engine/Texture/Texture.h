#pragma once

#include <optional>
#include <utility>
#include <regex>
#include <algorithm>
#include <iostream>
#include <string>
#include <unordered_map>
#include <future>

#include <glad/glad.h>

#ifndef STB_IMAGE_IMPLEMENTATION
#include "../../../../libraries/stb_image.h"
#define STB_IMAGE_IMPLEMENTATION
#endif

#include "Headers/Engine/Shader/Shader.h"
#include "Headers/Engine/ResourceContainers/TextureResourceContainer.h"
#include "Headers/Engine/Constants.h"
#include "Headers/Engine/Utils/MiscUtils.h"

struct EntryType {
    int width{}, height{}, nrchannels{};
    unsigned char *data = nullptr;
    ImageType type{};
};

class Texture {
    bool isLoading = false;

    std::future<EntryType> thread;

    std::optional<EntryType> entry = std::nullopt;

    friend void swap(Texture &tex1, Texture &tex2);

    static inline std::unordered_map<std::string, std::pair<std::optional<TextureResourceContainer>, std::size_t>> textureCache;
    std::string textureCacheKey;

    unsigned int IDCache{}; //denormalized data from textureCache

    bool isLoaded = false;

    int textureUnit{};
    std::string shaderName;

    static EntryType loadFromDisk(const std::string textureCacheKey);
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
