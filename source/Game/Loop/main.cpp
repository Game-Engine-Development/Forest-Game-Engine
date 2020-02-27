#define STB_IMAGE_IMPLEMENTATION
#include "Headers/Engine/Models/stb_image.h"

#include "Headers/Engine/IO/Input.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Headers/Engine/Shader/Shader.h"
#include "Headers/Engine/Camera/Camera.h"
#include "Headers/Engine/Models/Texture.h"
#include "Headers/Engine/Models/Mesh.h"
#include "Headers/Engine/Models/Entity.h"
#include "Headers/Engine/Terrain/Terrain.h"
#include "Headers/Engine/Terrain/TerrainTextureMap.h"
#include <glm/glm.hpp>
#include "Headers/Game/Player/Player.h"
#include "Headers/Game/Entities/Wolf.h"
#include "Headers/Game/Entities/Deer.h"

#include <Headers/Engine/Skybox/Skybox.h>
#include <Headers/Engine/GUI/Button.h>
#include <Headers/Game/Player/Shooter.h>
#include <Headers/Engine/IO/Window.h>
#include <Headers/Game/Entities/Spirit.h>

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
    Shader entityShader("../source/Engine/Models/Shaders/vertexShader.glsl", "../source/Engine/Models/Shaders/fragmentShader.glsl");
    Shader terrainShader("../source/Engine/Terrain/Shaders/terrainVertexShader.glsl", "../source/Engine/Terrain/Shaders/terrainFragmentShader.glsl");
    Shader normalMappedShader("../source/Engine/Models/Shaders/normalMappedVertex.glsl", "../source/Engine/Models/Shaders/normalMappedFragment.glsl");
    Shader skyboxShader("../source/Engine/Skybox/Shaders/skyboxVertexShader.glsl", "../source/Engine/Skybox/Shaders/skyboxFragmentShader.glsl");
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
    Entity wolfEntity(containerMesh, currentTextures, glm::vec3(-200, 10, -100), glm::vec3(0, 0, 0), glm::vec3(2, 1, 1));
    Entity wolfEntity2(containerMesh, currentTextures, glm::vec3(200, 10, 100), glm::vec3(0, 45, 0), glm::vec3(2, 1, 1));
    Entity wolfEntity3(containerMesh, currentTextures, glm::vec3(-100, 10, -100), glm::vec3(0, 0, 0), glm::vec3(2, 1, 1));
    Entity wolfEntity4(containerMesh, currentTextures, glm::vec3(100, 10, 100), glm::vec3(0, 45, 0), glm::vec3(2, 1, 1));
    Entity wolfEntity5(containerMesh, currentTextures, glm::vec3(-200, 10, -200), glm::vec3(0, 0, 0), glm::vec3(2, 1, 1));
    Entity wolfEntity6(containerMesh, currentTextures, glm::vec3(200, 10, 200), glm::vec3(0, 45, 0), glm::vec3(2, 1, 1));
    Entity wolfEntity7(containerMesh, currentTextures, glm::vec3(-200, 10, -10), glm::vec3(0, 0, 0), glm::vec3(2, 1, 1));
    Entity wolfEntity8(containerMesh, currentTextures, glm::vec3(20, 10, 100), glm::vec3(0, 45, 0), glm::vec3(2, 1, 1));
    wolfEntity.setAsAnimal();
    wolfEntity2.setAsAnimal();
    wolfEntity3.setAsAnimal();
    wolfEntity4.setAsAnimal();
    wolfEntity5.setAsAnimal();
    wolfEntity6.setAsAnimal();
    wolfEntity7.setAsAnimal();
    wolfEntity8.setAsAnimal();
    currentTextures.clear();
    currentTextures.push_back(deerTexture);
    Entity deerEntity1(containerMesh, currentTextures, glm::vec3(-300, 10, -200), glm::vec3(0, 0, 0), glm::vec3(2, 3, 1));
    Entity deerEntity2(containerMesh, currentTextures, glm::vec3(100, 10, 100), glm::vec3(0, 45, 0), glm::vec3(2, 3, 1));
    Entity deerEntity3(containerMesh, currentTextures, glm::vec3(-300, 10, -200), glm::vec3(0, 0, 0), glm::vec3(2, 3, 1));
    Entity deerEntity4(containerMesh, currentTextures, glm::vec3(200, 10, 100), glm::vec3(0, 45, 0), glm::vec3(2, 3, 1));
    Entity deerEntity5(containerMesh, currentTextures, glm::vec3(-100, 10, -400), glm::vec3(0, 0, 0), glm::vec3(2, 3, 1));
    Entity deerEntity6(containerMesh, currentTextures, glm::vec3(300, 10, 200), glm::vec3(0, 45, 0), glm::vec3(2, 3, 1));
    Entity deerEntity7(containerMesh, currentTextures, glm::vec3(-400, 10, -40), glm::vec3(0, 0, 0), glm::vec3(2, 3, 1));
    Entity deerEntity8(containerMesh, currentTextures, glm::vec3(50, 10, 400), glm::vec3(0, 45, 0), glm::vec3(2, 3, 1));
    deerEntity1.setAsAnimal();
    deerEntity2.setAsAnimal();
    deerEntity3.setAsAnimal();
    deerEntity4.setAsAnimal();
    deerEntity5.setAsAnimal();
    deerEntity6.setAsAnimal();
    deerEntity7.setAsAnimal();
    deerEntity8.setAsAnimal();
    currentTextures.clear();
    currentTextures.push_back(ghostTexture);
    Entity spiritEntity(containerMesh, currentTextures, glm::vec3(100, 100, 100), glm::vec3(0, 0, 0), glm::vec3(2, 4, 2));
    spiritEntity.setAsAnimal();
    TerrainTextureMap terrainMap("../res/blendMap.png", "../res/grass.png", "../res/mud.png", "../res/flowers.png", "../res/path.png");
    TerrainMesh terrainMesh("../res/heightmap.png");
    TerrainMesh terrainMesh1("../res/heightmap2.png");
    Terrain terrain1(terrainMap, terrainMesh, 0, 0);
    Terrain terrain2(terrainMap, terrainMesh, -1, 0);
    Terrain terrain3(terrainMap, terrainMesh, -1, -1);
    Terrain terrain4(terrainMap, terrainMesh, 0, -1);
    terrains.push_back(&terrain1);
    terrains.push_back(&terrain2);
    terrains.push_back(&terrain3);
    terrains.push_back(&terrain4);
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
    entities.push_back(wolf1.getEntityPointer());
    Wolf wolf2(wolfEntity2, &player);
    entities.push_back(wolf2.getEntityPointer());
    Wolf wolf3(wolfEntity3, &player);
    entities.push_back(wolf3.getEntityPointer());
    Wolf wolf4(wolfEntity4, &player);
    entities.push_back(wolf4.getEntityPointer());
    Wolf wolf5(wolfEntity5, &player);
    entities.push_back(wolf5.getEntityPointer());
    Wolf wolf6(wolfEntity6, &player);
    entities.push_back(wolf6.getEntityPointer());
    Wolf wolf7(wolfEntity7, &player);
    entities.push_back(wolf7.getEntityPointer());
    Wolf wolf8(wolfEntity8, &player);
    entities.push_back(wolf8.getEntityPointer());
    wolves.push_back(&wolf1);
    wolves.push_back(&wolf2);
    wolves.push_back(&wolf3);
    wolves.push_back(&wolf4);
    wolves.push_back(&wolf5);
    wolves.push_back(&wolf6);
    wolves.push_back(&wolf7);
    wolves.push_back(&wolf8);

    Deer deer1(deerEntity1, &player);
    entities.push_back(deer1.getEntityPointer());
    Deer deer2(deerEntity2, &player);
    entities.push_back(deer2.getEntityPointer());
    Deer deer3(deerEntity3, &player);
    entities.push_back(deer3.getEntityPointer());
    Deer deer4(deerEntity4, &player);
    entities.push_back(deer4.getEntityPointer());
    Deer deer5(deerEntity5, &player);
    entities.push_back(deer5.getEntityPointer());
    Deer deer6(deerEntity6, &player);
    entities.push_back(deer6.getEntityPointer());
    Deer deer7(deerEntity7, &player);
    entities.push_back(deer7.getEntityPointer());
    Deer deer8(deerEntity8, &player);
    entities.push_back(deer8.getEntityPointer());
    deers.push_back(&deer1);
    deers.push_back(&deer2);
    deers.push_back(&deer3);
    deers.push_back(&deer4);
    deers.push_back(&deer5);
    deers.push_back(&deer6);
    deers.push_back(&deer7);
    deers.push_back(&deer8);

    Spirit spirit(spiritEntity, &player);
    entities.push_back(spirit.getEntityPointer());

    Button button((char*) "../res/deer.jpg", glm::vec2(0.0f, 0.0f), glm::vec2(0.1, 0.1), nullptr, &window);
    Shader buttonShader("../source/Engine/GUI/Shaders/vertexShader.glsl", "../source/Engine/GUI/Shaders/fragmentShader.glsl");

    while (!glfwWindowShouldClose(window.getWindow()) && player.getHealth() > 0) {
        Input::getInstance()->processInput(&player);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        hdr.bind();

        skybox.render(skyboxShader, camera);
        player.movePlayer(entities, terrains);
        shooter.update();
        if(Input::getInstance()->isShouldShoot()) {
            shooter.shoot(entities, terrains);
            Input::getInstance()->setShouldShoot(false);
        }

        player.render(normalMappedShader, lightPos, lightColor);
        spirit.update(entities, terrains);
        for(Deer* deer : deers) {
            deer->update( entities, terrains);
        }
        for(Wolf* wolf : wolves) {
            wolf->update(entities, terrains);
        }
        for(Entity* entity : entities) {
            entity->render(camera, normalMappedShader, lightPos, lightColor);
        }
        for(Terrain* terrain : terrains) {
            terrain->render(camera, terrainShader, lightPos, lightColor);
        }

        button.render(buttonShader);

        hdr.render(entityShader, true,1.5);

        glfwSwapBuffers(window.getWindow());
        glfwPollEvents();
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwDestroyWindow(window.getWindow());
    glfwTerminate();
    return 0;
}
