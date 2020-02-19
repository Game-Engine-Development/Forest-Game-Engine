#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <Headers/Engine/Camera.h>
#include <cstdlib>

class Window {
public:
    explicit Window(Camera *camera);

    GLFWmonitor* getMonitor();
    const GLFWvidmode* getMode();
    GLFWwindow* getWindow();

    float getWidth();
    float getHeight();

private:
    static void framebuffer_size_callback(GLFWwindow* window, int width, int height);

    GLFWmonitor *monitor;
    const GLFWvidmode *mode;
    GLFWwindow *window;

    float width;
    float height;
};