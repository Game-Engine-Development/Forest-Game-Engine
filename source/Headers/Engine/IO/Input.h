#pragma once

#include <memory>
#include <array>
#include <GLFW/glfw3.h>
#include <Headers/Engine/Camera/Camera.h>
#include <Headers/Game/Player/Player.h>
#include "Window.h"

class Input {
public:
    Input();

    Input(Window *window, Camera *camera);

    void processInput(Player *player);

    static bool isKeyDown(int key);
    static bool isButtonDown(int button);

    static double getScrollY();
    static double getScrollX();

    static double getMouseY();
    static double getMouseX();

    bool isShouldShoot();
    void setShouldShoot(bool value);

    static std::unique_ptr<Input>& getInstance();
private:
    static void mouse_callback(GLFWwindow* window, double xpos, double ypos);
    static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
    static void cursor_enter_callback(GLFWwindow* window, int entered);

    GLFWwindow *m_window;
    Camera *m_camera;

    std::array<bool, GLFW_KEY_LAST> m_keys;
    std::array<bool, GLFW_MOUSE_BUTTON_LAST> m_buttons;

    double m_scrollX, m_scrollY;
    float lastX, lastY;

    bool firstMouse;

    static std::unique_ptr<Input> instance;

    bool cursor = false;
    bool held = false;
    bool cursorHeld = false;
    bool shouldShoot = false;
};