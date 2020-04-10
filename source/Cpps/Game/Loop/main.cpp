#define STB_IMAGE_IMPLEMENTATION
#include <Headers/Engine/Models/stb_image.h>

#include <memory>
#include <vector>
#include <array>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "Headers/Engine/Shader/Shader.h"
#include "Headers/Engine/Camera/Camera.h"
#include "Headers/Engine/Models/Texture.h"
#include "Headers/Engine/Models/Mesh.h"
#include "Headers/Engine/Models/Entity.h"
#include "Headers/Engine/Terrain/Terrain.h"
#include "Headers/Engine/Terrain/TerrainTextureMap.h"
#include "Headers/Game/Player/Player.h"
#include "Headers/Game/Environment/BoundingBox.h"
#include "Headers/Game/Entities/Spirit.h"
#include "Headers/Game/Player/Shooter.h"
#include "Headers/Game/Environment/Item.h"
#include "Headers/Engine/IO/Input.h"
#include "Headers/Engine/Skybox/Skybox.h"
#include "Headers/Engine/GUI/Button.h"
#include "Headers/Engine/IO/Window.h"
#include "Headers/Engine/Graphics/HDR.h"
#include "Headers/Engine/Graphics/Lighting/PointLight.h"
#include "Headers/Engine/Graphics/Materials/Material.h"

//@todo find out why collision engine broke
//@todo find out why health and damage to player system is completely broken
//@todo rewrite or refactor most of the code in the Game folder
//@todo optimize startup time so that not so much time is spent (look into advanced C++, vectorization, allocators, etc.)
//@todo move raii for opengl texture resources into a heap allocated data class managed by shared_ptr (stops leaking while avoiding copy constructor calls). This will speed up start up time quite a lot.

