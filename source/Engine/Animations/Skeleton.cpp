#include "Headers/Engine/Animations/Skeleton.h"

Skeleton::Skeleton(Joint* root, int jointCount) {
    rootJoint = root;
    this->jointCount = jointCount;
}

int Skeleton::getJointCount() {
    return jointCount;
}

Joint Skeleton::getRootJoint() {
    return *rootJoint;
}

std::vector<glm::mat4> Skeleton::getJointTransforms() {
    jointMatrices.clear();
    glm::mat4 defaultMat = glm::mat4(0.0);
    for(int i = 0; i < jointCount; ++i) {
        jointMatrices.push_back(defaultMat);
    }
    addJointsToArray(rootJoint);
}

void Skeleton::addJointsToArray(Joint *joint) {
    jointMatrices[joint->index] = joint->getAnimatedTransform();
    for(Joint* children : joint->children) {
        addJointsToArray(children);
    }
}

