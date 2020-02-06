#pragma once

#include <array>
#include <GLFW/glfw3.h>
#include <Headers/Engine/Camera.h>

class Input {
public:
    static void Init(GLFWwindow *window, Camera *camera, const GLFWvidmode *mode);

    static void processInput();

    static bool isKeyDown(int key);
    static bool isButtonDown(int button);

    static double getScrollY();
    static double getScrollX();

    static double getMouseY();
    static double getMouseX();
private:
    static void mouse_callback(GLFWwindow* window, double xpos, double ypos);
    static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
    static void cursor_enter_callback(GLFWwindow* window, int entered);

    static GLFWwindow *m_window;
    static Camera *m_camera;

    static std::array<bool, GLFW_KEY_LAST> m_keys;
    static std::array<bool, GLFW_MOUSE_BUTTON_LAST> m_buttons;

    static double m_scrollX, m_scrollY;
    static float lastX, lastY;

    static bool firstMouse;
};