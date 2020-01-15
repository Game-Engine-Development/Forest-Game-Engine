#include "Headers/Engine/Animations/Joint.h"

Joint::Joint(int index, std::string& name, glm::mat4& localBindTransform) {
    this->index = index;
    this->name = name;
    this->localBindTransform = localBindTransform;
}

void Joint::addChild(Joint *child) {
    children.push_back(child);
}

glm::mat4 Joint::getAnimatedTransform() {
    return animatedTransform;
}

void Joint::setAnimatedTransform(glm::mat4 &animatedTransform) {
    this->animatedTransform = animatedTransform;
}

glm::mat4 Joint::getInverseBindTransform() {
    return inverseBindTransform;
}

void Joint::calcInverseBindTransform(glm::mat4 &parentBindTransform) {
    glm::mat4 bindTransform = parentBindTransform * localBindTransform;
    inverseBindTransform = glm::inverse(bindTransform);
    for(Joint* child : children) {
        child->calcInverseBindTransform(bindTransform);
    }
}