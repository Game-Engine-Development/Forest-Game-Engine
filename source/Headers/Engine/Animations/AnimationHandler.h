#pragma once

#include "Skeleton.h"
#include "Animation.h"

class AnimationHandler {
public:
    explicit AnimationHandler(Skeleton& skeleton);
    void playAnimation(Animation& animation);
    void update();
    void increaseAnimationTime();
private:
    Skeleton skeleton;
    Animation currentAnimation;
    float animationTime;
    std::unordered_map<std::string, glm::mat4> calculateCurrentAnimationPose();
    static void applyPoseToJoints(std::unordered_map<std::string, glm::mat4> currentPose, Joint joint, glm::mat4 parentTransform);
    std::vector<KeyFrame> getPreviousAndNextFrames();
    float calculateProgression(KeyFrame previousFrame, KeyFrame nextFrame);
    static std::unordered_map<std::string, glm::mat4> interpolatePoses(KeyFrame previousFrame, KeyFrame nextFrame, float progression);
};