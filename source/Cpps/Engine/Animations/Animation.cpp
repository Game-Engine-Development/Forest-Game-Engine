#include "Headers/Engine/Animations/Animation.h"

Animation::Animation(float lengthInSeconds, std::vector<KeyFrame> &keyFrames) {
    this->length = lengthInSeconds;
    this->keyFrames = keyFrames;
}

float Animation::getLength() {
    return length;
}

std::vector<KeyFrame> Animation::getKeyFrames() {
    return keyFrames;
}