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

void AnimationHandler::increaseAnimationTime() {
    animationTime += 1; //add real time
    while(animationTime > currentAnimation.getLength()) {
        animationTime -= currentAnimation.getLength();
    }
}

std::unordered_map<std::string, glm::mat4> AnimationHandler::calculateCurrentAnimationPose() {
    std::vector<KeyFrame> frames = getPreviousAndNextFrames();
    float progression = calculateProgression(frames[0], frames[1]);
    return interpolatePoses(frames[0], frames[1], progression);
}

void AnimationHandler::applyPoseToJoints(std::unordered_map<std::string, glm::mat4> currentPose, Joint joint, glm::mat4 parentTransform) {
    glm::mat4 currentLocalTransform = currentPose.at(joint.name);
    glm::mat4 currentTransform = parentTransform * currentLocalTransform;
    for(Joint* childJoint : joint.children) {
        applyPoseToJoints(currentPose, *childJoint, currentTransform);
    }
    currentTransform *= joint.getInverseBindTransform();
    joint.setAnimatedTransform(currentTransform);
}

std::vector<KeyFrame> AnimationHandler::getPreviousAndNextFrames() {
    std::vector<KeyFrame> allFrames = currentAnimation.getKeyFrames();
    KeyFrame previousFrame  = allFrames[0];
    KeyFrame nextFrame = allFrames[0];
    for(int i = 0; i < allFrames.size(); ++i) {
        nextFrame = allFrames[i];
        if(nextFrame.getTimeStamp() > animationTime) {
            break;
        }
        previousFrame = allFrames[i];
    }
    return std::vector<KeyFrame> {previousFrame, nextFrame};
}

float AnimationHandler::calculateProgression(KeyFrame previousFrame, KeyFrame nextFrame) {
    float totalTime = nextFrame.getTimeStamp() - previousFrame.getTimeStamp();
    float currentTime = animationTime - previousFrame.getTimeStamp();
    return currentTime / totalTime;
}

std::unordered_map<std::string, glm::mat4> AnimationHandler::interpolatePoses(KeyFrame previousFrame, KeyFrame nextFrame, float progression) {
    std::unordered_map<std::string, glm::mat4> currentPose;
    for(auto it = currentPose.begin(); it != currentPose.end(); ++it) {
        JointTransform previousTransform = previousFrame.getJointKeyFrames().at(it->first);
        JointTransform nextTransform = nextFrame.getJointKeyFrames().at(it->first);
        JointTransform currentTransform = JointTransform::interpolate(previousTransform, nextTransform, progression);
        currentPose.emplace(std::pair(it->first, currentTransform.getLocalTransform()));
    }
    return currentPose;
}