#pragma once

#include <glm/glm.hpp>
#include "Headers/Engine/Terrain/Terrain.h"
#include "Headers/Engine/Camera.h"
#include "Headers/Engine/Models/Entity.h"
#include "Headers/Engine/Shader.h"
#include <array>

class Player {
private:
    Terrain *terrain;
    Camera *camera;
    Entity *container;
    Shader *shader;

public:
    explicit Player(Terrain *terrain1, Camera *camera1, Entity *container1, Shader *entityShader) : terrain(terrain1), camera(camera1), container(container1), shader(entityShader){};
    void setHeight();
};
