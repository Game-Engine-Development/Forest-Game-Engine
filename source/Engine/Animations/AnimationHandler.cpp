#include "Headers/Engine/Animations/AnimationHandler.h"

AnimationHandler::AnimationHandler(Skeleton &skeleton) : skeleton(skeleton) {
}

void AnimationHandler::playAnimation(Animation &animation) {
    animationTime = 0;
    this->currentAnimation = animation;
}

void AnimationHandler::update() {
    //if (currentAnimation == nullptr) {
    //    return;
    //}
    increaseAnimationTime();
    std::unordered_map<std::string, glm::mat4> currentPose = calculateCurrentAnimationPose();
    glm::mat4 defaultMatrix = glm::mat4(1.0f);
    applyPoseToJoints(currentPose, skeleton.getRootJoint(), defaultMatrix);
}