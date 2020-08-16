#pragma once

#include <memory>
#include <array>

#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <soloud_wav.h>

#include "Headers/Engine/Camera/Camera.h"
#include "Headers/Game/Player/Player.h"
#include "Window.h"
#include "Headers/Engine/Utils/MiscUtils.h"

class Input {
public:
    Input(const Input&) = delete;
    Input(Input&&) = delete;
    Input& operator=(const Input&) = delete;
    Input& operator=(Input&&) = delete;

    Input() noexcept = default;

    Input(Window *window, Camera *camera) noexcept;

    static bool isKeyDown(int key) noexcept;
    static bool isButtonDown(int button) noexcept;

    static double getScrollY() noexcept;
    static double getScrollX() noexcept;

    static double getMouseY() noexcept;
    static double getMouseX() noexcept;

    static bool isShouldShoot() noexcept;
    static void setShouldShoot(bool value) noexcept;

    static void processInput(Player &player, SoLoud::Soloud &gSoloud, SoLoud::Wav &gWave, Window &window) noexcept;

private:
    void processInputImpl(Player &player, SoLoud::Soloud &gSoloud, SoLoud::Wav &gWave, Window &window) noexcept;

    static void mouse_callback([[maybe_unused]] GLFWwindow* window, double xpos, double ypos);
    static void scroll_callback([[maybe_unused]] GLFWwindow* window, double xoffset, double yoffset);
    static void cursor_enter_callback([[maybe_unused]] GLFWwindow* window, int entered);

    GLFWwindow *m_window{};
    Camera *m_camera{};

    std::array<bool, GLFW_KEY_LAST> m_keys{};
    std::array<bool, GLFW_MOUSE_BUTTON_LAST> m_buttons{};

    double m_scrollX{}, m_scrollY{};
    float lastX{}, lastY{};

    bool firstMouse{};

    static std::unique_ptr<Input> instance;

    bool cursor = false;
    bool held = false;
    bool cursorHeld = false;
    bool shouldShoot = false;
};