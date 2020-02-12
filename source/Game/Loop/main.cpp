#define STB_IMAGE_IMPLEMENTATION
#include "Headers/stb_image.h"

#include "Headers/Engine/IO/Input.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Headers/Engine/Shader.h"
#include "Headers/Engine/Camera.h"
#include "Headers/Engine/Models/Texture.h"
#include "Headers/Engine/Models/Mesh.h"
#include "Headers/Engine/Models/Entity.h"
#include "Headers/Engine/Terrain/Terrain.h"
#include "Headers/Engine/Terrain/TerrainTextureMap.h"
#include <glm/glm.hpp>
#include "Headers/Game/Player/Player.h"
#include "Headers/Game/Entities/Wolf.h"

#include <iostream>
#include <Headers/Engine/Skybox/Skybox.h>
#include <Headers/Engine/GUI/Button.h>
#include <Headers/Game/Player/Shooter.h>
#include <Headers/Game/Entities/Deer.h>
#include <Headers/Engine/IO/Window.h>

void renderQuad();

int main() {
    Camera camera;
    Player player;
    Window window(&camera);

    std::vector<Entity*> entities;
    std::vector<Terrain*> terrains;
    Shader entityShader("../source/Engine/Models/Shaders/vertexShader.glsl", "../source/Engine/Models/Shaders/fragmentShader.glsl");
    Shader terrainShader("../source/Engine/Terrain/Shaders/terrainVertexShader.glsl", "../source/Engine/Terrain/Shaders/terrainFragmentShader.glsl");
    Shader normalMappedShader("../source/Engine/Models/Shaders/normalMappedVertex.glsl", "../source/Engine/Models/Shaders/normalMappedFragment.glsl");
    Shader skyboxShader("../source/Engine/Skybox/Shaders/skyboxVertexShader.glsl", "../source/Engine/Skybox/Shaders/skyboxFragmentShader.glsl");
    Texture texture("../res/container.jpg", 0);
    Texture normalMap("../res/grass.png", 1);
    Texture containerMap("../res/NormalMap.jpg", 2);
    Texture specularMap("../res/SpecularMap.jpg", 3);
    Texture wolf("../res/wolf.jpg", 0);
    Texture deer("../res/deer.jpg", 0);
    Texture human("../res/human.jpg", 0);
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
    currentTextures.push_back(wolf);
    Entity wolfEntity(containerMesh, currentTextures, glm::vec3(-200, 10, -100), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1));
    Entity wolfEntity2(containerMesh, currentTextures, glm::vec3(200, 10, 100), glm::vec3(0, 45, 0), glm::vec3(1, 1, 1));
    Entity wolfEntity3(containerMesh, currentTextures, glm::vec3(-100, 10, -100), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1));
    Entity wolfEntity4(containerMesh, currentTextures, glm::vec3(100, 10, 100), glm::vec3(0, 45, 0), glm::vec3(1, 1, 1));
    Entity wolfEntity5(containerMesh, currentTextures, glm::vec3(-200, 10, -200), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1));
    Entity wolfEntity6(containerMesh, currentTextures, glm::vec3(200, 10, 200), glm::vec3(0, 45, 0), glm::vec3(1, 1, 1));
    Entity wolfEntity7(containerMesh, currentTextures, glm::vec3(-200, 10, -10), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1));
    Entity wolfEntity8(containerMesh, currentTextures, glm::vec3(20, 10, 100), glm::vec3(0, 45, 0), glm::vec3(1, 1, 1));
    wolfEntity.setAsAnimal();
    wolfEntity2.setAsAnimal();
    wolfEntity3.setAsAnimal();
    wolfEntity4.setAsAnimal();
    wolfEntity5.setAsAnimal();
    wolfEntity6.setAsAnimal();
    wolfEntity7.setAsAnimal();
    wolfEntity8.setAsAnimal();
    currentTextures.clear();
    currentTextures.push_back(deer);
    Entity deerEntity1(containerMesh, currentTextures, glm::vec3(-300, 10, -200), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1));
    Entity deerEntity2(containerMesh, currentTextures, glm::vec3(100, 10, 100), glm::vec3(0, 45, 0), glm::vec3(1, 1, 1));
    Entity deerEntity3(containerMesh, currentTextures, glm::vec3(-300, 10, -200), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1));
    Entity deerEntity4(containerMesh, currentTextures, glm::vec3(200, 10, 100), glm::vec3(0, 45, 0), glm::vec3(1, 1, 1));
    Entity deerEntity5(containerMesh, currentTextures, glm::vec3(-100, 10, -400), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1));
    Entity deerEntity6(containerMesh, currentTextures, glm::vec3(300, 10, 200), glm::vec3(0, 45, 0), glm::vec3(1, 1, 1));
    Entity deerEntity7(containerMesh, currentTextures, glm::vec3(-400, 10, -40), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1));
    Entity deerEntity8(containerMesh, currentTextures, glm::vec3(50, 10, 400), glm::vec3(0, 45, 0), glm::vec3(1, 1, 1));
    deerEntity1.setAsAnimal();
    deerEntity2.setAsAnimal();
    deerEntity3.setAsAnimal();
    deerEntity4.setAsAnimal();
    deerEntity5.setAsAnimal();
    deerEntity6.setAsAnimal();
    deerEntity7.setAsAnimal();
    deerEntity8.setAsAnimal();
    currentTextures.clear();
    TerrainTextureMap terrainMap("../res/blendMap.png", "../res/grass.png", "../res/mud.png", "../res/flowers.png", "../res/path.png");
    TerrainMesh terrainMesh("../res/heightmap.png");
    TerrainMesh terrainMesh1("../res/heightmap2.png");
    Terrain terrain1(terrainMap, terrainMesh1, 0, 0);
    Terrain terrain2(terrainMap, terrainMesh1, -1, 0);
    Terrain terrain3(terrainMap, terrainMesh, -1, -1);
    Terrain terrain4(terrainMap, terrainMesh, 0, -1);
    terrains.push_back(&terrain1);
    terrains.push_back(&terrain2);
    terrains.push_back(&terrain3);
    terrains.push_back(&terrain4);
    std::vector<const char*> textures
    {
        "../res/right.jpg",
        "../res/left.jpg",
        "../res/top.jpg",
        "../res/bottom.jpg",
        "../res/front.jpg",
        "../res/back.jpg"
    };
    CubeMapTexture cubeMapTexture(textures, 0);
    Skybox skybox(cubeMapTexture);
    glm::vec3 lightPos(-3200, 3200, -3200);
    glm::vec3 lightColor(0.7, 0.7, 0.7);

    // uncomment this call to draw in wireframe polygons.
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

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

    Button button((char*) "../res/front.jpg", glm::vec2(0, 0), glm::vec2(0.01, 0.01), NULL, window, std::vector<glm::vec2> {glm::vec2(0.5f,  0.5f), glm::vec2(0.5f, -0.5f), glm::vec2(-0.5f,  0.5f), glm::vec2(-0.5f, -0.5f)}, std::vector<glm::vec2> {glm::vec2(0,  0), glm::vec2(0, 1), glm::vec2(1,  0), glm::vec2(1, 1)}, std::vector<unsigned int> {0, 1, 2, 1, 3, 2});
    Shader buttonShader("../source/Engine/GUI/Shaders/vertexShader.glsl", "../source/Engine/GUI/Shaders/fragmentShader.glsl");

    unsigned int hdrFBO;
    glGenFramebuffers(1, &hdrFBO);
    // create floating point color buffer
    unsigned int colorBuffer;
    glGenTextures(1, &colorBuffer);
    glBindTexture(GL_TEXTURE_2D, colorBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, window.getMode()->width/2.0, window.getMode()->height/2.0, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // create depth buffer (renderbuffer)
    unsigned int rboDepth;
    glGenRenderbuffers(1, &rboDepth);
    glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, window.getMode()->width/2.0, window.getMode()->height/2.0);
    // attach buffers
    glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorBuffer, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "Framebuffer not complete!" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    while (!glfwWindowShouldClose(window) && player.getHealth() > 0)
    {
        Input::getInstance()->processInput(&player);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        skybox.render(skyboxShader, camera);
        player.movePlayer(entities, terrains);
        shooter.update();
        if(shouldShoot) {
            shooter.shoot(entities, terrains);
            shouldShoot = false;
        }

        player.render(normalMappedShader, lightPos, lightColor);
        wolf1.update(camera, normalMappedShader, lightPos, lightColor, entities, terrains);
        wolf2.update(camera, normalMappedShader, lightPos, lightColor, entities, terrains);
        wolf3.update(camera, normalMappedShader, lightPos, lightColor, entities, terrains);
        wolf4.update(camera, normalMappedShader, lightPos, lightColor, entities, terrains);
        wolf5.update(camera, normalMappedShader, lightPos, lightColor, entities, terrains);
        wolf6.update(camera, normalMappedShader, lightPos, lightColor, entities, terrains);
        wolf7.update(camera, normalMappedShader, lightPos, lightColor, entities, terrains);
        wolf8.update(camera, normalMappedShader, lightPos, lightColor, entities, terrains);
        deer1.update(camera, normalMappedShader, lightPos, lightColor, entities, terrains);
        deer2.update(camera, normalMappedShader, lightPos, lightColor, entities, terrains);
        deer3.update(camera, normalMappedShader, lightPos, lightColor, entities, terrains);
        deer4.update(camera, normalMappedShader, lightPos, lightColor, entities, terrains);
        deer5.update(camera, normalMappedShader, lightPos, lightColor, entities, terrains);
        deer6.update(camera, normalMappedShader, lightPos, lightColor, entities, terrains);
        deer7.update(camera, normalMappedShader, lightPos, lightColor, entities, terrains);
        deer8.update(camera, normalMappedShader, lightPos, lightColor, entities, terrains);
        for(Entity* entity : entities) {
            entity->render(camera, normalMappedShader, lightPos, lightColor);
        }
        for(Terrain* terrain : terrains) {
            terrain->render(camera, terrainShader, lightPos, lightColor);
        }

        button.render(buttonShader);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        entityShader.use();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, colorBuffer);
        entityShader.setInt("hdr", false);
        entityShader.setFloat("exposure", 1);
        renderQuad();

        glfwSwapBuffers(window.getWindow());
        glfwPollEvents();
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwDestroyWindow(window.getWindow());
    glfwTerminate();
    return 0;
}

unsigned int quadVAO = 0;
unsigned int quadVBO;
void renderQuad()
{
    if (quadVAO == 0)
    {
        float quadVertices[] = {
                // positions        // texture Coords
                -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
                -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
                1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
                1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        };
        // setup plane VAO
        glGenVertexArrays(1, &quadVAO);
        glGenBuffers(1, &quadVBO);
        glBindVertexArray(quadVAO);
        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), nullptr);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    }
    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}