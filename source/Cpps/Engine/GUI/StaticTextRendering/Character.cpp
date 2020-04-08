#include "Headers/Engine/GUI/StaticTextRendering/Character.h"

Character::Character() = default;

Character::Character(const std::array<int, 8> &numbersInLine) :
        id(static_cast<char>(numbersInLine[0])),
        x(numbersInLine[1]),
        y(numbersInLine[2]),
        width(numbersInLine[3]),
        height(numbersInLine[4]),
        xOffset(numbersInLine[5]),
        yOffset(numbersInLine[6]),
        xAdvance(numbersInLine[7])
{}

Character::Character(char id, int x, int y, int width, int height, int xOffset, int yOffset, int xAdvance) :
        id(id),
        x(x),
        y(y),
        width(width),
        height(height),
        xOffset(xOffset),
        yOffset(yOffset),
        xAdvance(xAdvance)
{}
