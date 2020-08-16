#include "Headers/Engine/Texture/StaticTextRendering/Text.h"

Text::Text(const std::string &fontFilename, const std::string &imageFilename, const std::string &textString)
: fontFile(fontFilename), imageFile(imageFilename)
{
    size_t maxHeight = 0;
    size_t lineWidth = 0;

    unsigned int ID{};

    for(char c : textString) {
        text.push_back(fontFile.getChar(c));
        if (fontFile.getChar(c).height > maxHeight) {
            maxHeight = fontFile.getChar(c).height;
        }
        lineWidth += fontFile.getChar(c).width;
    }

    const size_t SIZE = lineWidth*maxHeight*imageFile.getNR_Channels();
    std::unique_ptr<unsigned char[]> textureDataBuffer = std::make_unique<unsigned char[]>(SIZE); //creates a buffer of heap memory that is deleted when the constructor scope ends

    size_t cursor = 0;
    for(const Character &c : text) {
        for(int i = 0; i < c.height; ++i) {
            std::memcpy(textureDataBuffer.get() + ((i+(maxHeight-c.height))*lineWidth + cursor)*imageFile.getNR_Channels(), imageFile.getRawDataBuffer() + ((i + c.y)*imageFile.getRawBufferWidth()*imageFile.getNR_Channels() + c.x*imageFile.getNR_Channels()), c.width*imageFile.getNR_Channels());
        }
        cursor += c.width;
    }

    for(int i = 0; i < maxHeight; ++i) { //@todo put the for loop into the reverse function itself
        reverse(textureDataBuffer.get() + (i*lineWidth*imageFile.getNR_Channels()), imageFile.getNR_Channels(),(lineWidth*imageFile.getNR_Channels()) / imageFile.getNR_Channels());
    }

    glGenTextures(1, &ID);
    glActiveTexture(GL_TEXTURE0 + textureUnit);
    glBindTexture(GL_TEXTURE_2D, ID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    if(textureDataBuffer != nullptr){
        if(imageFile.getNR_Channels() == 4) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, lineWidth, maxHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureDataBuffer.get());
        } else {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, lineWidth, maxHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, textureDataBuffer.get());
        }
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cerr << "Text Rendering Failed" << std::endl;
    }
    glBindTexture(GL_TEXTURE_2D, 0);

    IDContainer = std::make_unique<TextureResourceContainer>(ID);
}

void Text::bind(const Shader &shader) const noexcept {
    glActiveTexture(GL_TEXTURE0 + textureUnit);
    glBindTexture(GL_TEXTURE_2D, IDContainer->getID());
    const std::string name = std::string("texture") + std::to_string(textureUnit);
    const int textureLoc = glGetUniformLocation(shader.ID, name.c_str());
    glUniform1i(textureLoc, textureUnit);
}

void Text::unbind() noexcept {
    glActiveTexture(GL_TEXTURE0 + textureUnit);
    glBindTexture(GL_TEXTURE_2D, 0);
}

[[nodiscard]] unsigned int Text::get_ID() const {
    return IDContainer->getID();
}
