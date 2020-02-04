#include "Headers/Game/Player/Player.h"

Player::Player() = default;

Player::Player(Camera *camera1, Entity *container1, CollisionHandler &collisionHandler) {
    camera = camera1;
    playerEntity = container1;
    handler = collisionHandler;
    handler.move.eRadius = playerEntity->getScale();
}

void Player::movePlayer(std::vector<Entity*> &entities, std::vector<Terrain*> &terrains) {
    glm::vec3 newRotation(0, -camera->Yaw, 0);
    playerEntity->setRotation(newRotation);
    glm::vec3 finalMove = camera->Front;
    finalMove.y = 0;
    finalMove = glm::normalize(finalMove);
    finalMove *= currentSpeed;
    finalMove += lateralSpeed * camera->Right;
    handler.moveEntity(finalMove, entities, terrains);
    camera->Position = playerEntity->getPos();
    camera->setYPos(playerEntity->getPos().y + playerEntity->getScale().y + 0.5f);
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

void Player::takeDamage(float damage) {
    currentHealth -= damage;
    std::cout << currentHealth << std::endl;
}

float Player::getHealth() {
    return currentHealth;
}