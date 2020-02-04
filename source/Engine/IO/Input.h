#pragma once

#include <array>
#include <GLFW/glfw3.h>
#include <Headers/Engine/Camera.h>

class Input {
public:
    Input();
    Input(GLFWwindow *window, Camera *camera);

    void mouse_callback(GLFWwindow *window, double xpos, double ypos);
    void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
    void processInput(GLFWwindow *window);

    bool isKeyDown(int key);
    bool isButtonDown(int button);

    double getScrollY();
    void setScrollY(double scrollY);
    double getScrollX();
    void setScrollX(double scrollX);

    double getMouseY();
    void setMouseY(double mouseY);
    double getMouseX();
    void setMouseX(double mouseX);

    ~Input();
private:
    GLFWwindow *window;
    Camera *camera;

    std::array<bool, GLFW_KEY_LAST> m_keys{};
    std::array<bool, GLFW_MOUSE_BUTTON_LAST> m_buttons{};

    double m_mouseX{}, m_mouseY{};
    double m_scrollX{}, m_scrollY{};
};