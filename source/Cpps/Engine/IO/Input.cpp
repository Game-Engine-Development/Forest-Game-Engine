#include "Headers/Engine/IO/Input.h"

std::unique_ptr<Input> Input::instance;

Input::Input(Window *const window, Camera *const camera, /*std::vector<Sphere> *spheres,*/ std::array<Terrain, 9> *terrains) {
    instance = std::make_unique<Input>();

    instance->m_window = window;
    instance->m_camera = camera;
    instance->terrains = terrains;

    instance->firstMouse = true;

    instance->lastX = window->getWidth();
    instance->lastY = window->getHeight();

    instance->m_keys.fill(false);
    instance->m_buttons.fill(false);

    glfwSetCursorPosCallback(window->getWindow(), mouse_callback);
    glfwSetScrollCallback(window->getWindow(), scroll_callback);
    glfwSetCursorEnterCallback(window->getWindow(), cursor_enter_callback);
}

void Input::processInputImpl(SoLoud::Soloud &gSoloud, SoLoud::Wav &gWave, Window &window, EnttWrapper::Scene &scene, int startIndex, int numberOfSpheres) { //@todo fix this code duplication and weird mutability
    for (int i = 0; i < GLFW_KEY_LAST; ++i) {
        instance->m_keys[i] = glfwGetKey(instance->m_window->getWindow(), i) == GLFW_PRESS;
    }
    for (int i = 0; i < GLFW_MOUSE_BUTTON_LAST; ++i) {
        instance->m_buttons[i] = glfwGetMouseButton(instance->m_window->getWindow(), i) == GLFW_PRESS;
    }

    if (instance->m_keys[GLFW_KEY_ESCAPE]) {
        glfwSetWindowShouldClose(instance->m_window->getWindow(), true);
    }

    if ((glfwGetKey(m_window->getWindow(), GLFW_KEY_P) == GLFW_PRESS) && gSoloud.getActiveVoiceCount() == 0) {
        screenshot("hello_screenshot.jpg", window);

        gSoloud.play(gWave); // Play the wave
    }

    const bool onlyOnePressed = (glfwGetKey(m_window->getWindow(), GLFW_KEY_W) == GLFW_PRESS) ^ (glfwGetKey(m_window->getWindow(), GLFW_KEY_S) == GLFW_PRESS);
    const bool onlyOnePressed2 = (glfwGetKey(m_window->getWindow(), GLFW_KEY_D) == GLFW_PRESS) ^ (glfwGetKey(m_window->getWindow(), GLFW_KEY_A) == GLFW_PRESS);

    const bool onlyOnePressed3 = (glfwGetKey(m_window->getWindow(), GLFW_KEY_UP) == GLFW_PRESS) ^ (glfwGetKey(m_window->getWindow(), GLFW_KEY_DOWN) == GLFW_PRESS);

    constexpr float SIDE_SPEED = 0.3f;
    constexpr float FORWARD_SPEED = 0.5f;
    constexpr float UP_SPEED = 0.3f;

    if(onlyOnePressed || onlyOnePressed2 || onlyOnePressed3) {
        glm::vec3 finalMove = m_camera->getFront();
        finalMove.y = 0;
        finalMove = glm::normalize(finalMove);
        if (onlyOnePressed) {
            if (glfwGetKey(m_window->getWindow(), GLFW_KEY_W) == GLFW_PRESS) {
                finalMove *= FORWARD_SPEED;
            }
            if (glfwGetKey(m_window->getWindow(), GLFW_KEY_S) == GLFW_PRESS) {
                finalMove *= -FORWARD_SPEED;
            }
        }
        else {
            finalMove *= 0.f; //do not remove
        }
        if (onlyOnePressed2) {
            if (glfwGetKey(m_window->getWindow(), GLFW_KEY_D) == GLFW_PRESS) {
                finalMove += SIDE_SPEED * m_camera->getRight();
            }
            if (glfwGetKey(m_window->getWindow(), GLFW_KEY_A) == GLFW_PRESS) {
                finalMove += -SIDE_SPEED * m_camera->getRight();
            }
        }
        if (onlyOnePressed3) {
            if (glfwGetKey(m_window->getWindow(), GLFW_KEY_UP) == GLFW_PRESS) {
                finalMove.y = UP_SPEED;
            }
            if (glfwGetKey(m_window->getWindow(), GLFW_KEY_DOWN) == GLFW_PRESS) {
                finalMove.y = -UP_SPEED;
            }
        }

        m_camera->setPosition(m_camera->getPosition() + finalMove);
    }

    raycastPickSphere(scene, startIndex, numberOfSpheres);

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

void Input::raycastPickSphere(EnttWrapper::Scene &scene, int startIndex, int numberOfSpheres) {
    // Note: could query if window has lost focus here
    if ((Input::isButtonDown(GLFW_MOUSE_BUTTON_LEFT) || Input::isButtonDown(GLFW_MOUSE_BUTTON_RIGHT)) && instance->cursor) {
        double xposLocal, yposLocal;
        glfwGetCursorPos(instance->m_window->getWindow(), &xposLocal, &yposLocal);
        // work out ray
        const glm::vec3 ray_wor = get_ray_from_mouse((*instance->m_window), (*instance->m_camera), (float)xposLocal, (float)yposLocal);
        //const glm::vec3 ray_wor = getRayFromScreenSpace(glm::vec2{(float)xposLocal, (float)yposLocal}, (*instance->m_camera), (*instance->m_window));
        // check ray against all spheres in scene
        int closest_sphere_clicked = -1;
        float closest_intersection = 0.0f;
        for (int i = startIndex; i < /*instance->spheres->size()*/ startIndex+numberOfSpheres; i++) {
            float t_dist = 0.0f;
            constexpr float sphere_radius = 1.0f; //temporary @todo change
            if (ray_sphere(instance->m_camera->getPos(), ray_wor, /*sphere_pos_wor[i]*/ /*instance->spheres->at(i).getPos()*/ scene.getEntity(scene.getEntities().at(i)).getComponent<Component::PosRotationScale>().getPos(), sphere_radius, t_dist)) {
                // if more than one sphere is in path of ray, only use the closest one
                if (-1 == closest_sphere_clicked || t_dist < closest_intersection) {
                    closest_sphere_clicked = i;
                    closest_intersection = t_dist;
                }
            }
        }
        instance->g_selected_sphere = closest_sphere_clicked;
        std::cout << "sphere " << closest_sphere_clicked << " was clicked\n";
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



    //std::cout << "xoffset: " << xoffset << '\n';
    //std::cout << "yoffset: " << yoffset << '\n';
}

int Input::get_g_selected_sphere() noexcept {
    return instance->g_selected_sphere;
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

bool Input::notCursor() noexcept {
    return !instance->cursor;
}

void Input::processInput(SoLoud::Soloud &gSoloud, SoLoud::Wav &gWave, Window &window, EnttWrapper::Scene &scene, int startIndex, int numberOfSpheres) {
    instance->processInputImpl(gSoloud, gWave, window, scene, startIndex, numberOfSpheres);
}
