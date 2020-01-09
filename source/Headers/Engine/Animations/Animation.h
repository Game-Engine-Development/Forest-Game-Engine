#pragma once

#include <vector>
#include "KeyFrame.h"

class Animation {
public:
    Animation() = default;
    explicit Animation(float lengthInSeconds, std::vector<KeyFrame>& keyFrames);
    float getLength();
    std::vector<KeyFrame> getKeyFrames();
private:
    float length;
    std::vector<KeyFrame> keyFrames;
};