# Project definition
cmake_minimum_required(VERSION 3.10)
project(opengl_game)

# Source files
set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/source")
set(SOURCES "${SRC_DIR}/Cpps/Game/Loop/main.cpp" source/Headers/Engine/Shader/Shader.h source/Cpps/Engine/Shader/Shader.cpp source/Headers/Engine/Camera/Camera.h source/Cpps/Engine/Camera/Camera.cpp source/Cpps/Engine/Terrain/Terrain.cpp source/Headers/Engine/Terrain/Terrain.h source/Cpps/Engine/Terrain/TerrainMesh.cpp source/Headers/Engine/Terrain/TerrainMesh.h source/Cpps/Engine/Terrain/TerrainTextureMap.cpp source/Headers/Engine/Terrain/TerrainTextureMap.h source/Cpps/Engine/Skybox/CubeMapTexture.cpp source/Headers/Engine/Skybox/CubeMapTexture.h source/Cpps/Engine/Skybox/Skybox.cpp source/Headers/Engine/Skybox/Skybox.h source/Headers/Engine/IO/Window.h source/Cpps/Engine/IO/Window.cpp source/Headers/Engine/IO/Input.h source/Cpps/Engine/IO/Input.cpp source/Headers/Engine/Graphics/HDR.h source/Cpps/Engine/Graphics/HDR.cpp source/Headers/Engine/Utils/MiscUtils.h source/Cpps/Engine/Utils/MiscUtils.cpp source/Cpps/Engine/ResourceContainers/TextureResourceContainer.cpp source/Headers/Engine/ResourceContainers/TextureResourceContainer.h source/Cpps/Engine/Graphics/Lighting/IBL/HDRI.cpp source/Cpps/Engine/Graphics/Lighting/IBL/IBL.cpp source/Cpps/Engine/Graphics/Lighting/PointLight.cpp source/Cpps/Engine/Graphics/Materials/Material.cpp source/Headers/Engine/Graphics/Lighting/IBL/HDRI.h source/Headers/Engine/Graphics/Lighting/IBL/IBL.h source/Headers/Engine/Graphics/Lighting/PointLight.h source/Headers/Engine/Graphics/Materials/Material.h source/Cpps/Engine/Graphics/Materials/Material.cpp source/Cpps/Engine/ResourceContainers/stb_PointerContainer.cpp source/Headers/Engine/ResourceContainers/stb_PointerContainer.h source/Cpps/Engine/ResourceContainers/MeshResourceContainer.cpp source/Headers/Engine/ResourceContainers/MeshResourceContainer.h source/Cpps/Engine/ResourceContainers/SkyboxResourceContainer.cpp source/Headers/Engine/ResourceContainers/SkyboxResourceContainer.h source/Headers/Engine/Utils/CommonDeclarations.h source/Headers/Engine/Utils/DataStructures/Octree.h source/Headers/Engine/Scene/ENTTWrapper.h source/Cpps/Engine/Scene/LevelEditor.cpp source/Headers/Engine/Scene/LevelEditor.h source/Headers/Engine/Utils/Raycast.h source/Cpps/Engine/Utils/Raycast.cpp source/Headers/Engine/Scene/Components.h source/Headers/Engine/Graphics/RenderSystem/Renderer.h source/Headers/Engine/Scene/LookUpTables.h source/Cpps/Engine/Scene/Components.cpp source/Headers/Engine/Utils/CollisionDetection/CollisionDetection.h source/Cpps/Engine/Utils/CollisionDetection/CollisionDetection.cpp

        #IMGUI:
        libraries/imgui/imconfig.h libraries/imgui/imgui.h libraries/imgui/imgui.cpp libraries/imgui/imgui_demo.cpp libraries/imgui/imgui_draw.cpp libraries/imgui/imgui_widgets.cpp libraries/imgui/imgui_internal.h libraries/imgui/imstb_rectpack.h libraries/imgui/imstb_textedit.h libraries/imgui/imstb_truetype.h
        libraries/imgui/examples/imgui_impl_glfw.h libraries/imgui/examples/imgui_impl_glfw.cpp libraries/imgui/examples/imgui_impl_opengl3.h libraries/imgui/examples/imgui_impl_opengl3.cpp

        #MINIAUDIO:
        libraries/soloud/src/backend/miniaudio/soloud_miniaudio.cpp

        #SOLOUD:
        libraries/soloud/src/audiosource/ay/chipplayer.cpp
        libraries/soloud/src/audiosource/ay/sndbuffer.cpp
        libraries/soloud/src/audiosource/ay/sndchip.cpp
        libraries/soloud/src/audiosource/ay/sndrender.cpp
        libraries/soloud/src/audiosource/ay/soloud_ay.cpp
        libraries/soloud/src/audiosource/monotone/soloud_monotone.cpp
        libraries/soloud/src/audiosource/noise/soloud_noise.cpp
        libraries/soloud/src/audiosource/openmpt/soloud_openmpt.cpp
        libraries/soloud/src/audiosource/openmpt/soloud_openmpt_dll.c
        libraries/soloud/src/audiosource/sfxr/soloud_sfxr.cpp
        libraries/soloud/src/audiosource/speech/darray.cpp
        libraries/soloud/src/audiosource/speech/klatt.cpp
        libraries/soloud/src/audiosource/speech/resonator.cpp
        libraries/soloud/src/audiosource/speech/soloud_speech.cpp
        libraries/soloud/src/audiosource/speech/tts.cpp
        libraries/soloud/src/audiosource/tedsid/sid.cpp
        libraries/soloud/src/audiosource/tedsid/soloud_tedsid.cpp
        libraries/soloud/src/audiosource/tedsid/ted.cpp
        libraries/soloud/src/audiosource/vic/soloud_vic.cpp
        libraries/soloud/src/audiosource/vizsn/soloud_vizsn.cpp
        libraries/soloud/src/audiosource/wav/dr_impl.cpp
        libraries/soloud/src/audiosource/wav/soloud_wav.cpp
        libraries/soloud/src/audiosource/wav/soloud_wavstream.cpp
        libraries/soloud/src/audiosource/wav/stb_vorbis.c
        libraries/soloud/src/backend/alsa/soloud_alsa.cpp
        libraries/soloud/src/backend/miniaudio/soloud_miniaudio.cpp
        libraries/soloud/src/backend/null/soloud_null.cpp
        libraries/soloud/src/backend/oss/soloud_oss.cpp
        libraries/soloud/src/core/soloud.cpp
        libraries/soloud/src/core/soloud_audiosource.cpp
        libraries/soloud/src/core/soloud_bus.cpp
        libraries/soloud/src/core/soloud_core_3d.cpp
        libraries/soloud/src/core/soloud_core_basicops.cpp
        libraries/soloud/src/core/soloud_core_faderops.cpp
        libraries/soloud/src/core/soloud_core_filterops.cpp
        libraries/soloud/src/core/soloud_core_getters.cpp
        libraries/soloud/src/core/soloud_core_setters.cpp
        libraries/soloud/src/core/soloud_core_voicegroup.cpp
        libraries/soloud/src/core/soloud_core_voiceops.cpp
        libraries/soloud/src/core/soloud_fader.cpp
        libraries/soloud/src/core/soloud_fft.cpp
        libraries/soloud/src/core/soloud_fft_lut.cpp
        libraries/soloud/src/core/soloud_file.cpp
        libraries/soloud/src/core/soloud_filter.cpp
        libraries/soloud/src/core/soloud_misc.cpp
        libraries/soloud/src/core/soloud_queue.cpp
        libraries/soloud/src/core/soloud_thread.cpp
        libraries/soloud/src/filter/soloud_bassboostfilter.cpp
        libraries/soloud/src/filter/soloud_biquadresonantfilter.cpp
        libraries/soloud/src/filter/soloud_dcremovalfilter.cpp
        libraries/soloud/src/filter/soloud_echofilter.cpp
        libraries/soloud/src/filter/soloud_eqfilter.cpp
        libraries/soloud/src/filter/soloud_fftfilter.cpp
        libraries/soloud/src/filter/soloud_flangerfilter.cpp
        libraries/soloud/src/filter/soloud_freeverbfilter.cpp
        libraries/soloud/src/filter/soloud_lofifilter.cpp
        libraries/soloud/src/filter/soloud_robotizefilter.cpp
        libraries/soloud/src/filter/soloud_waveshaperfilter.cpp

        #STB_IMAGE:
        libraries/stb_image.h
        libraries/stb_image_write.h

        #NOISE:
        source/Headers/Engine/Utils/NoiseFunctions/OpenSimplex2F.h
        source/Headers/Engine/Utils/NoiseFunctions/OpenSimplex2S.h

        #ENTT:
        libraries/entt/single_include/entt/entt.hpp

        #MEMORY EDITOR:
        temporary_lib/imgui_club/imgui_memory_editor/imgui_memory_editor.h

        #IMGUI-NODE-EDITOR:
        temporary_lib/imgui-node-editor/crude_json.h
        temporary_lib/imgui-node-editor/crude_json.cpp
        temporary_lib/imgui-node-editor/imgui_bezier_math.h
        temporary_lib/imgui-node-editor/imgui_bezier_math.inl
        temporary_lib/imgui-node-editor/imgui_canvas.h
        temporary_lib/imgui-node-editor/imgui_canvas.cpp
        temporary_lib/imgui-node-editor/imgui_extra_math.h
        temporary_lib/imgui-node-editor/imgui_extra_math.inl
        temporary_lib/imgui-node-editor/imgui_node_editor.h
        temporary_lib/imgui-node-editor/imgui_node_editor.cpp
        temporary_lib/imgui-node-editor/imgui_node_editor_api.cpp
        temporary_lib/imgui-node-editor/imgui_node_editor_internal.h
        temporary_lib/imgui-node-editor/imgui_node_editor_internal.inl
        source/Cpps/Engine/Models/VertexSystems.cpp source/Headers/Engine/Models/VertexSystems.h source/Headers/Engine/Texture/TextureSystems.h source/Cpps/Engine/Texture/TextureSystems.cpp)

