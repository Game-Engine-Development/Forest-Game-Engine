#pragma once

#include <vector>
#include <string>
#include <glm/gtc/type_ptr.hpp>

class Joint {
public:
    int index;
    std::string name;
    std::vector<Joint*> children;

    Joint(int index, std::string name);
    void addChild(Joint* child);
    glm::mat4 getAnimatedTransform();
private:
    glm::mat4 animatedTransform;

};
