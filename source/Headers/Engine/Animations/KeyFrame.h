#pragma once

#include <unordered_map>
#include <string>
#include "JointTransform.h"

class KeyFrame {
public:
    KeyFrame(float timeStamp, std::unordered_map<std::string, JointTransform>& jointKeyFrames);
    float getTimeStamp();
    std::unordered_map<std::string, JointTransform> getJointKeyFrames();
private:
    float timeStamp;
    std::unordered_map<std::string, JointTransform> pose;
};
