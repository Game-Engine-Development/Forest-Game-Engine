#include "Headers/Engine/GUI/StaticTextRendering/Text.h"

Text::Text() = default;

Text::Text(Text &&textObj) noexcept {
    ID = textObj.ID;
    textObj.ID = 0;

    text = std::move(textObj.text);
    textObj.text.clear();
}

Text::Text(FontFileReader &fontInfo, ImageFileReader &imageData, const std::string &textString) {
    unsigned long long maxHeight = 0;
    unsigned long long lineWidth = 0;

    for(char c : textString) {
        text.push_back(fontInfo.getChar(c));
        if(fontInfo.getChar(c).height > maxHeight) {
            maxHeight = fontInfo.getChar(c).height;
        }
        lineWidth += fontInfo.getChar(c).width;
    }

    const size_t SIZE = lineWidth*maxHeight*imageData.getNR_Channels();
    std::unique_ptr<unsigned char[]> textureDataBuffer = std::make_unique<unsigned char[]>(SIZE); //creates a buffer of heap memory that is deleted when the constructor scope ends

    for(int i = 0; i < SIZE; ++i) { //@todo replace with memset padding (using for loop) to make more optimized
        textureDataBuffer[i++] = 255;
        textureDataBuffer[i++] = 165;
        textureDataBuffer[i++] = 0;
        textureDataBuffer[i] = 0;
    }

    //@todo work on optimization & code refactoring (two unrelated tasks)
    size_t cursor = 0;
    for(Character &c : text) {
        for(int i = 0; i < c.height; ++i) {
            std::memcpy(textureDataBuffer.get() + ((i+(maxHeight-c.height))*lineWidth + cursor)*imageData.getNR_Channels(), imageData.getRawDataBuffer() + ((i + c.y)*imageData.getRawBufferWidth()*imageData.getNR_Channels() + c.x*imageData.getNR_Channels()), c.width*imageData.getNR_Channels());
        }
        cursor += c.width;
    }

    //@todo replace reverse with proper memory order during initial copying (will have to get rid of memset and memcpy though :( )
    for(int i = 0; i < maxHeight; ++i) { //@todo put the for loop into the reverse function itself
        reverse(textureDataBuffer.get() + (i*lineWidth*imageData.getNR_Channels()), textureDataBuffer.get() + (i*lineWidth*imageData.getNR_Channels()) + (lineWidth*imageData.getNR_Channels()) - 1, imageData.getNR_Channels(),(lineWidth*imageData.getNR_Channels()) / imageData.getNR_Channels());
    }

    glGenTextures(1, &ID);
    glActiveTexture(GL_TEXTURE0 + textureUnit);
    glBindTexture(GL_TEXTURE_2D, ID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    if(textureDataBuffer != nullptr){
        if(imageData.getNR_Channels() == 4) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, lineWidth, maxHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureDataBuffer.get());
        } else {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, lineWidth, maxHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, textureDataBuffer.get());
        }
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cerr << "Text Rendering Failed" << std::endl;
    }
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Text::bind(Shader &shader) const {
    glActiveTexture(GL_TEXTURE0 + textureUnit);
    glBindTexture(GL_TEXTURE_2D, ID);
    std::string name("texture");
    name += std::to_string(textureUnit);
    int textureLoc = glGetUniformLocation(shader.ID, name.c_str());
    glUniform1i(textureLoc, textureUnit);
}

void Text::unbind() {
    glActiveTexture(GL_TEXTURE0 + textureUnit);
    glBindTexture(GL_TEXTURE_2D, 0);
}

Text &Text::operator=(Text &&textObj) noexcept {
    ID = textObj.ID;
    textObj.ID = 0;

    text = std::move(textObj.text);
    textObj.text.clear();

    return *this;
}

unsigned int Text::get_ID() {
    return ID;
}

Text::~Text() {
    glDeleteTextures(1, &ID); //deletes texture unit to stop leaking vram
}
