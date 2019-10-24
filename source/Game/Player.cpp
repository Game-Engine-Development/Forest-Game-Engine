#include "Headers/Game/Player.h"

void Player::setHeight(){
    terrainHeight = terrain->getHeight(camera->getPos().x, camera->getPos().z);
    //playerPos.y = terrainHeight;
    //camera->setYPos(terrainHeight);
    glm::vec3 test(0, 0, 0);
    glm::vec3 test2(camera->getPos().x + 10, terrainHeight, camera->getPos().z);
    container->setPos(test2);
    container->render(*camera, *shader, test, test);
    std::cout << "cameraY: " << camera->getPos().y << ", terrainY: " << terrain->getHeight(camera->getPos().x, camera->getPos().z) << std::endl;
}
