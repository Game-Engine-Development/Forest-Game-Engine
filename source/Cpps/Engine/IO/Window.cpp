#include "Headers/Engine/IO/Window.h"

Window::Window(Camera &camera) {
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif
    // glfw window creation
    // --------------------
    monitor = glfwGetPrimaryMonitor();
    mode = (glfwGetVideoMode(monitor));

    glfwWindowHint(GLFW_RED_BITS, mode->redBits);
    glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
    glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
    glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

    width = static_cast<float>(mode->width);
    height = static_cast<float>(mode->height);

    camera.setAspectRatio(width/height);

    std::cout << "width: " << width << std::endl;
    std::cout << "height: " << height << std::endl;

    window = glfwCreateWindow(static_cast<int>(width), static_cast<int>(height), "Forest", monitor, nullptr);
    if (window == nullptr) {
        std::cerr << "Failed to create GLFW window" << std::endl;
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
        glfwTerminate();
#endif
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
    }

    glfwSwapInterval(1); //vsync

    glEnable(GL_DEPTH_TEST);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); //GLFW_CURSOR_NORMAL);

    // uncomment this call to draw in wireframe polygons.
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void Window::framebuffer_size_callback([[maybe_unused]] GLFWwindow* window, int width, int height) noexcept {
    //window is "maybe_unused" because we don't use it, but it is needed for the callback to work
    //@todo fix resizing window

    glViewport(0, 0, width, height);
}

[[nodiscard]] const GLFWmonitor* Window::getMonitor() const noexcept {
    return monitor;
}
[[nodiscard]] const GLFWvidmode* Window::getMode() const noexcept {
    return mode;
}
[[nodiscard]] GLFWwindow* Window::getWindow() const noexcept {
    return window;
}

[[nodiscard]] float Window::getWidth() const noexcept {
    return width;
}
[[nodiscard]] float Window::getHeight() const noexcept {
    return height;
}

Window::~Window() noexcept {
    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwDestroyWindow(window);
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
    glfwTerminate();
#endif
}
