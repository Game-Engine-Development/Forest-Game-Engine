#include "Headers/Game/Player/Player.h"

Player::Player() = default;

Player::Player(Camera *camera1, Entity *container1, CollisionHandler &collisionHandler) {
    camera = camera1;
    playerEntity = container1;
    handler = collisionHandler;
    handler.move.eRadius = playerEntity->getScale();
}

void Player::movePlayer(std::vector<Entity*> &entities, std::vector<Terrain*> &terrains, Entity* boundingBox, bool bound) {
    glm::vec3 newRotation(0, -camera->Yaw, 0);
    playerEntity->setRotation(newRotation);
    glm::vec3 finalMove = camera->Front;
    finalMove.y = 0;
    finalMove = glm::normalize(finalMove);
    finalMove *= currentSpeed;
    finalMove += lateralSpeed * camera->Right;
    handler.moveEntity(finalMove, entities, terrains);
    camera->Position = playerEntity->getPos();
    camera->setYPos(playerEntity->getPos().y + 4);
    if(bound) {
        if(playerEntity->getPos().x > boundingBox->getScale().x - 5 + boundingBox->getPos().x) {
            glm::vec3 newPos(boundingBox->getScale().x - 5 + boundingBox->getPos().x, playerEntity->getPos().y, playerEntity->getPos().z);
            playerEntity->setPos(newPos);
        } else if(playerEntity->getPos().x < -boundingBox->getScale().x + 5 + boundingBox->getPos().x) {
            glm::vec3 newPos(-boundingBox->getScale().x + 5 + boundingBox->getPos().x, playerEntity->getPos().y, playerEntity->getPos().z);
            playerEntity->setPos(newPos);
        }
        if(playerEntity->getPos().z > boundingBox->getScale().z - 5 + boundingBox->getPos().z) {
            glm::vec3 newPos(playerEntity->getPos().x, playerEntity->getPos().y, boundingBox->getScale().z - 5 + boundingBox->getPos().z);
            playerEntity->setPos(newPos);
        } else if(playerEntity->getPos().z < -boundingBox->getScale().z + 5 + boundingBox->getPos().z) {
            glm::vec3 newPos(playerEntity->getPos().x, playerEntity->getPos().y, -boundingBox->getScale().z + 5 + boundingBox->getPos().z);
            playerEntity->setPos(newPos);
        }
    }
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
}

float Player::getHealth() {
    return currentHealth;
}