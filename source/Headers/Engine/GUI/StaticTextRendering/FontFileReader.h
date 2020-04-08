#pragma once

#include <cassert> //for debugging, will remove later

#include <iostream>
#include <fstream>

#include <array>
#include <vector>
#include <string>
#include <unordered_map>

#include "Headers/Engine/GUI/StaticTextRendering/Character.h"
#include "Headers/Engine/Math/MathUtils.h"

class FontFileReader {
public:
    FontFileReader();
    explicit FontFileReader(const char *filename);

    Character getChar(char id);

    ~FontFileReader();
private:
    std::unordered_map<char, Character> fontInfo;
};
