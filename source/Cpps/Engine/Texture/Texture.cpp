#include "Headers/Engine/Texture/Texture.h"

void Texture::func(std::optional<EntryType> *entry, const std::string *const textureCacheKey, std::mutex *entryMutex) {
    std::cout << "thread func\n";
}

void swap(Texture &tex1, Texture &tex2) {
    std::swap(tex1.textureCacheKey, tex2.textureCacheKey);
    std::swap(tex1.IDCache, tex2.IDCache);

    std::swap(tex1.textureUnit, tex2.textureUnit);
    std::swap(tex1.shaderName, tex2.shaderName);
}

Texture::Texture(const std::string& filename, const int unit, const std::optional<std::string>& nameInShader)
: textureCacheKey(filename), textureUnit(unit)
{
    shaderName = nameInShader.value_or(std::string("texture") + std::to_string(unit));

    if(textureCache.count(filename)) {
        textureCache.at(filename).second += 1;

        pollIsLoaded();
    }
    else {
        addNullEntry();

        //std::cout << "preLoad: " << textureCacheKey << std::endl;
        thread = std::async(std::launch::async, loadFromDisk, &entry, &textureCacheKey, &entryMutex);
        //thread = std::async(std::launch::async, func, &entry, &textureCacheKey, &entryMutex);
        //loadFromDisk(&entry, &textureCacheKey, &entryMutex);

        /*int width{}, height{}, nrchannels{};
        unsigned char *data = nullptr;

        ImageType type{};
        if (std::regex_match(filename, std::regex("(.+)(\\.png)")) ||
            std::regex_match(filename, std::regex("(.+)(\\.bmp)")))
        {
            type = ImageType::RGBA_IMG;
            data = stbi_load(filename.c_str(), &width, &height, &nrchannels, STBI_rgb_alpha);
        } else if (std::regex_match(filename, std::regex("(.+)(\\.jpg)")) ||
            std::regex_match(filename, std::regex("(.+)(\\.jpeg)")))
        {
            type = ImageType::RGB_IMG;
            data = stbi_load(filename.c_str(), &width, &height, &nrchannels, STBI_rgb);
        } else {
            std::cerr << "File type not supported!. Please supply a JPG or PNG!" << std::endl;
        }

        unsigned int ID{};

        glGenTextures(1, &ID);
        std::cout << "ID in Texture contructor: " << ID << '\n';

        glActiveTexture(GL_TEXTURE0 + textureUnit);
        glBindTexture(GL_TEXTURE_2D, ID);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        if (data) {
            if (type == ImageType::RGB_IMG) {
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            } else {
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            }
            glGenerateMipmap(GL_TEXTURE_2D);
        } else {
            std::cerr << "Failed to load " << filename << " texture!" << std::endl;
        }
        stbi_image_free(data);
        glBindTexture(GL_TEXTURE_2D, 0);

        IDCache = ID; //denormalizes data
        //std::cout << "ID in Texture contructor: " << ID << '\n';
        textureCache.at(textureCacheKey).first = TextureResourceContainer(ID);*/
    }
}

void Texture::addNullEntry() {
    textureCache.emplace(std::string(textureCacheKey), std::make_pair<std::optional<TextureResourceContainer>, std::size_t>(std::nullopt, 1));
}

void Texture::loadFromDisk(std::optional<EntryType> *entry, const std::string *const textureCacheKey, std::mutex *entryMutex) {
    std::cout << "loadFromDisk: " << (*textureCacheKey) << std::endl;

    int width{}, height{}, nrchannels{};
    unsigned char *data = nullptr;

    ImageType type{};
    if (std::regex_match((*textureCacheKey), std::regex("(.+)(\\.png)")) ||
        std::regex_match((*textureCacheKey), std::regex("(.+)(\\.bmp)")))
    {
        type = ImageType::RGBA_IMG;
        data = stbi_load(textureCacheKey->c_str(), &width, &height, &nrchannels, STBI_rgb_alpha);
    } else if (std::regex_match((*textureCacheKey), std::regex("(.+)(\\.jpg)")) ||
               std::regex_match((*textureCacheKey), std::regex("(.+)(\\.jpeg)")))
    {
        type = ImageType::RGB_IMG;
        data = stbi_load(textureCacheKey->c_str(), &width, &height, &nrchannels, STBI_rgb);
    } else {
        std::cerr << "File type not supported!. Please supply a JPG or PNG!" << std::endl;
        std::cerr << (*textureCacheKey) << std::endl;
    }

    EntryType entryTmp {
        width, height, nrchannels, data, type
    };

    std::lock_guard<std::mutex> lock(*entryMutex);
    *entry = entryTmp;
}

