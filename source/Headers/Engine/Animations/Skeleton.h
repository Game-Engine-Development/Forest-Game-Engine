#pragma once

#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include "Headers/Engine/Animations/Joint.h"

class Skeleton {

public:
    Joint* rootJoint;
    int jointCount;
    std::vector<glm::mat4> jointMatrices;

    Skeleton(Joint* root, int jointCount);
    Joint getRootJoint();
    int getJointCount();
    std::vector<glm::mat4> getJointTransforms();
private:
    void addJointsToArray(Joint* joint);
};
