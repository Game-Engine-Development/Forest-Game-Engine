#include "Headers/Game/Player/Player.h"

Player::Player(Camera *camera1, Entity *container1, CollisionHandler &collisionHandler)
: camera(camera1), playerEntity(container1), handler(collisionHandler)
{
    handler.move.eRadius = playerEntity->getScale();
}

void Player::render(const Shader &shader, const glm::vec3 lightPos, const glm::vec3 lightColor) const {
    playerEntity->render(*camera, shader, lightPos, lightColor);
}

void Player::setSpeed(const float speed) {
    currentSpeed = speed;
}

void Player::setLateralSpeed(const float speed) {
    lateralSpeed = speed;
}

[[nodiscard]] bool Player::isInAir() const noexcept {
    return handler.inAir;
}

void Player::jump() {
    handler.currentGravity.y = JUMP_POWER;
    handler.inAir = true;
}
