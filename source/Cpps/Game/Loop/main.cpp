#define STB_IMAGE_IMPLEMENTATION
#include "../../../../libraries/stb_image.h" //needs to be imported here because of an issue in stb_image

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
#include "Headers/Engine/Terrain/Terrain.h"
#include "Headers/Engine/IO/Input.h"
#include "Headers/Engine/Skybox/Skybox.h"
#include "Headers/Engine/IO/Window.h"
#include "Headers/Engine/Graphics/HDR.h"
#include "Headers/Engine/Graphics/Materials/Material.h"
#include "Headers/Engine/Scene/LevelEditor.h"
#include "Headers/Engine/Utils/CommonDeclarations.h"

//@todo rewrite Texture and Mesh classes

#include "Headers/Engine/Graphics/RenderSystem/Renderer.h"

#include "Headers/Engine/Utils/DataStructures/Ntree.h" //@todo finish implementing efficient Octree and custom memory allocators
//@todo finish RTS style unit placement for Level Editor
#include "Headers/Engine/Scene/ENTTWrapper.h"

#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags
#include <assimp/DefaultLogger.hpp>


int main() {
    Camera camera;
    Window window(camera);

    EnttWrapper::Scene scene{};


    using namespace std::string_literals;

    Shader entityShader(
            "../source/Cpps/Engine/Models/Shaders/vertexShader.glsl",
            "../source/Cpps/Engine/Models/Shaders/fragmentShader.glsl"
    );
    Shader simpleTerrainShader(
            "../source/Cpps/Engine/Terrain/Shaders/terrainVertexShader.glsl",
            "../source/Cpps/Engine/Terrain/Shaders/terrainFragmentShader.glsl"
    );
    Shader normalMappedShader(
            "../source/Cpps/Engine/Models/Shaders/normalMappedVertex.glsl",
            "../source/Cpps/Engine/Models/Shaders/normalMappedFragment.glsl"
    );
    Shader simpleDemoShaders(
            "../source/Cpps/Engine/Models/Shaders/SimpleDemoVertex.glsl",
            "../source/Cpps/Engine/Models/Shaders/SimpleDemoFragment.glsl"
    );

    scene.createEntity().addComponent<Component::Drawable, Component::MeshComponent, std::vector<Component::TextureComponent>, Shader>(
            Component::MeshComponent("../res/container.obj"s, false),
            {Component::TextureComponent("../res/human.jpg"s, 0)}, Shader(normalMappedShader))
                        .addComponent<Component::PosRotationScale, Component::PosRotationScale>(
                 {glm::vec3(0, -10, 0), glm::vec3(0), glm::vec3(1000, 0.0, 1000)});


    for(int i = 0; i < 5; ++i) {
        scene.createEntity().addComponent<Component::Drawable, Component::MeshComponent,
        std::vector<Component::TextureComponent>, Shader, std::vector<Uniform>>(
                Component::MeshComponent("../res/Sphere.obj"s, false), {}, Shader(simpleDemoShaders), {Uniform{"blue"s, 0.f}})
                .addComponent<Component::PosRotationScale>(
                Component::PosRotationScale{glm::vec3(i*10), glm::vec3(0), glm::vec3(1)});
    }


    SoLoud::Soloud gSoloud; // SoLoud engine
    SoLoud::Wav gWave;      // One wave file

    HDR hdr(window);


    gSoloud.init(); // Initialize SoLoud

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

    scene.createEntity().addComponent<Component::Drawable, Component::MeshComponent, std::vector<Component::TextureComponent>, Shader>(
            Component::MeshComponent("../res/container.obj"s, false), {Component::TextureComponent("../res/human.jpg"s, 0)}, Shader(normalMappedShader))
                        .addComponent<Component::PosRotationScale, Component::PosRotationScale>(
                        {glm::vec3(7), glm::vec3(0), glm::vec3(1)});

    //@todo implement timestep to make fps have constant time between renders

    std::array<Terrain, 9> terrains;
    for (int i = 0, index = 0; i < 3 && index < terrains.size(); ++i) {
        for (int j = 0; j < 3; ++j, ++index) {
            terrains[index] = Terrain(terrainTexture, &terrainMesh, i-1, j-1);
        }
    }

    //Input input(&window, &camera, &spheres, &terrains);

    Input input(&window, &camera, &terrains);



    const auto worldBox = scene.createEntity()
                        .addComponent<Component::Drawable, Component::MeshComponent, std::vector<Component::TextureComponent>, Shader>(
                                Component::MeshComponent("../res/container.obj"s, false),
                                {Component::TextureComponent("../res/wolf.jpg"s, 0)}, Shader(normalMappedShader))
                        .addComponent<Component::PosRotationScale, Component::PosRotationScale>(
                                {glm::vec3(0, -10, 0), glm::vec3(0), glm::vec3(1)}).getID();


    LevelEditor editor(&window);


    Terrain &terrain = terrains[0];

    std::cout << "pos: " << terrain.getPos() << ' ';

    scene.createEntity().addComponent<Component::Drawable, Component::MeshComponent, std::vector<Component::TextureComponent>, Shader>(
                            Component::MeshComponent("../res/container.obj"s, false),
                            {Component::TextureComponent("../res/wolf.jpg"s, 0)}, Shader(normalMappedShader))
                        .addComponent<Component::PosRotationScale, Component::PosRotationScale>(
                                {terrain.getPos() + glm::vec3(TerrainMesh::SIZE/2.f, 0.f,
                                TerrainMesh::SIZE/2.f),glm::vec3(0), glm::vec3(10)});


    std::cout << "width: " << terrain.terrainMesh->getWidth() << '\n';

    scene.createEntity()
        .addComponent<Component::Drawable, Component::MeshComponent, std::vector<Component::TextureComponent>, Shader>(
                Component::MeshComponent("../res/container.obj"s, false),
                {Component::TextureComponent("../res/wolf.jpg"s, 0)}, Shader(normalMappedShader))
        .addComponent<Component::PosRotationScale, Component::PosRotationScale>(
                {terrains[0].getPos() + glm::vec3(0, 30, 0),
                 glm::vec3(0), glm::vec3(10)});


    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

    //SmallVector<Foo> tree;

    while (!glfwWindowShouldClose(window.getWindow())) {
        Input::processInput(gSoloud, gWave, window, scene, 1, 5);

        glClear(static_cast<unsigned int>(GL_COLOR_BUFFER_BIT) | static_cast<unsigned int>(GL_DEPTH_BUFFER_BIT));

        //render
        hdr.bind();

        renderScene(scene, camera, {light});

        //for(int i = 0; i < spheres.size(); ++i) {
        //    spheres[i].render(camera, simpleDemoShaders, (i == Input::get_g_selected_sphere()) /*if i == chosen circle*/);
        //}
        for(int i = 1; i < 6; ++i) {
            const float blueAsFloat = (i == Input::get_g_selected_sphere()) ? 1.0f : 0.0f;
            auto &drawable = scene.getEntityFromIndex(i).getComponent<Component::Drawable>();
            drawable.getUniformData(0) = blueAsFloat;
        }
        if(Input::getPointOfIntersection().has_value()) {
            std::cout << "has_value()\n";
            scene.getEntity(worldBox).getComponent<Component::PosRotationScale>().setPos(Input::getPointOfIntersection().value());
        }

        //terrain.render(camera, simpleTerrainShader, lightPos, lightColor);

        hdr.render(entityShader, 1);


        LevelEditor::update(terrainMesh);


        glfwSwapBuffers(window.getWindow());
        glfwPollEvents();
    }


    gSoloud.deinit(); // Clean up!

    return 0;
}
