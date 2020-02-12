#pragma once

#include "Headers/Engine/IO/Input.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <Headers/Engine/Camera.h>
#include <cstdlib>

class Window {
public:
    Window(Camera *camera);

    GLFWmonitor* getMonitor();
    const GLFWvidmode* getMode();
    GLFWwindow* getWindow();

private:
    static void framebuffer_size_callback(GLFWwindow* window, int width, int height);

    GLFWmonitor *monitor;
    const GLFWvidmode *mode;
    GLFWwindow *window;
};