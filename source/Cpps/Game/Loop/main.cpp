#define STB_IMAGE_IMPLEMENTATION
#include "Headers/Engine/Models/stb_image.h"

#include <vector>
#include <array>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "Headers/Game/Environment/BoundingBox.h"
#include "Headers/Engine/Shader/Shader.h"
#include "Headers/Engine/Camera/Camera.h"
#include "Headers/Engine/Models/Texture.h"
#include "Headers/Engine/Models/Mesh.h"
#include "Headers/Engine/Models/Entity.h"
#include "Headers/Engine/Terrain/Terrain.h"
#include "Headers/Engine/Terrain/TerrainTextureMap.h"
#include "Headers/Game/Player/Player.h"
#include "Headers/Game/Entities/Animal.h"
#include "Headers/Engine/IO/Input.h"
#include "Headers/Engine/Skybox/Skybox.h"
#include "Headers/Engine/GUI/Button.h"
#include "Headers/Game/Player/Shooter.h"
#include "Headers/Engine/IO/Window.h"
#include "Headers/Game/Entities/Spirit.h"
#include "Headers/Engine/Graphics/HDR.h"

int main() {
    Camera camera;
    Player player;
    Window window(&camera);
    Input input(&window, &camera);

    HDR hdr(window);

    std::vector<Entity*> entities;

    Shader entityShader(
            "../source/Cpps/Engine/Models/Shaders/vertexShader.glsl",
            "../source/Cpps/Engine/Models/Shaders/fragmentShader.glsl"
            );
    Shader terrainShader(
            "../source/Cpps/Engine/Terrain/Shaders/terrainVertexShader.glsl",
            "../source/Cpps/Engine/Terrain/Shaders/terrainFragmentShader.glsl"
            );
    Shader normalMappedShader(
            "../source/Cpps/Engine/Models/Shaders/normalMappedVertex.glsl",
            "../source/Cpps/Engine/Models/Shaders/normalMappedFragment.glsl"
            );
    Shader skyboxShader(
            "../source/Cpps/Engine/Skybox/Shaders/skyboxVertexShader.glsl",
            "../source/Cpps/Engine/Skybox/Shaders/skyboxFragmentShader.glsl"
            );

    Texture texture("../res/container.jpg", 0);
    Texture normalMap("../res/grass.png", 1);
    Texture containerMap("../res/NormalMap.jpg", 2);
    Texture specularMap("../res/SpecularMap.jpg", 3);
    Texture wolfTexture("../res/wolf.jpg", 0);
    Texture deerTexture("../res/deer.jpg", 0);
    Texture human("../res/human.jpg", 0);
    Texture ghostTexture("../res/ghost.png", 0);

    Mesh containerMesh("../res/container.obj", true);

    Entity playerEntity(
            containerMesh,
            std::vector<Texture>{human},
            glm::vec3(0, 10, 0),
            glm::vec3(0, 0, 0),
            glm::vec3(1,1,1)
            );
    Entity bullet(
            containerMesh,
            std::vector<Texture>{human},
            glm::vec3(0, 0, 0),
            glm::vec3(0, 0, 0),
            glm::vec3(1, 1, 1)
            );

    Entity boundingBoxEntity(
            containerMesh,
            std::vector<Texture>{texture, normalMap, containerMap, specularMap},
            glm::vec3(0, 0, 0),
            glm::vec3(0, 0, 0),
            glm::vec3(500, 500, 500)
            );
    boundingBoxEntity.setFlipped();

    Entity container(
            containerMesh,
            std::vector<Texture>{texture, normalMap, containerMap, specularMap},
            glm::vec3(-250,10,100),
            glm::vec3(0,0,0),
            glm::vec3(10,10,10)
            );
    Entity container2(container);
    container2.translate(0, 20, 25);
    Entity container3(container2);
    container3.translate(0, 20, 25);
    Entity container4(container3);
    container4.translate(0, 20, 25);
    Entity container5(container4);
    container5.translate(0, 20, 25);
    entities.push_back(&container);
    entities.push_back(&container2);
    entities.push_back(&container3);
    entities.push_back(&container4);
    entities.push_back(&container5);

    std::array<Entity, 16> animalEntities = {
            //wolves
            Entity (
                    containerMesh,
                    std::vector<Texture>{wolfTexture},
                    glm::vec3(-200, 10, -100),
                    glm::vec3(0, 0, 0),
                    glm::vec3(2, 1, 1)
                    ),
            Entity (
                    containerMesh,
                    std::vector<Texture>{wolfTexture},
                    glm::vec3(200, 10, 100),
                    glm::vec3(0, 45, 0),
                    glm::vec3(2, 1, 1)
                    ),
            Entity (
                    containerMesh,
                    std::vector<Texture>{wolfTexture},
                    glm::vec3(-100, 10, -100),
                    glm::vec3(0, 0, 0),
                    glm::vec3(2, 1, 1)
                    ),
            Entity (
                    containerMesh,
                    std::vector<Texture>{wolfTexture},
                    glm::vec3(100, 10, 100),
                    glm::vec3(0, 45, 0),
                    glm::vec3(2, 1, 1)
                    ),
            Entity (
                    containerMesh,
                    std::vector<Texture>{wolfTexture},
                    glm::vec3(-200, 10, -200),
                    glm::vec3(0, 0, 0),
                    glm::vec3(2, 1, 1)
                    ),
            Entity (
                    containerMesh,
                    std::vector<Texture>{wolfTexture},
                    glm::vec3(200, 10, 200),
                    glm::vec3(0, 45, 0),
                    glm::vec3(2, 1, 1)
                    ),
            Entity (
                    containerMesh,
                    std::vector<Texture>{wolfTexture},
                    glm::vec3(-200, 10, -10),
                    glm::vec3(0, 0, 0),
                    glm::vec3(2, 1, 1)
                    ),
            Entity (
                    containerMesh,
                    std::vector<Texture>{wolfTexture},
                    glm::vec3(20, 10, 100),
                    glm::vec3(0, 45, 0),
                    glm::vec3(2, 1, 1)
                    ),

            //deer
            Entity (
                    containerMesh,
                    std::vector<Texture>{deerTexture},
                    glm::vec3(-300, 10, -200),
                    glm::vec3(0, 0, 0),
                    glm::vec3(2, 3, 1)
                    ),
            Entity (
                    containerMesh,
                    std::vector<Texture>{deerTexture},
                    glm::vec3(100, 10, 100),
                    glm::vec3(0, 45, 0),
                    glm::vec3(2, 3, 1)
                    ),
            Entity (
                    containerMesh,
                    std::vector<Texture>{deerTexture},
                    glm::vec3(-300, 10, -200),
                    glm::vec3(0, 0, 0),
                    glm::vec3(2, 3, 1)
                    ),
            Entity (
                    containerMesh,
                    std::vector<Texture>{deerTexture},
                    glm::vec3(200, 10, 100),
                    glm::vec3(0, 45, 0),
                    glm::vec3(2, 3, 1)
                    ),
            Entity (
                    containerMesh,
                    std::vector<Texture>{deerTexture},
                    glm::vec3(-100, 10, -400),
                    glm::vec3(0, 0, 0),
                    glm::vec3(2, 3, 1)
                    ),
            Entity (
                    containerMesh,
                    std::vector<Texture>{deerTexture},
                    glm::vec3(300, 10, 200),
                    glm::vec3(0, 45, 0),
                    glm::vec3(2, 3, 1)
                    ),
            Entity (
                    containerMesh,
                    std::vector<Texture>{deerTexture},
                    glm::vec3(-400, 10, -40),
                    glm::vec3(0, 0, 0),
                    glm::vec3(2, 3, 1)
                    ),
            Entity (
                    containerMesh,
                    std::vector<Texture>{deerTexture},
                    glm::vec3(50, 10, 400),
                    glm::vec3(0, 45, 0),
                    glm::vec3(2, 3, 1)
                    ),
    };
    for(Entity &animalEntity : animalEntities) {
        animalEntity.setAsAnimal();
    }

    Entity spiritEntity(
            containerMesh,
            std::vector<Texture>{ghostTexture},
            glm::vec3(100, 100, 100),
            glm::vec3(0, 0, 0),
            glm::vec3(2, 4, 2)
            );
    spiritEntity.setAsAnimal();

    TerrainTextureMap terrainMap(
            "../res/blendMap.png",
            "../res/grass.png",
            "../res/mud.png",
            "../res/flowers.png",
            "../res/path.png"
            );

    TerrainMesh terrainMesh("../res/heightmap.png");
    TerrainMesh terrainMesh1("../res/heightmap2.png");

    std::array<Terrain, 9> terrains = {
            Terrain (terrainMap, terrainMesh, -1, -1),
            Terrain (terrainMap, terrainMesh, -1,  0),
            Terrain (terrainMap, terrainMesh, -1,  1),
            Terrain (terrainMap, terrainMesh,  0, -1),
            Terrain (terrainMap, terrainMesh,  0,  0),
            Terrain (terrainMap, terrainMesh,  0,  1),
            Terrain (terrainMap, terrainMesh,  1, -1),
            Terrain (terrainMap, terrainMesh,  1,  0),
            Terrain (terrainMap, terrainMesh,  1,  1),
    };

    std::vector<const char*> textures
    {
        "../res/Standard-Cube-Map2/px.png",
        "../res/Standard-Cube-Map2/nx.png",
        "../res/Standard-Cube-Map2/py.png",
        "../res/Standard-Cube-Map2/ny.png",
        "../res/Standard-Cube-Map2/pz.png",
        "../res/Standard-Cube-Map2/nz.png"
    };

    CubeMapTexture cubeMapTexture(textures, 0);

    Skybox skybox(cubeMapTexture);

    glm::vec3 lightPos(-3200, 3200, -3200);
    glm::vec3 lightColor(0.7, 0.7, 0.7);

    CollisionHandler playerCollider(&playerEntity);

    player = Player(&camera, &playerEntity, playerCollider);

    playerEntity.setAsPlayerEntity();

    bullet.setAsBullet();

    Shooter shooter(&camera, &bullet, &player);

    entities.push_back(&playerEntity);

    Spirit spirit(spiritEntity, &player);
    entities.push_back(spirit.getEntityPointer());

    BoundingBox boundingBox(&boundingBoxEntity, &spirit);
    boundingBox.turnOn(entities);
    boundingBox.turnOff(entities);

    std::array<Animal, 16> animals;

    for(int i = 0; i < animals.size(); ++ i) {
        //wolf
        if(i < 8) {
            animals[i].create(
                    animalEntities[i],
                    &player,
                    &spirit,
                    2.0f,
                    1.0f
                    );
        }
        //deer
        else {
            animals[i].create(
                    animalEntities[i],
                    &player,
                    &spirit,
                    1.5f,
                    1.0f
                    );
        }
    }
    for(Animal &animal : animals) {
        entities.push_back(animal.getEntityPointer());
    }

    Button button(
            static_cast<const char*>("../res/white.png"),
            glm::vec2(0.0f, 0.0f),
            glm::vec2(0.01, 0.01),
            [&hdr]()->void{hdr.setHDRStatus(!hdr.getHDRStatus());},
            &window
            );
    Shader buttonShader(
            "../source/Cpps/Engine/GUI/Shaders/vertexShader.glsl",
            "../source/Cpps/Engine/GUI/Shaders/fragmentShader.glsl"
            );

    while (!glfwWindowShouldClose(window.getWindow()) && player.getHealth() > 0) {
        Input::getInstance()->processInput(&player);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(static_cast<unsigned int>(GL_COLOR_BUFFER_BIT) | static_cast<unsigned int>(GL_DEPTH_BUFFER_BIT));

        hdr.bind();

        skybox.render(skyboxShader, camera);

        player.movePlayer(entities, terrains, boundingBox.getEntity(), false);

        shooter.update();

        boundingBox.shrink();

        if(Input::getInstance()->isShouldShoot()) {
            shooter.shoot(entities, terrains);
            Input::getInstance()->setShouldShoot(false);
        }

        player.render(normalMappedShader, lightPos, lightColor);

        spirit.update(entities, terrains);

        for(Animal &animal : animals) {
            animal.update<terrains.size()>(entities, terrains);
        }

        for(Entity* entity : entities) {
            entity->render(camera, normalMappedShader, lightPos, lightColor);
        }

        for(Terrain& terrain : terrains) {
            terrain.render(camera, terrainShader, lightPos, lightColor);
        }

        button.render(buttonShader);

        hdr.render(entityShader, 1.5);

        glfwSwapBuffers(window.getWindow());
        glfwPollEvents();
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwDestroyWindow(window.getWindow());
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
    glfwTerminate();
#endif
    return 0;
}
