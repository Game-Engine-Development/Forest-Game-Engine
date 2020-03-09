#include "Headers/Engine/Math/MathUtils.h"

float mathRound(float value) {
    return std::round(value * 1000.0f) / 1000.0f;
}