#include "Input.h"

void Input::Init(GLFWwindow *window, Camera *camera, const GLFWvidmode *mode) {
    m_window = window;
    m_camera = camera;

    firstMouse = true;

    lastX = mode->width/2;
    lastY = mode->height/2;

    m_keys.fill(false);
    m_buttons.fill(false);

    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetCursorEnterCallback(window, cursor_enter_callback);
}

void Input::processInput() {
    for(int i = 0; i < GLFW_KEY_LAST; ++i) {
        m_keys[i] = glfwGetKey(m_window, i) == GLFW_PRESS;
    }
    for(int i = 0; i < GLFW_MOUSE_BUTTON_LAST; ++i) {
        m_buttons[i] = glfwGetMouseButton(m_window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;
    }
}

void Input::mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    std::cout << "mouse moved" << std::endl;

    if(firstMouse){
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    std::cout << "xPos: " << xpos << ", yPos: " << ypos << std::endl;

    float sensitivity = 0.05f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    m_camera->ProcessMouseMovement(xoffset, yoffset, true);
}

void Input::scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    std::cout << "scroll" << std::endl;
    m_camera->ProcessMouseScroll(yoffset);
}

void Input::cursor_enter_callback(GLFWwindow* window, int entered)
{
    if (entered)
    {
        std::cout << "in window" << std::endl;
    }
    else
    {
        std::cout << "not" << std::endl;
    }
}

bool Input::isKeyDown(int key) {
    return m_keys[key];
}
bool Input::isButtonDown(int button) {
    return m_buttons[button];
}

double Input::getScrollY() {
    return m_scrollY;
}
double Input::getScrollX() {
    return m_scrollX;
}

double Input::getMouseY() {
    return lastY;
}
double Input::getMouseX() {
    return lastX;
}