# Executable definition and properties
add_executable(${PROJECT_NAME} ${SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE "${SRC_DIR}")
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package( Threads REQUIRED )
message(${CMAKE_THREAD_LIBS_INIT}) #pthread library
target_link_libraries(${PROJECT_NAME} PRIVATE ${CMAKE_THREAD_LIBS_INIT} PRIVATE util)


#IMGUI includes:
target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/libraries/imgui")
target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/libraries/imgui/examples")

set(LIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/libraries")

#GLFW
set(GLFW_DIR "${LIB_DIR}/glfw")
set(GLFW_BUILD_EXAMPLES OFF CACHE INTERNAL "Build the GLFW example programs")
set(GLFW_BUILD_TESTS OFF CACHE INTERNAL "Build the GLFW test programs")
set(GLFW_BUILD_DOCS OFF CACHE INTERNAL "Build the GLFW documentation")
set(GLFW_INSTALL OFF CACHE INTERNAL "Generate installation target")
add_subdirectory("${GLFW_DIR}")
target_link_libraries(${PROJECT_NAME} PRIVATE "glfw" PRIVATE "${GLFW_LIBRARIES}")
target_include_directories(${PROJECT_NAME} PRIVATE "${GLFW_DIR}/include")
target_compile_definitions(${PROJECT_NAME} PRIVATE "GLFW_INCLUDE_NONE")

#GLM
set(GLM_DIR "${LIB_DIR}/glm")
set(GLM_BUILD_EXAMPLES OFF CACHE INTERNAL "Build the GLM example programs")
set(GLM_BUILD_TESTS OFF CACHE INTERNAL "Build the GLM test programs")
set(GLM_BUILD_DOCS OFF CACHE INTERNAL "Build the GLM documentation")
set(GLM_INTALL OFF CACHE INTERNAL "Generate installation target")
add_subdirectory("${GLM_DIR}")
target_link_libraries(${PROJECT_NAME} PRIVATE "glm" PRIVATE "${GLM_LIBRARIES}")   #this line can break on linux
target_include_directories(${PROJECT_NAME} PRIVATE "${GLM_DIR}/include")
target_compile_definitions(${PROJECT_NAME} PRIVATE "GLM_INCLUDE_NONE")   #this line can break on linux

#GLAD
set(GLAD_DIR "${LIB_DIR}/glad")
add_library("glad" "${GLAD_DIR}/src/glad.c")
target_include_directories("glad" PRIVATE "${GLAD_DIR}/include")
target_include_directories(${PROJECT_NAME} PRIVATE "${GLAD_DIR}/include")
target_link_libraries(${PROJECT_NAME} PRIVATE "glad" PRIVATE "${CMAKE_DL_LIBS}")

#SOLOUD:
set(SOLOUD_DIR "${LIB_DIR}/soloud")
target_include_directories(${PROJECT_NAME} PRIVATE "${SOLOUD_DIR}/include")

#ASSIMP:
set(ASSIMP_DIR "${LIB_DIR}/assimp")
set(BUILD_SHARED_LIBS ON CACHE INTERNAL "Build package with shared libraries.")
set(ASSIMP_HUNTER_ENABLED OFF CACHE INTERNAL "Turn Off ASSIMP Hunter")
set(ASSIMP_BUILD_ZLIB TRUE CACHE INTERNAL "Build ZLib From Source")
add_subdirectory("${ASSIMP_DIR}")
target_link_libraries(${PROJECT_NAME} PRIVATE "assimp" PRIVATE ${target})
target_include_directories(${PROJECT_NAME} PRIVATE ${ASSIMP_DIR}/include)