int main() {
    Camera camera;
    Window window(&camera);
    Player player;
    Input input(&window, &camera);

    HDR hdr(window);

    glm::vec3 lightPos(-3200, 3200, -3200);
    glm::vec3 lightColor(0.7, 0.7, 0.7);

    std::vector<PointLight> pointLights {
            PointLight(glm::vec3(-10, 10, 16)+glm::vec3(-8.75,-8.75,0), glm::vec3(1), 100),
            PointLight(glm::vec3(-10, -10, 16)+glm::vec3(-8.75,-8.75,0), glm::vec3(1), 100),
            PointLight(glm::vec3(10, -10, 16)+glm::vec3(-8.75,-8.75,0), glm::vec3(1), 100),
            PointLight(glm::vec3(10, 10, 16)+glm::vec3(-8.75,-8.75,0), glm::vec3(1), 100)

    };

    std::vector<Entity*> entities;

    Shader pbrShader(
            "../source/Cpps/Engine/Models/Shaders/PBR/pbrVertexShader.glsl",
            "../source/Cpps/Engine/Models/Shaders/PBR/pbrFragmentShader.glsl"
    );
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
    Shader buttonShader(
            "../source/Cpps/Engine/GUI/Shaders/vertexShader.glsl",
            "../source/Cpps/Engine/GUI/Shaders/fragmentShader.glsl"
    );

    std::shared_ptr<Mesh> containerMesh = std::make_shared<Mesh>("../res/container.obj", true);
    std::shared_ptr<Mesh> wolfMesh = std::make_shared<Mesh>("../res/wolf.obj", false);
    std::shared_ptr<Mesh> deerMesh = std::make_shared<Mesh>("../res/deer.obj", false);

    std::array<TerrainMesh, 2> terrainMeshes {
            TerrainMesh("../res/heightmap.png"),
            TerrainMesh("../res/heightmap2.png"),
    };

    TerrainTextureMap terrainMap (
            "../res/blendMap.png",
            "../res/grass.png",
            "../res/mud.png",
            "../res/flowers.png",
            "../res/path.png"
    );
    std::array<const char*, 6> textures {
            "../res/Standard-Cube-Map2/px.bmp",
            "../res/Standard-Cube-Map2/nx.bmp",
            "../res/Standard-Cube-Map2/py.bmp",
            "../res/Standard-Cube-Map2/ny.bmp",
            "../res/Standard-Cube-Map2/pz.bmp",
            "../res/Standard-Cube-Map2/nz.bmp",
    };

    Texture texture("../res/container.jpg", 0, "texture0");
    Texture normalMap("../res/grass.png", 1, "texture1");
    Texture containerMap("../res/NormalMap.jpg", 2, "texture2");
    Texture specularMap("../res/SpecularMap.jpg", 3, "texture3");
    Texture wolfTexture("../res/wolfTexture.png", 0, "texture0");
    Texture deerTexture("../res/deerTexture.png", 0, "texture0");
    Texture human("../res/human.jpg", 0, "texture0");
    Texture ghostTexture("../res/ghost.png", 0, "texture0");
    Texture noteTexture("../res/note.png", 0, "texture0");
    std::vector<Texture> currentTextures;

    Material pavement("../res/pavement", 1);

    Skybox skybox(CubeMapTexture (textures, 0));

    FontFileReader fontFile("../res/calibri.fnt");
    ImageFileReader imageFile("../res/calibri.png");

    Button textButton(
            Text(fontFile, imageFile, std::string("Donnette")),
            glm::vec2(0.0f, 0.0f),
            glm::vec2(1.5, 1.5),
            nullptr,
            &window
    );

    Button textureButton(
            Text(fontFile, imageFile, std::string("+")),
            glm::vec2(0.0f, 0.0f),
            glm::vec2(0.03, 0.05),
            [&hdr]()->void{hdr.setHDRStatus(!hdr.getHDRStatus());},
            &window,
            true
    );

    Entity centerEntity(containerMesh, pavement, pbrShader, glm::vec3(0,5,0), glm::vec3(0,0,0), glm::vec3(5,5,5));

    Entity playerEntity(
            containerMesh,
            std::vector<Texture>{human},
            glm::vec3(0, 10, 0),
            glm::vec3(0, 0, 0),
            glm::vec3(1, 1, 1)
    );
    playerEntity.setAsPlayerEntity();
    entities.push_back(&playerEntity);

    Entity wolfEntity(
            wolfMesh,
            std::vector<Texture>{wolfTexture},
            glm::vec3(0, 10, 0),
            glm::vec3(0, 0, 0),
            glm::vec3(1, 1, 1)
    );
    wolfEntity.setAsAnimal();

    Entity deerEntity(
            deerMesh,
            std::vector<Texture>{deerTexture},
            glm::vec3(0, 10, 0),
            glm::vec3(0, 0, 0),
            glm::vec3(1, 1, 1)
    );
    deerEntity.setAsAnimal();

    Entity spiritEntity(
            containerMesh,
            std::vector<Texture>{ghostTexture},
            glm::vec3(100, 100, 100),
            glm::vec3(0, 0, 0),
            glm::vec3(1, 1, 1)
    );
    spiritEntity.setAsAnimal();

    std::array<Entity, 5> containers;
    //for loop values xTranslate, yTranslate, zTranslate build a staircase with containers
    for (int i = 0, xTranslate = 0, yTranslate = 0, zTranslate = 0;
            i < containers.size();
            ++i, xTranslate += 0, yTranslate += 20, zTranslate += 25) {
        containers[i].create(
                containerMesh,
                std::vector<Texture>{texture, normalMap, containerMap, specularMap},
                glm::vec3(-250 + xTranslate, 10 + yTranslate, 100 + zTranslate),
                glm::vec3(0, 0, 0),
                glm::vec3(10, 10, 10)
        );
        entities.push_back(&containers[i]);
    }

    Entity bullet(
            containerMesh,
            std::vector<Texture>{texture},
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

    Entity noteEntity1(
            containerMesh,
            std::vector<Texture>{noteTexture},
            glm::vec3(10, 2, 0),
            glm::vec3(0, 0, 0),
            glm::vec3(1, 1, 1)
    );

    Entity noteEntity2(noteEntity1);
    noteEntity2.setPos(glm::vec3(0, 2, 10));
    noteEntity1.setAsItem();
    noteEntity2.setAsItem();

    CollisionHandler playerCollider(&playerEntity);
    player = Player(&camera, &playerEntity, playerCollider);
    playerEntity.setAsPlayerEntity();
    bullet.setAsBullet();
    Shooter shooter(&camera, &bullet, &player);
    entities.push_back(&playerEntity);

    Animal wolf1(wolfEntity, &player, 1, 5, 2.0, 1.0);
    Animal deer1(deerEntity, &player, 3, 3, 1.5, 1.0);
    BoundingBox boundingBox(&boundingBoxEntity);
    Spirit spirit(spiritEntity, &player, &boundingBox, wolf1, deer1);
    entities.push_back(spirit.getEntityPointer());
    spirit.spawn(entities);

    Item item1(&noteEntity1, [&hdr]()->void{hdr.setHDRStatus(!hdr.getHDRStatus());}, entities);
    Item item2(&noteEntity2, [&hdr]()->void{hdr.setHDRStatus(!hdr.getHDRStatus());}, entities);

    std::array<Terrain, 9> terrains;
    for (int x0 = -1, index = 0; x0 < 2 && index < terrains.size(); ++x0) {
        for (int x1 = -1; x1 < 2; ++x1, ++index) {
            terrains[index].create(terrainMap, terrainMeshes[0], x0, x1);
        }
    }

    while (!glfwWindowShouldClose(window.getWindow()) && player.getHealth() > 0) {
        Input::getInstance()->processInput(&player);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(static_cast<unsigned int>(GL_COLOR_BUFFER_BIT) | static_cast<unsigned int>(GL_DEPTH_BUFFER_BIT));

        hdr.bind();

        /*
        if(!spirit.isAlive()) {
            double respawnChance = rand() % 500;
            if(respawnChance == 0) {
                spirit.spawn(entities);
            }
        }
         */

        skybox.render(skyboxShader, camera);
        player.movePlayer(entities, terrains, boundingBox.getEntity(), spirit.isBound());
        shooter.update();
        item1.update(entities, &player);
        item2.update(entities, &player);
        if(Input::getInstance()->isShouldShoot()) {
            shooter.shoot(entities, terrains);
            Input::getInstance()->setShouldShoot(false);
        }

        player.movePlayer(entities, terrains, nullptr, false);

        player.render(normalMappedShader, lightPos, lightColor);
        //spirit.updateAnimals(entities, terrains);
        //spirit.update(entities, terrains);
        for(Entity* entity : entities) {
            entity->render(camera, normalMappedShader, lightPos, lightColor);
        }

        for(Terrain &terrain : terrains) {
            terrain.render(camera, terrainShader, lightPos, lightColor);
        }

        centerEntity.render(camera, pointLights);

        textureButton.render(buttonShader);
        //textButton.render(buttonShader); //@todo get rid of the order of rendering of quads mattering

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
