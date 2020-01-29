#include <array>
#include "Headers/Engine/GUI/Button.h"

Button::Button() = default;
Button::Button(char *textureLocation, glm::vec2 position, glm::vec2 scale, std::function<void(void)> &action, GLFWwindow* window, std::vector<glm::vec2> &&verts, std::vector<glm::vec2> &&texts, std::vector<unsigned int> &&inds) : window(window), quad(Quad(Texture(textureLocation, 0), position, scale, verts, texts, inds)) {
    clampToScreen();
}

void Button::onClick() {
    //top y = 0
    //bottom y = 600

    //left x = 0
    //right x = 800

    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);

    //0 - 800 -> -1 - 1
    //operation 1: -400
    //-400 - 400 -> -1 - 1
    //operation 2: /400
    //-1 - 1 -> -1 - 1

    xpos -= 400.0;
    xpos /= 400.0;

    //0 - 600 -> 1 - -1
    //operation 1: *-1
    //0 - -600 -> 1 - -1
    //operation2: +300
    //300 - -300 -> 1 - -1
    //operation 3: /300
    //1 - -1 -> 1 - -1

    ypos *= -1.0;
    ypos += 300.0;
    ypos /= 300.0;

    bool xValid = xpos >= edges[0] && xpos <= edges[1];
    bool yValid = ypos >= edges[2] && ypos <= edges[3];

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

    if((xValid && yValid) && pressed) { //if button pressed
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

    glm::vec2 scale = quad.getScale();
    glm::vec2 position = quad.getPos();
    glm::vec2 offset = quad.getOffset();

    for (const glm::vec2& vertex : quad.getVertices()){
        if((vertex.x*scale.x + position.x + offset.x) < edges[0]){
            edges[0] = offset.x + (vertex.x * scale.x + position.x);
        }
        else if((vertex.x*scale.x + position.x + offset.x) > edges[1]){
            edges[1] = offset.x + (vertex.x * scale.x + position.x);
        }

        if((vertex.y*scale.y + position.y + offset.y) < edges[2]){
                edges[2] = offset.y + (vertex.y*scale.y + position.y);
        }
        else if((vertex.y*scale.y + position.y + offset.y) > edges[3]){
                edges[3] = offset.y + (vertex.y*scale.y + position.y);
        }
    }
}

void Button::clampToScreen() {
    quad.setOffsetX(0);
    quad.setOffsetY(0);

    glm::vec2 position = quad.getPos();
    glm::vec2 scale = quad.getScale();

    std::array<bool, 4> offscreen{}; //x < -1.0f, x > 1.0f, y < -1.0f, y > 1.0f

    for (const glm::vec2& vertex : quad.getVertices()){
        if((vertex.x*scale.x + position.x) < -1.0f){
            if(std::abs(-1.0f - (vertex.x*scale.x + position.x)) > std::abs(offset.x)){
                quad.setOffsetX(-1.0f - (vertex.x * scale.x + position.x));
            }
        }
        else if((vertex.x*scale.x + position.x) > 1.0f){
            if(std::abs(1.0f - (vertex.x*scale.x + position.x)) > std::abs(offset.x)){
                quad.setOffsetX(1.0f - (vertex.x * scale.x + position.x));
            }
        }

        if((vertex.y*scale.y + position.y) < -1.0f){
            if(std::abs(-1.0f - (vertex.y*scale.y + position.y)) > std::abs(offset.y)){
                quad.setOffsetY(-1.0f - (vertex.y*scale.y + position.y));
            }
        }
        else if((vertex.y*scale.y + position.y) > 1.0f){
            if(std::abs(1.0f - (vertex.y*scale.y + position.y)) > std::abs(offset.y)){
                quad.setOffsetY(1.0f - (vertex.y*scale.y + position.y));
            }
        }


        if((vertex.x*scale.x) < -1.0f) {
            offscreen[0] = true;
        }
        else if((vertex.x*scale.x) > 1.0f){
            offscreen[1] = true;
        }

        if((vertex.y*scale.y) < -1.0f){
            offscreen[2] = true;
        }
        else if((vertex.y*scale.y) > 1.0f){
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