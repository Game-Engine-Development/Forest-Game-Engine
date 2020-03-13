#define STB_IMAGE_IMPLEMENTATION
#include "Headers/Engine/Models/stb_image.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <Headers/Game/Environment/BoundingBox.h>
#include "Headers/Engine/Shader/Shader.h"
#include "Headers/Engine/Camera/Camera.h"
#include "Headers/Engine/Models/Texture.h"
#include "Headers/Engine/Models/Mesh.h"
#include "Headers/Engine/Models/Entity.h"
#include "Headers/Engine/Terrain/Terrain.h"
#include "Headers/Engine/Terrain/TerrainTextureMap.h"
#include "Headers/Game/Player/Player.h"
#include "Headers/Game/Entities/Wolf.h"
#include "Headers/Game/Entities/Deer.h"
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
    std::vector<Terrain*> terrains;
    std::vector<Wolf*> wolves;
    std::vector<Deer*> deers;
    Shader entityShader("../source/Cpps/Engine/Models/Shaders/vertexShader.glsl", "../source/Cpps/Engine/Models/Shaders/fragmentShader.glsl");
    Shader terrainShader("../source/Cpps/Engine/Terrain/Shaders/terrainVertexShader.glsl", "../source/Cpps/Engine/Terrain/Shaders/terrainFragmentShader.glsl");
    Shader normalMappedShader("../source/Cpps/Engine/Models/Shaders/normalMappedVertex.glsl", "../source/Cpps/Engine/Models/Shaders/normalMappedFragment.glsl");
    Shader skyboxShader("../source/Cpps/Engine/Skybox/Shaders/skyboxVertexShader.glsl", "../source/Cpps/Engine/Skybox/Shaders/skyboxFragmentShader.glsl");
    Texture texture("../res/container.jpg", 0);
    Texture normalMap("../res/grass.png", 1);
    Texture containerMap("../res/NormalMap.jpg", 2);
    Texture specularMap("../res/SpecularMap.jpg", 3);
    Texture wolfTexture("../res/wolf.jpg", 0);
    Texture deerTexture("../res/deer.jpg", 0);
    Texture human("../res/human.jpg", 0);
    Texture ghostTexture("../res/ghost.png", 0);
    std::vector<Texture> currentTextures;
    Mesh containerMesh("../res/container.obj", true);
    currentTextures.push_back(human);
    Entity playerEntity(containerMesh, currentTextures, glm::vec3(0, 10, 0), glm::vec3(0, 0, 0), glm::vec3(1,1,1));
    Entity bullet(containerMesh, currentTextures, glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1));
    currentTextures.clear();
    currentTextures.push_back(texture);
    currentTextures.push_back(normalMap);
    currentTextures.push_back(containerMap);
    currentTextures.push_back(specularMap);
    Entity boundingBoxEntity(containerMesh, currentTextures, glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(500, 500, 500));
    boundingBoxEntity.setFlipped();
    Entity container(containerMesh, currentTextures, glm::vec3(-250,10,100), glm::vec3(0,0,0), glm::vec3(10,10,10));
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
    currentTextures.clear();
    currentTextures.push_back(wolfTexture);
    Entity wolfEntity(containerMesh, currentTextures, glm::vec3(-200, 10, -100), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1));
    wolfEntity.setAsAnimal();
    currentTextures.clear();
    currentTextures.push_back(deerTexture);
    Entity deerEntity1(containerMesh, currentTextures, glm::vec3(-300, 10, -200), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1));
    deerEntity1.setAsAnimal();
    currentTextures.clear();
    currentTextures.push_back(ghostTexture);
    Entity spiritEntity(containerMesh, currentTextures, glm::vec3(100, 100, 100), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1));
    spiritEntity.setAsAnimal();
    TerrainTextureMap terrainMap("../res/blendMap.png", "../res/grass.png", "../res/mud.png", "../res/flowers.png", "../res/path.png");
    TerrainMesh terrainMesh("../res/heightMap.png");
    Terrain terrain1(terrainMap, terrainMesh, -1, -1);
    Terrain terrain2(terrainMap, terrainMesh, -1, 0);
    Terrain terrain3(terrainMap, terrainMesh, -1, 1);
    Terrain terrain4(terrainMap, terrainMesh, 0, -1);
    Terrain terrain5(terrainMap, terrainMesh, 0, 0);
    Terrain terrain6(terrainMap, terrainMesh, 0, 1);
    Terrain terrain7(terrainMap, terrainMesh, 1, -1);
    Terrain terrain8(terrainMap, terrainMesh, 1, 0);
    Terrain terrain9(terrainMap, terrainMesh, 1, 1);
    terrains.push_back(&terrain1);
    terrains.push_back(&terrain2);
    terrains.push_back(&terrain3);
    terrains.push_back(&terrain4);
    terrains.push_back(&terrain5);
    terrains.push_back(&terrain6);
    terrains.push_back(&terrain7);
    terrains.push_back(&terrain8);
    terrains.push_back(&terrain9);
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

    Wolf wolf1(wolfEntity, &player);
    Deer deer1(deerEntity1, &player);
    BoundingBox boundingBox(&boundingBoxEntity);
    Spirit spirit(spiritEntity, &player, &boundingBox, wolf1, deer1);
    entities.push_back(spirit.getEntityPointer());

    spirit.spawn(entities);

    Button button((char*) "../res/white.png", glm::vec2(0.0f, 0.0f), glm::vec2(0.01, 0.01), [&hdr]()->void{hdr.setHDRStatus(!hdr.getHDRStatus());}, &window);
    Shader buttonShader("../source/Cpps/Engine/GUI/Shaders/vertexShader.glsl", "../source/Cpps/Engine/GUI/Shaders/fragmentShader.glsl");

    while (!glfwWindowShouldClose(window.getWindow()) && player.getHealth() > 0) {
        Input::getInstance()->processInput(&player);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        hdr.bind();

        if(!spirit.isAlive()) {
            double respawnChance = rand() % 500;
            if(respawnChance == 0) {
                spirit.spawn(entities);
            }
        }

        skybox.render(skyboxShader, camera);
        player.movePlayer(entities, terrains, boundingBox.getEntity(), spirit.isBound());
        shooter.update();
        if(Input::getInstance()->isShouldShoot()) {
            shooter.shoot(entities, terrains);
            Input::getInstance()->setShouldShoot(false);
        }

        player.render(normalMappedShader, lightPos, lightColor);
        spirit.updateAnimals(entities, terrains);
        spirit.update(entities, terrains);
        for(Entity* entity : entities) {
            entity->render(camera, normalMappedShader, lightPos, lightColor);
        }
        for(Terrain* terrain : terrains) {
            terrain->render(camera, terrainShader, lightPos, lightColor);
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
