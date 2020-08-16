#pragma once

#include <vector>
#include <string>
#include <glm/gtc/type_ptr.hpp>

class Joint {
public:
    int index;
    std::string name;
    std::vector<Joint*> children;

    Joint(int index, std::string& name, glm::mat4& localBindTransform);
    void addChild(Joint* child);
    glm::mat4 getAnimatedTransform();
    void setAnimatedTransform(glm::mat4& animatedTransform);
    glm::mat4 getInverseBindTransform();
    void calcInverseBindTransform(glm::mat4& parentBindTransform);
private:
    glm::mat4 animatedTransform, inverseBindTransform, localBindTransform;

};
