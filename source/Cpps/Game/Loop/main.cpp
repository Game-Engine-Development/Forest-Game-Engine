#define STB_IMAGE_IMPLEMENTATION
#include "../../../../libraries/stb_image.h" //needs to be imported here because of an issue in stb_image

#include <vector>
#include <array>

#include <random>

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
#include "Headers/Engine/Terrain/Terrain.h"
#include "Headers/Engine/IO/Input.h"
#include "Headers/Engine/Skybox/Skybox.h"
#include "Headers/Engine/IO/Window.h"
#include "Headers/Engine/Graphics/HDR.h"
#include "Headers/Engine/Scene/LevelEditor.h"
#include "Headers/Engine/Utils/CommonDeclarations.h"

//@todo rewrite Texture and Mesh classes

#include "Headers/Engine/Graphics/RenderSystem/Renderer.h"

#include "Headers/Engine/Utils/DataStructures/Octree.h" //@todo finish implementing efficient Octree and custom memory allocators
//@todo finish RTS style unit placement for Level Editor
#include "Headers/Engine/Scene/ENTTWrapper.h"

#include "Headers/Engine/Utils/FactoriesAndUtils.h"

#include "Headers/Engine/Utils/GarbageAndTemporary/TemporaryFunctions.h"

struct AudioEngine {
    SoLoud::Soloud gSoloud;
    AudioEngine() {
        gSoloud.init();
    }
    AudioEngine(const AudioEngine&) = delete;
    AudioEngine& operator=(const AudioEngine&) = delete;
    AudioEngine(AudioEngine&&) = delete;
    AudioEngine& operator=(AudioEngine&&) = delete;
    ~AudioEngine() {
        gSoloud.deinit();
    }
};


