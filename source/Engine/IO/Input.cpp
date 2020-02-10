#include "Headers/Engine/IO/Input.h"

std::shared_ptr<Input> Input::instance;

void Input::Init(GLFWwindow *window, Camera *camera, const GLFWvidmode *mode) {
    instance = std::make_shared<Input>();

    instance->m_window = window;
    instance->m_camera = camera;

    instance->firstMouse = true;

    instance->lastX = mode->width/2;
    instance->lastY = mode->height/2;

    instance->m_keys.fill(false);
    instance->m_buttons.fill(false);

    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetCursorEnterCallback(window, cursor_enter_callback);
}

void Input::processInput() {
    for(int i = 0; i < GLFW_KEY_LAST; ++i) {
        instance->m_keys[i] = glfwGetKey(instance->m_window, i) == GLFW_PRESS;
    }
    for(int i = 0; i < GLFW_MOUSE_BUTTON_LAST; ++i) {
        instance->m_buttons[i] = glfwGetMouseButton(instance->m_window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;
    }

    if(instance->m_keys[GLFW_KEY_ESCAPE]) {
        glfwSetWindowShouldClose(instance->m_window, true);
    }
}

void Input::mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    if(instance->firstMouse){
        instance->lastX = xpos;
        instance->lastY = ypos;
        instance->firstMouse = false;
    }
    float xoffset = xpos - instance->lastX;
    float yoffset = instance->lastY - ypos;
    instance->lastX = xpos;
    instance->lastY = ypos;

    float sensitivity = 0.05f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    instance->m_camera->ProcessMouseMovement(xoffset, yoffset, true);
}

void Input::scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    instance->m_camera->ProcessMouseScroll(yoffset);
}

void Input::cursor_enter_callback(GLFWwindow* window, int entered) {}

bool Input::isKeyDown(int key) {
    return instance->m_keys[key];
}
bool Input::isButtonDown(int button) {
    return instance->m_buttons[button];
}

double Input::getScrollY() {
    return instance->m_scrollY;
}
double Input::getScrollX() {
    return instance->m_scrollX;
}

double Input::getMouseY() {
    return instance->lastY;
}
double Input::getMouseX() {
    return instance->lastX;
}

std::shared_ptr<Input> Input::getInstance() {
    return instance;
}