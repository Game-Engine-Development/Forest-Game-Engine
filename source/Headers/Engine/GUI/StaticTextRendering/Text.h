#pragma once

#include <memory>
#include <vector>
#include <cstring> //memcpy & memset

#include "Headers/Engine/Math/MathUtils.h"
#include "Headers/Engine/GUI/StaticTextRendering/Character.h"
#include "Headers/Engine/GUI/StaticTextRendering/FontFileReader.h"
#include "Headers/Engine/GUI/StaticTextRendering/ImageFileReader.h"
#include "Headers/Engine/Shader/Shader.h"

class Text {
public:
    Text();
    Text(Text &&textObj) noexcept;
    Text(FontFileReader &fontInfo, ImageFileReader &imageData, const std::string &text);

    void bind(Shader &shader) const;
    static void unbind();

    Text& operator=(Text &&textObj) noexcept;

    unsigned int get_ID();

    ~Text();
private:
    std::vector<Character> text;

    unsigned int ID; //texture ID
    static constexpr int textureUnit = 0; //temporary, will change later
};
