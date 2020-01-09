#pragma once

#include <glm/gtc/type_ptr.hpp>
#include "Quaternion.h"

class JointTransform {
public:
    JointTransform(glm::vec3& position, Quaternion &rotation);
    glm::mat4 getLocalTransform();
    static JointTransform interpolate(JointTransform& frameA, JointTransform& frameB, float progression);
    static glm::vec3 interpolate(glm::vec3 start, glm::vec3 end, float progression);
private:
    glm::vec3 position;
    Quaternion rotation;
};