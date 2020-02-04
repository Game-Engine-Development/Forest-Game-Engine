#include "Input.h"

Input::Input() = default;
Input::Input(GLFWwindow *window, Camera *camera) : window(window), camera(camera) {


    m_keys.fill(false);
    m_buttons.fill(false);
}

void Input::mouse_callback(GLFWwindow* window, double xpos, double ypos) {

}
void Input::scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    camera->ProcessMouseScroll(yoffset);
}
void Input::processInput(GLFWwindow *window) {
    for(int i = 0; i < GLFW_KEY_LAST; ++i) {
        m_keys[i] = glfwGetKey(window, i) == GLFW_PRESS;
    }
    for(int i = 0; i < GLFW_MOUSE_BUTTON_LAST; ++i) {
        m_buttons[i] = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;
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
void Input::setScrollY(double scrollY) {
    m_scrollY = scrollY;
}
double Input::getScrollX() {
    return m_scrollX;
}
void Input::setScrollX(double scrollX) {
    m_scrollX = scrollX;
}

double Input::getMouseY() {
    return m_mouseY;
}
void Input::setMouseY(double mouseY) {
    m_mouseY = mouseY;
}
double Input::getMouseX() {
    return m_mouseX;
}
void Input::setMouseX(double mouseX) {
    m_mouseX = mouseX;
}

Input::~Input() = default;