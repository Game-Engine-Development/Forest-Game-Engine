#include "Headers/Engine/IO/Input.h"

std::unique_ptr<Input> Input::instance;

Input::Input(Window *const window, Camera *const camera) noexcept {
    instance = std::make_unique<Input>();

    instance->m_window = window->getWindow();
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

void Input::processInputImpl(Player &player, SoLoud::Soloud &gSoloud, SoLoud::Wav &gWave, Window &window) noexcept { //@todo fix this code duplication and weird mutability
    for(int i = 0; i < GLFW_KEY_LAST; ++i) {
        instance->m_keys[i] = glfwGetKey(instance->m_window, i) == GLFW_PRESS;
    }
    for(int i = 0; i < GLFW_MOUSE_BUTTON_LAST; ++i) {
        instance->m_buttons[i] = glfwGetMouseButton(instance->m_window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;
    }

    if(instance->m_keys[GLFW_KEY_ESCAPE]) {
        glfwSetWindowShouldClose(instance->m_window, true);
    }

    if ((glfwGetKey(m_window, GLFW_KEY_P) == GLFW_PRESS) && gSoloud.getActiveVoiceCount() == 0) {
        screenshot("hello_screenshot.jpg", window);

        gSoloud.play(gWave); // Play the wave
    }

    constexpr float SPEED = 50.f;

    const float speedBoost = static_cast<float>((glfwGetKey(m_window, GLFW_KEY_B) == GLFW_PRESS)) * SPEED;

    const bool onlyOnePressed = (glfwGetKey(m_window, GLFW_KEY_W) == GLFW_PRESS) ^ (glfwGetKey(m_window, GLFW_KEY_S) == GLFW_PRESS);
    if(onlyOnePressed) {
        if (glfwGetKey(m_window, GLFW_KEY_W) == GLFW_PRESS) {
            player.setSpeed(Player::SPEED + speedBoost);
        } if (glfwGetKey(m_window, GLFW_KEY_S) == GLFW_PRESS) {
            player.setSpeed(-Player::SPEED - speedBoost);
        }
    } else {
        player.setSpeed(0);
    }

    const bool onlyOnePressed2 = (glfwGetKey(m_window, GLFW_KEY_D) == GLFW_PRESS) ^ (glfwGetKey(m_window, GLFW_KEY_A) == GLFW_PRESS);
    if(onlyOnePressed2) {
        if (glfwGetKey(m_window, GLFW_KEY_D) == GLFW_PRESS) {
            player.setLateralSpeed(Player::LATERAL_SPEED + speedBoost);
        } if (glfwGetKey(m_window, GLFW_KEY_A) == GLFW_PRESS) {
            player.setLateralSpeed(-Player::LATERAL_SPEED - speedBoost);
        }
    } else {
        player.setLateralSpeed(0);
    }

    if(glfwGetKey(m_window, GLFW_KEY_SPACE) == GLFW_PRESS && !player.isInAir()) {
        player.jump();
    }
    if(glfwGetKey(m_window, GLFW_KEY_C) == GLFW_PRESS) {
        if(cursor && !held) {
            cursor = false;
            held = true;
            glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        } else if(!held) {
            cursor = true;
            held = true;
            glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
    } else {
        held = false;
    }
    if(glfwGetMouseButton(m_window, GLFW_MOUSE_BUTTON_LEFT)) {
        if(!cursorHeld) {
            cursorHeld = true;
            shouldShoot = true;
        }
    } else {
        cursorHeld = false;
    }
}

void Input::mouse_callback([[maybe_unused]] GLFWwindow* window, double xpos, double ypos) {
    if(instance->firstMouse){
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

bool Input::isKeyDown(int key) noexcept {
    return instance->m_keys[key];
}
bool Input::isButtonDown(int button) noexcept {
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

bool Input::isShouldShoot() noexcept {
    return instance->shouldShoot;
}

void Input::setShouldShoot(bool value) noexcept {
    instance->shouldShoot = value;
}

void Input::processInput(Player &player, SoLoud::Soloud &gSoloud, SoLoud::Wav &gWave, Window &window) noexcept {
    instance->processInputImpl(player, gSoloud, gWave, window);
}
