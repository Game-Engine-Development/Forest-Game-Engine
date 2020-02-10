#pragma once

#include <memory>
#include <array>
#include <GLFW/glfw3.h>
#include <Headers/Engine/Camera.h>

class Input {
public:
    static void Init(GLFWwindow *window, Camera *camera, const GLFWvidmode *mode);

    void processInput();

    bool isKeyDown(int key);
    bool isButtonDown(int button);

    double getScrollY();
    double getScrollX();

    double getMouseY();
    double getMouseX();

    static std::shared_ptr<Input> getInstance();
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

    static std::shared_ptr<Input> instance;
};