int main() {
    //make std::cout fast:
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);


    //initialize essential subsystems
    Camera camera;
    Window window(camera);
    Input input(&window, &camera);


    EnttWrapper::Scene scene{};


    using namespace std::string_literals;

    //@todo do something to make dealing with Shaders easier and somehow integrate them with the Scene
    Shader entityShader(
            "../source/Cpps/Engine/Models/Shaders/vertexShader.glsl",
            "../source/Cpps/Engine/Models/Shaders/fragmentShader.glsl"
    );
    Shader normalMappedShader(
            "../source/Cpps/Engine/Models/Shaders/normalMappedVertex.glsl",
            "../source/Cpps/Engine/Models/Shaders/normalMappedFragment.glsl"
    );
    Shader simpleDemoShaders(
            "../source/Cpps/Engine/Models/Shaders/SimpleDemoVertex.glsl",
            "../source/Cpps/Engine/Models/Shaders/SimpleDemoFragment.glsl"
    );
    Shader simpleTerrainShader(
            "../source/Cpps/Engine/Terrain/Shaders/terrainVertexShader.glsl",
            "../source/Cpps/Engine/Terrain/Shaders/simpleTerrainFragmentShader.glsl"
    );
    Shader skyboxShader(
            "../source/Cpps/Engine/Skybox/Shaders/skyboxVertexShader.glsl",
            "../source/Cpps/Engine/Skybox/Shaders/skyboxFragmentShader.glsl"
    );

    Shader mainShader(
            "../source/Cpps/Engine/Models/Shaders/MainShaders/mainVert.glsl",
            "../source/Cpps/Engine/Models/Shaders/MainShaders/mainFrag.glsl"
    );


    for(int i = 0; i < 5; ++i) {
        createBaseEntity(scene, "../res/sphere.obj"s,
                         simpleDemoShaders, {Uniform{"blue"s, 0.f}},
                         {glm::vec3(i), glm::vec3(0), glm::vec3(1)});
    }

    const bool intersection = convexShapeIntersection({scene.getEntityFromIndex(0).getComponent<Component::Drawable>().model.meshes.at(0).vertices}, {scene.getEntityFromIndex(1).getComponent<Component::Drawable>().model.meshes.at(0).vertices});
    std::cout << "intersection: " << intersection << '\n';

    AudioEngine audioEngine; // SoLoud engine
    SoLoud::Wav gWave;      // One wave file

    HDR hdr(window);


    gWave.load("../res/DuskHowl.wav"); // Load a wave

    const Light light {
            "lightPos", glm::vec3(-3200, 3200, -3200),
            "lightColor", glm::vec3(0.7)
    };

    TerrainMesh terrainMesh("../res/heightmap.png");

    Component::TextureComponent terrainTexture("../res/repeating_mud_texture.jpeg", 0);

    std::array<const char*, 6> textures {
            "../res/Standard-Cube-Map2/px.bmp",
            "../res/Standard-Cube-Map2/nx.bmp",
            "../res/Standard-Cube-Map2/py.bmp",
            "../res/Standard-Cube-Map2/ny.bmp",
            "../res/Standard-Cube-Map2/pz.bmp",
            "../res/Standard-Cube-Map2/nz.bmp",
    };

    Skybox skybox(CubeMapTexture(textures, 0));

    //createBaseEntity(scene, "../res/container.obj"s, normalMappedShader,
    //                 {glm::vec3(7), glm::vec3(0), glm::vec3(1)});


    //@todo implement timestep to make fps have constant time between renders

    std::array<Terrain, 9> terrains;
    for (int i = 0, index = 0; i < 3 && index < terrains.size(); ++i) {
        for (int j = 0; j < 3; ++j, ++index) {
            terrains[index] = Terrain(terrainTexture, &terrainMesh, i-1, j-1);
        }
    }


    //const entt::entity worldBox = createBaseEntity(scene, "../res/container.obj"s, normalMappedShader,
    //                              {glm::vec3(0, -10, 0), glm::vec3(0), glm::vec3(1)})->getID();


    LevelEditor editor(&window);


    //Terrain &terrain = terrains[0];

    //std::cout << "pos: " << terrain.getPos() << ' ';

    //for(int i = 0; i < terrain.terrainMesh->getWidth(); ++i) {
        //createBaseEntity(scene, "../res/container.obj"s, normalMappedShader,
        //                 {terrain.getPos() + glm::vec3(terrain.terrainMesh->getX_Y_InWorldCoords(i, i).x, terrain.terrainMesh->getHeight(i, i),
        //                                                                          terrain.terrainMesh->getX_Y_InWorldCoords(i, i).y),glm::vec3(0), glm::vec3(1)});
    //}

    //createBaseEntity(scene, "../res/container.obj"s, normalMappedShader,
    //                 {terrain.getPos() + glm::vec3(TerrainMesh::SIZE/2.f, 0.f,
    //                                         TerrainMesh::SIZE/2.f),glm::vec3(0), glm::vec3(10)});


    //std::cout << "width: " << terrain.terrainMesh->getWidth() << '\n';

    //createBaseEntity(scene, "../res/container.obj"s, normalMappedShader,
    //                 {terrains[0].getPos() + glm::vec3(0, 30, 0),
    //                                             glm::vec3(0), glm::vec3(10)});


    //createBaseEntity(scene, "../res/container.obj"s, normalMappedShader,
    //                 {terrains[0].getPos(), glm::vec3(0), glm::vec3(10)});


    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

    //EnttWrapper::Entity terrainEntity = createBaseEntity(scene, "../res/container.obj"s, normalMappedShader,
    //                 {glm::vec3(0), glm::vec3(0), glm::vec3(10)}).value();


    //createBaseEntity(scene, "../res/Sponza/sponza.obj"s, mainShader,
    //                 {glm::vec3(150, 30, 150), glm::vec3(0, 0, 0), glm::vec3(0.1)});

    //auto tree = createTreeWithTerrain(terrain);
    //std::cout << "isInTree: " << tree.isInTree(glm::vec3{terrain.terrainMesh->getX_Y_InWorldCoords(0, 0).x, terrain.terrainMesh->getHeightFromHeightmap(0, 0), terrain.terrainMesh->getX_Y_InWorldCoords(0, 0).y} + terrain.getPos()) << '\n';

    while (!glfwWindowShouldClose(window.getWindow())) {
        Input::processInput(audioEngine.gSoloud, gWave, window, scene, 0, 5);

        playMusic(window, audioEngine.gSoloud, gWave);
        controlPlayer(window, camera);

        glClear(static_cast<unsigned int>(GL_COLOR_BUFFER_BIT) | static_cast<unsigned int>(GL_DEPTH_BUFFER_BIT));

        //render
        hdr.bind();

        skybox.render(skyboxShader, camera);

        renderScene(scene, camera, {light});

        pickAndColorSpheres(scene, /*worldBox,*/ window, camera); //, tree, terrainEntity);

        raycastPickSphere(window, camera, scene, 0, 5);

        //terrain.render(camera, simpleTerrainShader, light.lightPos, light.lightColor);

        hdr.render(entityShader, 1);


        LevelEditor::update(terrainMesh);


        glfwSwapBuffers(window.getWindow());
        glfwPollEvents();
    }


    return 0;
}
