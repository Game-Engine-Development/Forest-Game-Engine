#pragma once

#include <memory>
#include <optional>
#include <array>

#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <soloud_wav.h>

#include "Headers/Engine/Terrain/Terrain.h"
#include "Headers/Engine/Camera/Camera.h"
#include "Headers/Engine/IO/Window.h"
#include "Headers/Engine/Utils/Raycast.h"
#include "Headers/Engine/Utils/MiscUtils.h"

#include "Headers/Engine/Scene/ENTTWrapper.h"

class Input {
public:
    Input(const Input&) = delete;
    Input(Input&&) = delete;
    Input& operator=(const Input&) = delete;
    Input& operator=(Input&&) = delete;

    Input() noexcept = default;

    Input(Window *window, Camera *camera, /*std::vector<Sphere> *spheres,*/ std::array<Terrain, 9> *terrains);

    static int get_g_selected_sphere() noexcept;

    static bool isKeyDown(int key) noexcept;
    static bool isButtonDown(int button) noexcept;

    static double getScrollY() noexcept;
    static double getScrollX() noexcept;

    static double getMouseY() noexcept;
    static double getMouseX() noexcept;

    static bool notCursor() noexcept;

    static std::optional<glm::vec3> getPointOfIntersection() {
         return instance->pointOfIntersection;
    }

    static void processInput(SoLoud::Soloud &gSoloud, SoLoud::Wav &gWave, Window &window, EnttWrapper::Scene &scene, int startIndex, int numberOfSpheres);

private:
    void processInputImpl(SoLoud::Soloud &gSoloud, SoLoud::Wav &gWave, Window &window, EnttWrapper::Scene &scene, int startIndex, int numberOfSpheres);
    static void raycastPickSphere(EnttWrapper::Scene &scene, int startIndex, int numberOfSpheres);

    static void mouse_callback([[maybe_unused]] GLFWwindow* window, double xpos, double ypos);
    static void scroll_callback([[maybe_unused]] GLFWwindow* window, double xoffset, double yoffset);
    static void cursor_enter_callback([[maybe_unused]] GLFWwindow* window, int entered);

    Window *m_window{};
    Camera *m_camera{};

    std::array<Terrain, 9> *terrains = nullptr;
    std::optional<glm::vec3> pointOfIntersection = std::nullopt;

    int g_selected_sphere = -1;

    std::array<bool, GLFW_KEY_LAST> m_keys{};
    std::array<bool, GLFW_MOUSE_BUTTON_LAST> m_buttons{};

    double m_scrollX{}, m_scrollY{};
    float lastX{}, lastY{};

    bool firstMouse{};

    static std::unique_ptr<Input> instance;

    bool cursor = false;
    bool held = false;
};