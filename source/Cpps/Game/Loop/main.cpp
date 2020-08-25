#define STB_IMAGE_IMPLEMENTATION
#include "../../../../libraries/stb_image.h" //needs to be imported here because of an issue in stb_image

#include <vector>
#include <array>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "../libraries/entt/single_include/entt/entt.hpp"

#include "../temporary_lib/imgui_club/imgui_memory_editor/imgui_memory_editor.h"

#define WITH_MINIAUDIO
#include "../libraries/soloud/src/backend/miniaudio/miniaudio.h"
#include <soloud.h>
#include <soloud_wav.h>

#include "Headers/Engine/Shader/Shader.h"
#include "Headers/Engine/Camera/Camera.h"
#include "Headers/Engine/Models/Entity.h"
#include "Headers/Engine/Terrain/Terrain.h"
#include "Headers/Engine/Terrain/TerrainTextureMap.h"
#include "Headers/Engine/IO/Input.h"
#include "Headers/Engine/Skybox/Skybox.h"
#include "Headers/Engine/IO/Window.h"
#include "Headers/Engine/Graphics/HDR.h"
#include "Headers/Engine/Graphics/Materials/Material.h"
#include "Headers/Engine/Constants.h"

//@todo optimize startup time so that not so much time is spent loading files (using async multithreading)

int main() {
    Camera camera;
    Window window(camera);
    Input input(&window, &camera);

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

    std::array<TerrainMesh, 2> terrainMeshes {
            TerrainMesh("../res/heightmap.png", 100),
            TerrainMesh("../res/heightmap2.png", 150),
    };

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
            terrains[index] = Terrain(terrainTexture, &terrainMeshes[0], i-1, j-1);
        }
    }

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    const char *const glsl_version = "#version 330 core";
    ImGui_ImplGlfw_InitForOpenGL(window.getWindow(), true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    constexpr int DATA_SIZE = 50;
    char data[DATA_SIZE];

    static MemoryEditor mem_edit;

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

    while (!glfwWindowShouldClose(window.getWindow())) {
        Input::processInput(player, gSoloud, gWave, window);

        glClear(static_cast<unsigned int>(GL_COLOR_BUFFER_BIT) | static_cast<unsigned int>(GL_DEPTH_BUFFER_BIT));

        player.movePlayer(entities, terrains);

        //render
        hdr.bind();

        skybox.render(skyboxShader, camera);

        for (Entity &entity : entities) {
            entity.render(camera, normalMappedShader, lightPos, lightColor);
        }

        for(Terrain &terrain : terrains) {
            //terrain.render(camera, simpleTerrainShader, lightPos, lightColor);
        }

        hdr.render(entityShader, 1);





        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        static bool show = false;

        mem_edit.DrawWindow("Memory Editor", data, DATA_SIZE);

        if (show) ImGui::ShowDemoWindow(&show);

        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window.getWindow(), &display_w, &display_h);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


        glfwSwapBuffers(window.getWindow());
        glfwPollEvents();
    }

    // IMGUI Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    gSoloud.deinit(); // Clean up!

    return 0;
}
