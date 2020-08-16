#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <cstdlib>

#include "Headers/Engine/Camera/Camera.h"

class Window {
public:
    Window(const Window&) = delete;
    Window(Window&&) = delete;
    Window& operator=(const Window&) = delete;
    Window& operator=(Window&&) = delete;

    explicit Window(Camera &camera);

    [[nodiscard]] const GLFWmonitor* getMonitor() const noexcept;
    [[nodiscard]] const GLFWvidmode* getMode() const noexcept;
    [[nodiscard]] GLFWwindow* getWindow() const noexcept; //caution. returns pointer that is mutable. needed for opengl callbacks

    [[nodiscard]] float getWidth() const noexcept;
    [[nodiscard]] float getHeight() const noexcept;

    ~Window() noexcept;

private:
    static void framebuffer_size_callback(GLFWwindow* window, int width, int height) noexcept;

    GLFWmonitor *monitor;
    const GLFWvidmode *mode;
    GLFWwindow *window;

    float width;
    float height;
};