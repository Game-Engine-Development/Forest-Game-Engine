#include "Headers/Engine/IO/Input.h"

Input::Input(Window *const window, Camera *const camera) {
    instance->m_window = window;
    instance->m_camera = camera;

    instance->firstMouse = true;

    instance->lastX = window->getWidth();
    instance->lastY = window->getHeight();

    instance->m_keys.fill(false);
    instance->m_buttons.fill(false);

    glfwSetCursorPosCallback(window->getWindow(), mouse_callback);
    glfwSetScrollCallback(window->getWindow(), scroll_callback);
    glfwSetCursorEnterCallback(window->getWindow(), cursor_enter_callback);
}

void Input::processInputImpl() {
    for (int i = 0; i < GLFW_KEY_LAST; ++i) {
        instance->m_keys[i] = glfwGetKey(instance->m_window->getWindow(), i) == GLFW_PRESS;
    }
    for (int i = 0; i < GLFW_MOUSE_BUTTON_LAST; ++i) {
        instance->m_buttons[i] = glfwGetMouseButton(instance->m_window->getWindow(), i) == GLFW_PRESS;
    }

    if (instance->m_keys[GLFW_KEY_ESCAPE]) {
        glfwSetWindowShouldClose(instance->m_window->getWindow(), true);
    }


    if(glfwGetKey(m_window->getWindow(), GLFW_KEY_C) == GLFW_PRESS) {
        if(cursor && !held) {
            cursor = false;
            held = true;
            glfwSetInputMode(m_window->getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        } else if(!held) {
            cursor = true;
            held = true;
            glfwSetInputMode(m_window->getWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
    } else {
        held = false;
    }
}

void Input::mouse_callback([[maybe_unused]] GLFWwindow* window, const double xpos, const double ypos) {
    if (instance->firstMouse) {
        instance->lastX = static_cast<float>(xpos);
        instance->lastY = static_cast<float>(ypos);
        instance->firstMouse = false;
    }

    constexpr float sensitivity = 0.05f;
    const auto xoffset = static_cast<float>(xpos - instance->lastX) * sensitivity;
    const auto yoffset = static_cast<float>(instance->lastY - ypos) * sensitivity;
    instance->lastX = static_cast<float>(xpos);
    instance->lastY = static_cast<float>(ypos);

    instance->m_camera->ProcessMouseMovement(xoffset, yoffset, true);
}

void Input::scroll_callback([[maybe_unused]] GLFWwindow* window, double xoffset, double yoffset) {
    instance->m_camera->ProcessMouseScroll(static_cast<float>(yoffset));
}

void Input::cursor_enter_callback([[maybe_unused]] GLFWwindow* window, int entered) {}

bool Input::isKeyDown(const int key) noexcept {
    return instance->m_keys[key];
}
bool Input::isButtonDown(const int button) noexcept {
    return instance->m_buttons[button];
}

double Input::getScrollY() noexcept {
    return instance->m_scrollY;
}
double Input::getScrollX() noexcept {
    return instance->m_scrollX;
}

double Input::getMouseY() noexcept {
    return instance->lastY;
}
double Input::getMouseX() noexcept {
    return instance->lastX;
}

bool Input::getCursor() noexcept {
    return instance->cursor;
}

void Input::processInput() {
    instance->processInputImpl();
}
