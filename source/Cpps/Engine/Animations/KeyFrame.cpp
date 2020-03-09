#include "Headers/Engine/Animations/KeyFrame.h"

KeyFrame::KeyFrame(float timeStamp, std::unordered_map<std::string, JointTransform> &jointKeyFrames) {
    this->timeStamp = timeStamp;
    this->pose = jointKeyFrames;
}

float KeyFrame::getTimeStamp() {
    return timeStamp;
}

std::unordered_map<std::string, JointTransform> KeyFrame::getJointKeyFrames() {
    return pose;
}