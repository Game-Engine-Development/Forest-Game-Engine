#include "Headers/Game/Player.h"

void Player::setHeight(){
    float terrainHeight = 0;
    std::array<float, 4> heights{};
    terrainHeight = terrain->getHeight(camera->getPos().x + container->getScale().x, camera->getPos().z + container->getScale().z);
    heights[0] = terrainHeight;
    terrainHeight = terrain->getHeight(camera->getPos().x + container->getScale().x, camera->getPos().z - container->getScale().z);
    heights[1] = terrainHeight;
    terrainHeight = terrain->getHeight(camera->getPos().x - container->getScale().x, camera->getPos().z + container->getScale().z);
    heights[2] = terrainHeight;
    terrainHeight = terrain->getHeight(camera->getPos().x - container->getScale().x, camera->getPos().z - container->getScale().z);
    heights[3] = terrainHeight;
    terrainHeight = max(heights);

    //playerPos.y = terrainHeight;
    //camera->setYPos(terrainHeight);
    glm::vec3 test(0, 0, 0);
    glm::vec3 test2(camera->getPos().x, std::abs(terrainHeight) + container->getScale().y + std::abs(terrain->getPos().y), camera->getPos().z);
    container->setPos(test2);
    camera->setYPos(container->getPos().y + 5);
    container->render(*camera, *shader, test, test);
    std::cout << "containerY:: " << container->getPos().y << ", terrainY: " << terrain->getHeight(camera->getPos().x, camera->getPos().z) << std::endl;
}

float Player::max(std::array<float, 4> terrainHeights){
    float tempValue = 0;
    for(float terrainHeight : terrainHeights){
        if(std::abs(terrainHeight) > tempValue){
            tempValue = std::abs(terrainHeight);
        }
    }
    return tempValue;
}
