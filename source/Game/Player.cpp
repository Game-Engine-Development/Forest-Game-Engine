#include "Headers/Game/Player.h"

void Player::setHeight(){
    glm::vec3 lighting(0, 0, 0);
    glm::vec3 pos(camera->getPos().x + 10.0f, terrain->getTerrainHeight(camera->getPos().x + 10.0f, camera->getPos().z + 10.0f) + container->getScale().y, camera->getPos().z + 10.0f);
    container->setPos(pos);
    container->render(*camera, *shader, lighting, lighting);



    //radius = container->getScale().y. radius /= container->getScale().y/2.0f
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

}
