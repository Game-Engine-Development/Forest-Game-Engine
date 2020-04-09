# Project definition
cmake_minimum_required(VERSION 3.10)
project(opengl_game)

# Source files
set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/source")
set(SOURCES "${SRC_DIR}/Cpps/Game/Loop/main.cpp" source/Headers/Engine/Shader/Shader.h source/Cpps/Engine/Shader/Shader.cpp source/Headers/Engine/Models/stb_image.h source/Headers/Engine/Camera/Camera.h source/Cpps/Engine/Camera/Camera.cpp source/Cpps/Engine/Models/Texture.cpp source/Headers/Engine/Models/Texture.h source/Cpps/Engine/Models/Mesh.cpp source/Headers/Engine/Models/Mesh.h source/Cpps/Engine/Models/Entity.cpp source/Headers/Engine/Models/Entity.h source/Cpps/Engine/Terrain/Terrain.cpp source/Headers/Engine/Terrain/Terrain.h source/Cpps/Engine/Terrain/TerrainMesh.cpp source/Headers/Engine/Terrain/TerrainMesh.h source/Cpps/Engine/Terrain/TerrainTextureMap.cpp source/Headers/Engine/Terrain/TerrainTextureMap.h source/Cpps/Game/Player/Player.cpp source/Headers/Game/Player/Player.h source/Cpps/Engine/Skybox/CubeMapTexture.cpp source/Headers/Engine/Skybox/CubeMapTexture.h source/Cpps/Engine/Skybox/Skybox.cpp source/Headers/Engine/Skybox/Skybox.h source/Cpps/Engine/Collisions/Plane.cpp source/Headers/Engine/Collisions/Plane.h source/Cpps/Engine/Collisions/CollisionHandler.cpp source/Headers/Engine/Collisions/CollisionHandler.h source/Cpps/Engine/Animations/Skeleton.cpp source/Headers/Engine/Animations/Skeleton.h source/Cpps/Engine/Animations/Joint.cpp source/Headers/Engine/Animations/Joint.h source/Cpps/Engine/Animations/KeyFrame.cpp source/Headers/Engine/Animations/KeyFrame.h source/Cpps/Engine/Animations/Animation.cpp source/Headers/Engine/Animations/Animation.h source/Cpps/Engine/Animations/JointTransform.cpp source/Headers/Engine/Animations/JointTransform.h source/Cpps/Engine/Animations/Quaternion.cpp source/Headers/Engine/Animations/Quaternion.h source/Cpps/Engine/Animations/AnimationHandler.cpp source/Headers/Engine/Animations/AnimationHandler.h source/Cpps/Engine/GUI/Button.cpp source/Headers/Engine/GUI/Button.h source/Cpps/Game/Player/Shooter.cpp source/Headers/Game/Player/Shooter.h source/Cpps/Game/Entities/Animal.cpp source/Headers/Game/Entities/Animal.h source/Headers/Engine/IO/Window.h source/Cpps/Engine/IO/Window.cpp source/Headers/Engine/IO/Input.h source/Cpps/Engine/IO/Input.cpp source/Headers/Engine/GUI/Quad.h source/Cpps/Engine/GUI/Quad.cpp source/Headers/Engine/Graphics/HDR.h source/Cpps/Engine/Graphics/HDR.cpp source/Headers/Engine/Math/MathUtils.h source/Cpps/Engine/Math/MathUtils.cpp source/Cpps/Game/Entities/Spirit.cpp source/Headers/Game/Entities/Spirit.h source/Cpps/Game/Environment/BoundingBox.cpp source/Headers/Game/Environment/BoundingBox.h source/Headers/Engine/GUI/StaticTextRendering/Character.h source/Cpps/Engine/GUI/StaticTextRendering/ImageFileReader.cpp source/Headers/Engine/GUI/StaticTextRendering/ImageFileReader.h source/Cpps/Engine/GUI/StaticTextRendering/Text.cpp source/Headers/Engine/GUI/StaticTextRendering/Text.h source/Cpps/Engine/GUI/StaticTextRendering/FontFileReader.cpp source/Headers/Engine/GUI/StaticTextRendering/FontFileReader.h source/Cpps/Engine/GUI/StaticTextRendering/Character.cpp source/Cpps/Engine/GUI/TextureResourceContainer.cpp source/Headers/Engine/GUI/TextureResourceContainer.h source/Headers/Game/Environment/Item.h source/Cpps/Game/Environment/Item.cpp)

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
target_link_libraries(${PROJECT_NAME} "glm" "${GLM_LIBRARIES}")   #this line can break on linux
target_include_directories(${PROJECT_NAME} PRIVATE "${GLM_DIR}/include")
target_compile_definitions(${PROJECT_NAME} PRIVATE "GLM_INCLUDE_NONE")   #this line can break on linux

#GLAD
set(GLAD_DIR "${LIB_DIR}/glad")
add_library("glad" "${GLAD_DIR}/src/glad.c")
target_include_directories("glad" PRIVATE "${GLAD_DIR}/include")
target_include_directories(${PROJECT_NAME} PRIVATE "${GLAD_DIR}/include")
target_link_libraries(${PROJECT_NAME} "glad" "${CMAKE_DL_LIBS}")

