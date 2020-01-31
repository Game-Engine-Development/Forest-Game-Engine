#include "Headers/Engine/GUI/Button.h"

Button::Button() = default;
Button::Button(char *textureLocation, glm::vec2 position, glm::vec2 scale, std::function<void(void)> action, GLFWwindow *window, std::vector<glm::vec2> &&verts, std::vector<glm::vec2> &&texts, std::vector<unsigned int> &&inds) : window(window), mode(mode), quad(Quad(Texture(textureLocation, 0), position, scale, verts, texts, inds)) {
    clampToScreen();
}

void Button::onClick() {
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);

    int centerX, centerY;
    glfwGetWindowPos(window, &centerX, &centerY);

    /*int scaleX, scaleY;
    glfwGetWindowSize(window, &scaleX, &scaleY);*/

    //std::cout << "xPos: " << xpos << ", yPos: " << ypos << std::endl;
    //std::cout << "screenX: " << centerX << ", screenY: " << centerY << std::endl;

    int state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);

    if(notPressed && (state == GLFW_PRESS)) {
        notPressed = false;
        pressed = true;
    }
    else if(pressed && (state == GLFW_PRESS)) {
        pressed = false;
    }
    else if(state != GLFW_PRESS) {
        pressed = false;
        notPressed = true;
    }

    bool xValid = xpos >= edges[0] && xpos <= edges[1];
    bool yValid = ypos >= edges[2] && ypos <= edges[3];
    if((xValid && yValid) && pressed) { //if button pressed
        std::cout << "pressed" << std::endl;
        if(action != nullptr) {
            action();
        }
        else {
            std::cout << "no action given for button" << std::endl;
        }
    }
}

void Button::detectEdges() {
    edges = {1.0, -1.0, 1.0, -1.0};

    for (const glm::vec2& vertex : quad.getVertices()){
        if((vertex.x*quad.getScale().x + quad.getPos().x + quad.getOffset().x) < edges[0]){
            edges[0] = quad.getOffset().x + (vertex.x * quad.getScale().x + quad.getPos().x);
        }
        else if((vertex.x*quad.getScale().x + quad.getPos().x + quad.getOffset().x) > edges[1]){
            edges[1] = quad.getOffset().x + (vertex.x * quad.getScale().x + quad.getPos().x);
        }

        if((vertex.y*quad.getScale().y + quad.getPos().y + quad.getOffset().y) < edges[2]){
                edges[2] = quad.getOffset().y + (vertex.y*quad.getScale().y + quad.getPos().y);
        }
        else if((vertex.y*quad.getScale().y + quad.getPos().y + quad.getOffset().y) > edges[3]){
                edges[3] = quad.getOffset().y + (vertex.y*quad.getScale().y + quad.getPos().y);
        }
    }
}

void Button::clampToScreen() {
    quad.setOffsetX(0);
    quad.setOffsetY(0);

    std::array<bool, 4> offscreen{}; //x < -1.0f, x > 1.0f, y < -1.0f, y > 1.0f

    for (const glm::vec2& vertex : quad.getVertices()){
        if((vertex.x*quad.getScale().x + quad.getPos().x) < -1.0f){
            if(std::abs(-1.0f - (vertex.x*quad.getScale().x + quad.getPos().x)) > std::abs(quad.getOffset().x)){
                quad.setOffsetX(-1.0f - (vertex.x * quad.getScale().x + quad.getPos().x));
            }
        }
        else if((vertex.x*quad.getScale().x + quad.getPos().x) > 1.0f){
            if(std::abs(1.0f - (vertex.x*quad.getScale().x + quad.getPos().x)) > std::abs(quad.getOffset().x)){
                quad.setOffsetX(1.0f - (vertex.x * quad.getScale().x + quad.getPos().x));
            }
        }

        if((vertex.y*quad.getScale().y + quad.getPos().y) < -1.0f){
            if(std::abs(-1.0f - (vertex.y*quad.getScale().y + quad.getPos().y)) > std::abs(quad.getOffset().y)){
                quad.setOffsetY(-1.0f - (vertex.y*quad.getScale().y + quad.getPos().y));
            }
        }
        else if((vertex.y*quad.getScale().y + quad.getPos().y) > 1.0f){
            if(std::abs(1.0f - (vertex.y*quad.getScale().y + quad.getPos().y)) > std::abs(quad.getOffset().y)){
                quad.setOffsetY(1.0f - (vertex.y*quad.getScale().y + quad.getPos().y));
            }
        }


        if((vertex.x*quad.getScale().x) < -1.0f) {
            offscreen[0] = true;
        }
        else if((vertex.x*quad.getScale().x) > 1.0f){
            offscreen[1] = true;
        }

        if((vertex.y*quad.getScale().y) < -1.0f){
            offscreen[2] = true;
        }
        else if((vertex.y*quad.getScale().y) > 1.0f){
            offscreen[3] = true;
        }
    }

    if(offscreen[0] && offscreen[1]){
        std::cerr << "scale is too big on the x axis" << std::endl;
        return;
    }

    if(offscreen[2] && offscreen[3]){
        std::cerr << "scale is too big on the y axis" << std::endl;
        return;
    }
}

void Button::render(Shader &shader) {
    detectEdges();

    onClick();

    clampToScreen();

    quad.render(shader);
}