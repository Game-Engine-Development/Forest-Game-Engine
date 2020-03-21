#include "Headers/Engine/IO/Window.h"

Window::Window(Camera *camera) {
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

    camera->setAspectRatio(width/height);

    std::cout << "width: " << width << std::endl;
    std::cout << "height: " << height << std::endl;

    window = glfwCreateWindow(static_cast<int>(width), static_cast<int>(height), "Forest", monitor, nullptr);
    if (window == nullptr) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        std::exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        std::exit(EXIT_FAILURE);
    }

    glfwSwapInterval(1);

    glEnable(GL_DEPTH_TEST);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // uncomment this call to draw in wireframe polygons.
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void Window::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.



    glViewport(0, 0, width, height);
}

GLFWmonitor* Window::getMonitor() {
    return monitor;
}
const GLFWvidmode* Window::getMode() {
    return mode;
}
GLFWwindow* Window::getWindow() {
    return window;
}

float Window::getWidth() {
    return width;
}
float Window::getHeight() {
    return height;
}
