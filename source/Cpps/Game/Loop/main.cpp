#define STB_IMAGE_IMPLEMENTATION
#include "../../../../libraries/stb_image.h" //needs to be imported here because of an issue in stb_image

#include "Headers/Engine/Scene/ENTTWrapper.h"
#include "Headers/Engine/Utils/DataStructures/Ntree.h"

#include <vector>
#include <array>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>


#include "../libraries/entt/single_include/entt/entt.hpp"

#define WITH_MINIAUDIO
#include "../libraries/soloud/src/backend/miniaudio/miniaudio.h"
#include <soloud.h>
#include <soloud_wav.h>

#include "Headers/Engine/Shader/Shader.h"
#include "Headers/Engine/Camera/Camera.h"
#include "Headers/Engine/Models/Entity.h"
#include "Headers/Engine/Terrain/Terrain.h"
#include "Headers/Engine/IO/Input.h"
#include "Headers/Engine/Skybox/Skybox.h"
#include "Headers/Engine/IO/Window.h"
#include "Headers/Engine/Graphics/HDR.h"
#include "Headers/Engine/Graphics/Materials/Material.h"
#include "Headers/Engine/Scene/LevelEditor.h"
#include "Headers/Engine/Utils/Raycast.h"
#include "Headers/Engine/Utils/CommonDeclarations.h"

#include "Headers/Engine/Models/Sphere.h"

//@todo optimize startup time so that not so much time is spent loading files (using async multithreading)