void Texture::loadOnMain() {
    std::cout << "loadOnMain: " << textureCacheKey << std::endl;

    unsigned int ID{};

    glGenTextures(1, &ID);
    std::cout << "ID in Texture contructor: " << ID << '\n';

    glActiveTexture(GL_TEXTURE0 + textureUnit);
    glBindTexture(GL_TEXTURE_2D, ID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    if (entry->data) {
        if (entry->type == ImageType::RGB_IMG) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, entry->width, entry->height, 0, GL_RGB, GL_UNSIGNED_BYTE, entry->data);
        } else {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, entry->width, entry->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, entry->data);
        }
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cerr << "Failed to load " << textureCacheKey << " texture!" << std::endl;
    }
    stbi_image_free(entry->data);
    glBindTexture(GL_TEXTURE_2D, 0);

    IDCache = ID; //denormalizes data
    textureCache.at(textureCacheKey).first = TextureResourceContainer(ID);
}

Texture::Texture(const Texture &tex)
: textureCacheKey(tex.textureCacheKey), IDCache(tex.IDCache),
  textureUnit(tex.textureUnit), shaderName(tex.shaderName)
{
    textureCache[textureCacheKey].second += 1;
}
Texture::Texture(Texture &&oldTexture) noexcept
: textureCacheKey(std::move(oldTexture.textureCacheKey)), IDCache(oldTexture.IDCache),
  textureUnit(oldTexture.textureUnit), shaderName(std::move(oldTexture.shaderName))
{
    oldTexture.textureCacheKey.clear();
    oldTexture.IDCache = 0;
    oldTexture.textureUnit = 0;
    oldTexture.shaderName.clear();
}

Texture& Texture::operator=(const Texture &tex) {
    Texture copy(tex);
    swap(*this, copy);
    return *this;
}
Texture& Texture::operator=(Texture &&oldTexture) noexcept {
    Texture move(std::move(oldTexture));
    swap(*this, move);
    return *this;
}

void Texture::pollIsLoaded() {
    if(!isLoaded) {
        if(textureCache.at(textureCacheKey).first.has_value()) {
            assert(!entry.has_value());

            std::cout << "pollIsLoaded first option\n";
            IDCache = textureCache.at(textureCacheKey).first->getID();
            isLoaded = true;
        }

        std::lock_guard<std::mutex> lock(entryMutex);
        if(entry.has_value() && !textureCache.at(textureCacheKey).first.has_value()) {
            std::cout << "pollIsLoaded second option\n";
            loadOnMain();
            isLoaded = true;
            entry = std::nullopt;
        }
    }
}

void Texture::bind(const Shader &shader) {
    pollIsLoaded();
    glActiveTexture(GL_TEXTURE0 + textureUnit);
    glBindTexture(GL_TEXTURE_2D, IDCache);
    const int textureLoc = glGetUniformLocation(shader.ID, shaderName.c_str());
    glUniform1i(textureLoc, textureUnit);
}

void Texture::unbind() {
    pollIsLoaded();
    glActiveTexture(GL_TEXTURE0 + textureUnit);
    glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::~Texture() {
    if(textureCache.count(textureCacheKey)) {
        textureCache[textureCacheKey].second -= 1;

        //if the refcount drops to 0, delete the element
        if(!textureCache[textureCacheKey].second) {
            textureCache.erase(textureCacheKey);
        }
    }
}
