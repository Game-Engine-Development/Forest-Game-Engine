# Project definition
cmake_minimum_required(VERSION 3.10)
project(opengl_game)

# Source files
set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/source")
set(SOURCES "${SRC_DIR}/Game/Loop/main.cpp" source/Headers/Engine/Shader.h source/Engine/Shader/Shader.cpp source/Headers/stb_image.h source/Headers/Engine/Camera.h source/Engine/Camera/Camera.cpp source/Engine/Models/Texture.cpp source/Headers/Engine/Models/Texture.h source/Engine/Models/Mesh.cpp source/Headers/Engine/Models/Mesh.h source/Engine/Models/Entity.cpp source/Headers/Engine/Models/Entity.h source/Engine/Terrain/Terrain.cpp source/Headers/Engine/Terrain/Terrain.h source/Engine/Terrain/TerrainMesh.cpp source/Headers/Engine/Terrain/TerrainMesh.h source/Engine/Terrain/TerrainTextureMap.cpp source/Headers/Engine/Terrain/TerrainTextureMap.h source/Game/Player.cpp source/Headers/Game/Player.h source/Engine/Skybox/CubeMapTexture.cpp source/Headers/Engine/Skybox/CubeMapTexture.h source/Engine/Skybox/Skybox.cpp source/Headers/Engine/Skybox/Skybox.h source/Engine/Terrain/Plane/Plane.cpp source/Headers/Engine/Terrain/Plane/Plane.h)

# Executable definition and properties
add_executable(${PROJECT_NAME} ${SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE "${SRC_DIR}")
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)

#GLFW
set(LIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/libraries")
set(GLFW_DIR "${LIB_DIR}/glfw")
set(GLFW_BUILD_EXAMPLES OFF CACHE INTERNAL "Build the GLFW example programs")
set(GLFW_BUILD_TESTS OFF CACHE INTERNAL "Build the GLFW test programs")
set(GLFW_BUILD_DOCS OFF CACHE INTERNAL "Build the GLFW documentation")
set(GLFW_INSTALL OFF CACHE INTERNAL "Generate installation target")
add_subdirectory("${GLFW_DIR}")
target_link_libraries(${PROJECT_NAME} "glfw" "${GLFW_LIBRARIES}")
target_include_directories(${PROJECT_NAME} PRIVATE "${GLFW_DIR}/include")
target_compile_definitions(${PROJECT_NAME} PRIVATE "GLFW_INCLUDE_NONE")

#GLM
set(GLM_DIR "${LIB_DIR}/glm")
set(GLM_BUILD_EXAMPLES OFF CACHE INTERNAL "Build the GLM example programs")
set(GLM_BUILD_TESTS OFF CACHE INTERNAL "Build the GLM test programs")
set(GLM_BUILD_DOCS OFF CACHE INTERNAL "Build the GLM documentation")
set(GLM_INTALL OFF CACHE INTERNAL "Generate installation target")
add_subdirectory("${GLM_DIR}")
#target_link_libraries(${PROJECT_NAME} "glm" "${GLM_LIBRARIES}")
target_include_directories(${PROJECT_NAME} PRIVATE "${GLM_DIR}/include")
target_compile_definitions(${PROJECT_NAME} PRIVATE "GLM_INCLUDE_NONE")


#GLAD
set(GLAD_DIR "${LIB_DIR}/glad")
add_library("glad" "${GLAD_DIR}/src/glad.c")
target_include_directories("glad" PRIVATE "${GLAD_DIR}/include")
target_include_directories(${PROJECT_NAME} PRIVATE "${GLAD_DIR}/include")
target_link_libraries(${PROJECT_NAME} "glad" "${CMAKE_DL_LIBS}")

