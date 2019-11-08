#include "Headers/Game/Player.h"

Player::Player() = default;

void Player::setHeight(){
    if(inAir) {
        float terrainHeight = 1 + terrain->getTerrainHeight(playerEntity->getPos().x, playerEntity->getPos().z);
        if(playerEntity->getPos().y + jumpingSpeed <= terrainHeight) {
            inAir = false;
            jumpingSpeed = 0;
            glm::vec3 newPos(playerEntity->getPos().x, terrainHeight, playerEntity->getPos().z);
            playerEntity->setPos(newPos);
        } else {
            glm::vec3 newPos(playerEntity->getPos().x, playerEntity->getPos().y + jumpingSpeed, playerEntity->getPos().z);
            playerEntity->setPos(newPos);
        }
        jumpingSpeed += GRAVITY;
    } else if(playerEntity->getPos().y > 1 + terrain->getTerrainHeight(playerEntity->getPos().x, playerEntity->getPos().z)) {
        inAir = true;
    } else {
        glm::vec3 terrainHeight(playerEntity->getPos().x, 1 + terrain->getTerrainHeight(playerEntity->getPos().x, playerEntity->getPos().z), playerEntity->getPos().z);
        playerEntity->setPos(terrainHeight);
    }
    glm::vec3 lighting(0, 0, 0);

    Plane plane(container->getPos(), glm::vec3(((camera->getPos().x + 10.0f) - container->getPos().x), ((terrain->getTerrainHeight(camera->getPos().x + 10.0f, camera->getPos().z + 10.0f) + container->getScale().y) - container->getPos().y), ((camera->getPos().z + 10.0f) - container->getPos().z)));

    //radius == container->getScale().y. radius /= container->getScale().y/2.0f
    //collision:
    glm::vec3 v(1.0f, 1.0f, 1.0f);
    float v1 = v.x;
    float v2 = v.y;
    float v3 = v.z;
    glm::vec3 radiusVector(container->getScale().x, container->getScale().y, container->getScale().z);
    float e1 = (1.0f/container->getPos().x)*v1 + 0.0f*v2 + 0.0f*v3;
    float e2 = 0.0f*v1 + (1.0f/container->getPos().y)*v2 + 0.0f*v3;
    float e3 = 0.0f*v1 + 0.0f*v2 + (1.0f/container->getPos().z)*v3;
    glm::mat3 CBM(glm::vec3((1.0f/container->getPos().x), 0, 0), glm::vec3(0, (1.0f/container->getPos().y), 0), glm::vec3(0, 0, (1.0f/container->getPos().z)));
    glm::vec3 ve = CBM * v;
    assert(ve == glm::vec3(e1, e2, e3));
    bool frontFacing = plane.isFrontFacingTo(container->getRotation());
    double distance = plane.signedDistanceTo(glm::vec3((camera->getPos().x + 10.0f), (terrain->getTerrainHeight((camera->getPos().x + 10.0f), (camera->getPos().z + 10.0f)) + container->getScale().y), (camera->getPos().z + 10.0f)));
    //std::cout << "facing: " << plane.isFrontFacingTo(container->getRotation()) << ", distance: " << plane.signedDistanceTo(glm::vec3((camera->getPos().x + 10.0f), (terrain->getTerrainHeight((camera->getPos().x + 10.0f), (camera->getPos().z + 10.0f)) + container->getScale().y), (camera->getPos().z + 10.0f))) << std::endl;

    //rendering
    glm::vec3 pos(camera->getPos().x + 10.0f, terrain->getTerrainHeight(camera->getPos().x + 10.0f, camera->getPos().z + 10.0f) + container->getScale().y, camera->getPos().z + 10.0f);
    container->setPos(pos);
    glm::vec3 rotation = glm::vec3(0, -camera->Yaw, 0);
    container->setRotation(rotation);
    container->render(*camera, *shader, lighting, lighting);
}

void Player::move() {
    glm::vec3 newRotation(0, -camera->Yaw, 0);
    getPlayerEntity().setRotation(newRotation);
    glm::vec3 moveDir = camera->Front;
    moveDir.y = 0;
    moveDir = glm::normalize(moveDir);
    glm::vec3 forwardMove = currentSpeed * moveDir;
    glm::vec3 lateralMove = lateralSpeed * camera->Right;
    glm::vec3 finalMove = forwardMove + lateralMove;
    playerEntity->translate(finalMove);
    setHeight();
    camera->Position = playerEntity->getPos();
    camera->setYPos(playerEntity->getPos().y + 1.5f);
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
    return inAir;
}

void Player::jump() {
    jumpingSpeed = JUMP_POWER;
    inAir = true;
}
