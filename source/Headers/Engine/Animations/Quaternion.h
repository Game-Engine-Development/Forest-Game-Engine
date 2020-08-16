#pragma once

#include <glm/gtc/type_ptr.hpp>

class Quaternion {
public:
    Quaternion(float x, float y, float z, float w);
    void normalize();
    glm::mat4 toRotationMatrix();
    static Quaternion fromMatrix(glm::mat4 matrix);
    static Quaternion interpolate(Quaternion& a, Quaternion& b, float blend);
private:
    float x, y, z, w;
};