int main() {
    Camera camera;
    Window window(camera);

    using namespace std::string_literals;
    Entity plane("../res/container.obj"s, false, std::vector{"../res/human.jpg"s},
                 glm::vec3(0), glm::vec3(0), glm::vec3(1000, 0.0, 1000));


    std::vector<Sphere> spheres;
    spheres.reserve(5);
    for(int i = 0; i < 5; ++i) {
        spheres.emplace_back(Transform{glm::vec3(i*10), glm::vec3(0), glm::vec3(1)}, Mesh("../res/Sphere.obj"s, false));
    }


    SoLoud::Soloud gSoloud; // SoLoud engine
    SoLoud::Wav gWave;      // One wave file

    HDR hdr(window);


    gSoloud.init(); // Initialize SoLoud

    gWave.load("../res/DuskHowl.wav"); // Load a wave

    glm::vec3 lightPos(-3200, 3200, -3200);
    glm::vec3 lightColor(0.7);

    std::vector<Entity> entities;

    Shader entityShader(
            "../source/Cpps/Engine/Models/Shaders/vertexShader.glsl",
            "../source/Cpps/Engine/Models/Shaders/fragmentShader.glsl"
    );
    Shader simpleTerrainShader(
            "../source/Cpps/Engine/Terrain/Shaders/terrainVertexShader.glsl",
            "../source/Cpps/Engine/Terrain/Shaders/simpleTerrainFragmentShader.glsl"
    );
    Shader normalMappedShader(
            "../source/Cpps/Engine/Models/Shaders/normalMappedVertex.glsl",
            "../source/Cpps/Engine/Models/Shaders/normalMappedFragment.glsl"
    );
    Shader skyboxShader(
            "../source/Cpps/Engine/Skybox/Shaders/skyboxVertexShader.glsl",
            "../source/Cpps/Engine/Skybox/Shaders/skyboxFragmentShader.glsl"
    );
    Shader simpleDemoShaders(
            "../source/Cpps/Engine/Models/Shaders/SimpleDemoVertex.glsl",
            "../source/Cpps/Engine/Models/Shaders/SimpleDemoFragment.glsl"
    );

    TerrainMesh terrainMesh("../res/heightmap.png");

    Texture terrainTexture("../res/repeating_mud_texture.jpeg", 0);

    std::array<const char*, 6> textures {
            "../res/Standard-Cube-Map2/px.bmp",
            "../res/Standard-Cube-Map2/nx.bmp",
            "../res/Standard-Cube-Map2/py.bmp",
            "../res/Standard-Cube-Map2/ny.bmp",
            "../res/Standard-Cube-Map2/pz.bmp",
            "../res/Standard-Cube-Map2/nz.bmp",
    };

    Skybox skybox(CubeMapTexture(textures, 0));

    using namespace std::string_literals;
    for(int i = 0; i < 5; ++i) {
        entities.emplace_back(
                Entity("../res/deer.obj"s, false, std::vector{"../res/deer.jpg"s},
                        glm::vec3(-5, 5, 5*(i+1)), glm::vec3(0), glm::vec3(1)));
    }

    for(int i = 0; i < 3; ++i) {
        entities.emplace_back(
                Entity("../res/wolf.obj"s, false, std::vector{"../res/wolf.jpg"s},
                       glm::vec3(-5, 5, -5*(i+1)), glm::vec3(0), glm::vec3(1)));
    }

    Entity playerEntity("../res/container.obj"s, false,
                        std::vector<std::string>{"../res/human.jpg"s}
    );

    //@todo implement timestep to make fps have constant time between renders

    CollisionHandler playerCollider(&playerEntity);
    Player player(&camera, &playerEntity, playerCollider);

    std::array<Terrain, 9> terrains;
    for (int i = 0, index = 0; i < 3 && index < terrains.size(); ++i) {
        for (int j = 0; j < 3; ++j, ++index) {
            terrains[index] = Terrain(terrainTexture, &terrainMesh, i-1, j-1);
        }
    }

    Input input(&window, &camera, &spheres, &terrains);


    Entity worldBox("../res/container.obj"s, false, std::vector{"../res/wolf.jpg"s},
                    glm::vec3(0), glm::vec3(0), glm::vec3(1));

//    Entity plane("../res/container.obj"s, false, std::vector{"../res/human.jpg"s},
//                    glm::vec3(0), glm::vec3(0), glm::vec3(1000, 0.0, 1000));

    LevelEditor editor(&window);

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

    while (!glfwWindowShouldClose(window.getWindow())) {
        Input::processInput(player, gSoloud, gWave, window);

        glClear(static_cast<unsigned int>(GL_COLOR_BUFFER_BIT) | static_cast<unsigned int>(GL_DEPTH_BUFFER_BIT));

        static bool isSet = false;

        if(Input::notCursor()) {
            player.movePlayer(entities, terrains);
            isSet = false;
        }
        else {
            //std::cout << "Input::getMouseX(): " << Input::getMouseX() << ", Input::getMouseY(): " << Input::getMouseY() << '\n';
            //std::cout << "worldBox.x: " << worldBox.getPos().x << ", worldBox.y: " << worldBox.getPos().y << ", worldBox.z: " << worldBox.getPos().z << '\n';
            //std::cout << "camera.x: " << camera.getPos().x << ", camera.y: " << camera.getPos().y << ", camera.z: " << camera.getPos().z << '\n';
            if(!isSet) {
                camera.setPosition(spheres[0].getPos() + glm::vec3(1));
                isSet = true;
            }
            SpacePartitionTree<8> tree;

        }

        //render
        hdr.bind();

        //@todo get rid of polling to fix garbage like this:
        terrainTexture.bind(simpleTerrainShader);
        terrainTexture.unbind();

        skybox.render(skyboxShader, camera);

        if(Input::notCursor()) {
            playerEntity.render(camera, normalMappedShader, lightPos, lightColor);

            for (Entity &entity : entities) {
                entity.render(camera, normalMappedShader, lightPos, lightColor);
            }
        }
        else {
            for(int i = 0; i < spheres.size(); ++i) {
                spheres[i].render(camera, simpleDemoShaders, (i == Input::get_g_selected_sphere()) /*if i == chosen circle*/);
            }
            if(Input::getPointOfIntersection().has_value()) {
                std::cout << "has_value()\n";
                worldBox.setPos(Input::getPointOfIntersection().value());
            }
            worldBox.render(camera, normalMappedShader, lightPos, lightColor);
            //plane.render(camera, normalMappedShader, lightPos, lightColor);
        }

        Terrain &terrain = terrains[0];
        //for (Terrain &terrain : terrains) {
        terrain.render(camera, simpleTerrainShader, lightPos, lightColor);
        //}

        hdr.render(entityShader, 1);



        LevelEditor::update(terrainMesh);


        glfwSwapBuffers(window.getWindow());
        glfwPollEvents();
    }

    gSoloud.deinit(); // Clean up!

    return 0;
}
