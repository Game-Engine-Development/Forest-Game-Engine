#pragma once

#include <array>

struct Character {
    Character();

    explicit Character(const std::array<int, 8> &numbersInLine);

    Character(char id, int x, int y, int width, int height, int xOffset, int yOffset, int xAdvance);

    char id;

    int x;
    int y;
    int width;
    int height;
    int xOffset;
    int yOffset;
    int xAdvance;
};
