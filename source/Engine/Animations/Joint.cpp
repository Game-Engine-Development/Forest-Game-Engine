#include "Headers/Engine/Animations/Joint.h"

Joint::Joint(int index, std::string name) {
    this->index = index;
    this->name = name;
}

void Joint::addChild(Joint *child) {
    children.push_back(child);
}

glm::mat4 Joint::getAnimatedTransform() {
    return animatedTransform;
}