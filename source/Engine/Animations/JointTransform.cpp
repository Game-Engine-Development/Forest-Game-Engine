#include <glm/gtx/transform.hpp>
#include "Headers/Engine/Animations/JointTransform.h"

JointTransform::JointTransform(glm::vec3 &position, Quaternion &rotation) : position(position), rotation(rotation) {

}

glm::mat4 JointTransform::getLocalTransform() {
    glm::mat4 matrix = glm::mat4(1.0f);
    matrix = glm::translate(matrix, position);
    matrix = matrix * rotation.toRotationMatrix();
    return matrix;
}

JointTransform JointTransform::interpolate(JointTransform &frameA, JointTransform &frameB, float progression) {
    glm::vec3 pos = interpolate(frameA.position, frameB.position, progression);
    Quaternion rot = Quaternion::interpolate(frameA.rotation, frameB.rotation, progression);
    return JointTransform(pos, rot);
}

glm::vec3 JointTransform::interpolate(glm::vec3 start, glm::vec3 end, float progression) {
    float x = start.x + (end.x - start.x) * progression;
    float y = start.y + (end.y - start.y) * progression;
    float z = start.z + (end.z - start.z) * progression;
    return glm::vec3(x, y, z);
}