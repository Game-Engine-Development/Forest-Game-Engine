#include "Headers/Game/Player.h"

void Player::setHeight(){
    glm::vec3 test(0, 0, 0);
    glm::vec3 test2(camera->getPos().x + 10, terrain->getTerrainHeight((int)camera->getPos().x + 10, (int)camera->getPos().z + 10), camera->getPos().z + 10);
    container->setPos(test2);
    container->render(*camera, *shader, test, test);
    std::cout << "containerY: " << container->getPos().y << ", terrainY: " << terrain->getTerrainHeight((int)camera->getPos().x + 10, (int)camera->getPos().z + 10) << std::endl;
    std::cout << "containerPos: " << container->getPos().x << " " << container->getPos().z << std::endl;
}
