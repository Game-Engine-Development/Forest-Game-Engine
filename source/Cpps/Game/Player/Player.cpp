#include "Headers/Game/Player/Player.h"

Player::Player() = default;

Player::Player(Camera *camera1, Entity *container1, CollisionHandler &collisionHandler) {
    camera = camera1;
    playerEntity = container1;
    handler = collisionHandler;
    handler.move.eRadius = playerEntity->getScale();
}

void Player::render(Shader &shader, glm::vec3 &lightPos, glm::vec3 &lightColor) {
    playerEntity->render(*camera, shader, lightPos, lightColor);
}

void Player::setSpeed(float speed) {
    currentSpeed = speed;
}

void Player::setLateralSpeed(float speed) {
    lateralSpeed = speed;
}

Entity& Player::getPlayerEntity() {
    return *playerEntity;
}

bool Player::isInAir() {
    return handler.inAir;
}

void Player::jump() {
    handler.currentGravity.y = JUMP_POWER;
    handler.inAir = true;
}

void Player::takeDamage(int damage) {
    currentHealth -= damage;
}

float Player::getHealth() {
    return currentHealth;
}

float Player::getCurrentHealth() {
    return currentHealth;
}
