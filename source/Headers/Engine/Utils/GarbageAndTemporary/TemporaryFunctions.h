#pragma once

#include "Headers/Engine/Scene/ENTTWrapper.h"
#include "Headers/Engine/Scene/Components.h"
#include "Headers/Engine/IO/Window.h"
#include "Headers/Engine/Camera/Camera.h"
#include "Headers/Engine/Utils/FactoriesAndUtils.h"

void pickAndColorSpheres(EnttWrapper::Scene &scene, const entt::entity worldBox, const Window &window, const Camera &camera, const DataStructures::Octree &tree, EnttWrapper::Entity terrainEntity) {
    for(int i = 0; i < 5; ++i) {
        const float blueAsFloat = (i == g_selected_sphere) ? 1.0f : 0.0f;
        auto &drawable = scene.getEntityFromIndex(i).getComponent<Component::Drawable>();
        drawable.getUniformData(0) = blueAsFloat;
    }

    const auto pointOfIntersection = getTerrainIntersection(window, camera, tree, terrainEntity);
    if(pointOfIntersection.has_value()) {
        auto &posRotScale = terrainEntity.getComponent<Component::PosRotationScale>();
        posRotScale.setPos(pointOfIntersection.value());
        std::cout << "pointOfIntersection: " << pointOfIntersection.value() << '\n';
    }
}

void playMusic(const Window &window, SoLoud::Soloud &gSoloud, SoLoud::Wav &gWave) {
    if ((glfwGetKey(window.getWindow(), GLFW_KEY_P) == GLFW_PRESS) && gSoloud.getActiveVoiceCount() == 0) {
        screenshot("hello_screenshot.jpg", window);

        gSoloud.play(gWave); // Play the wave
    }
}

void controlPlayer(const Window &window, Camera &camera) {
    const bool onlyOnePressed = (glfwGetKey(window.getWindow(), GLFW_KEY_W) == GLFW_PRESS) ^ (glfwGetKey(window.getWindow(), GLFW_KEY_S) == GLFW_PRESS);
    const bool onlyOnePressed2 = (glfwGetKey(window.getWindow(), GLFW_KEY_D) == GLFW_PRESS) ^ (glfwGetKey(window.getWindow(), GLFW_KEY_A) == GLFW_PRESS);

    const bool onlyOnePressed3 = (glfwGetKey(window.getWindow(), GLFW_KEY_UP) == GLFW_PRESS) ^ (glfwGetKey(window.getWindow(), GLFW_KEY_DOWN) == GLFW_PRESS);

    constexpr float SIDE_SPEED = 3.f;
    constexpr float FORWARD_SPEED = 5.f;
    constexpr float UP_SPEED = 3.f;

    if(onlyOnePressed || onlyOnePressed2 || onlyOnePressed3) {
        glm::vec3 finalMove = camera.getFront();
        finalMove.y = 0;
        finalMove = glm::normalize(finalMove);
        if (onlyOnePressed) {
            if (glfwGetKey(window.getWindow(), GLFW_KEY_W) == GLFW_PRESS) {
                finalMove *= FORWARD_SPEED;
            }
            if (glfwGetKey(window.getWindow(), GLFW_KEY_S) == GLFW_PRESS) {
                finalMove *= -FORWARD_SPEED;
            }
        }
        else {
            finalMove *= 0.f; //do not remove
        }
        if (onlyOnePressed2) {
            if (glfwGetKey(window.getWindow(), GLFW_KEY_D) == GLFW_PRESS) {
                finalMove += SIDE_SPEED * camera.getRight();
            }
            if (glfwGetKey(window.getWindow(), GLFW_KEY_A) == GLFW_PRESS) {
                finalMove += -SIDE_SPEED * camera.getRight();
            }
        }
        if (onlyOnePressed3) {
            if (glfwGetKey(window.getWindow(), GLFW_KEY_UP) == GLFW_PRESS) {
                finalMove.y = UP_SPEED;
            }
            if (glfwGetKey(window.getWindow(), GLFW_KEY_DOWN) == GLFW_PRESS) {
                finalMove.y = -UP_SPEED;
            }
        }

        camera.setPosition(camera.getPosition() + finalMove);
    }
}
