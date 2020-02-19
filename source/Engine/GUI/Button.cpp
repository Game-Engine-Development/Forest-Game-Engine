#include "Headers/Engine/GUI/Button.h"

#include <utility>

Button::Button() = default;
Button::Button(char *textureLocation, glm::vec2 position, glm::vec2 scale, std::function<void(void)> action, Window *window, std::vector<glm::vec2> &&verts, std::vector<glm::vec2> &&texts, std::vector<unsigned int> &&inds) : window(window), quad(Quad(Texture(textureLocation, 0), position, scale, verts, texts, inds)), action(std::move(action)) {
    clampToScreen();
}

void Button::onClick() {
    double xpos = Input::getInstance()->getMouseX();
    double ypos = Input::getInstance()->getMouseY();

    int leftButtonState = Input::getInstance()->isButtonDown(GLFW_MOUSE_BUTTON_LEFT);

    if(notPressed && (leftButtonState == GLFW_PRESS)) {
        notPressed = false;
        pressed = true;
    }
    else if(pressed && (leftButtonState == GLFW_PRESS)) {
        pressed = false;
    }
    else if(leftButtonState != GLFW_PRESS) {
        pressed = false;
        notPressed = true;
    }

    assert(mathRound(edges[1] - quad.getScale().x - edges[0]) == 0.0f);
    assert(mathRound(edges[3] - quad.getScale().y - edges[2]) == 0.0f);

    edges[0] += 1.0f;
    edges[1] += 1.0f;

    edges[0] /= 2.0f;
    edges[1] /= 2.0f;

    edges[0] *= window->getWidth();
    edges[1] *= window->getWidth();

    ypos -= window->getHeight();
    ypos *= -1.0f;

    edges[2] += 1.0f;
    edges[3] += 1.0f;

    edges[2] /= 2.0f;
    edges[3] /= 2.0f;

    edges[2] *= window->getHeight();
    edges[3] *= window->getHeight();

    bool xValid = xpos >= edges[0] && xpos <= edges[1];
    bool yValid = ypos >= edges[2] && ypos <= edges[3];

    if((xValid && yValid) && pressed) { //if button pressed
        std::cout << "pressed" << std::endl;
        if(action != nullptr) {
            std::cout << "action given" << std::endl;
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
        if(mathRound(vertex.x*quad.getScale().x + quad.getPos().x + quad.getOffset().x) < edges[0]){
            //std::cout << "0: new: " << mathRound(vertex.x*quad.getScale().x + quad.getPos().x + quad.getOffset().x) << ", old: " << mathRound(edges[0]) << std::endl;
            assert(mathRound(vertex.x*quad.getScale().x + quad.getPos().x + quad.getOffset().x) != mathRound(edges[0]));
            edges[0] = mathRound(quad.getOffset().x + (vertex.x * quad.getScale().x + quad.getPos().x));
        }
        if(mathRound(vertex.x*quad.getScale().x + quad.getPos().x + quad.getOffset().x) > edges[1]){
            //std::cout << "1: new: " << mathRound(vertex.x*quad.getScale().x + quad.getPos().x + quad.getOffset().x) << ", old: " << mathRound(edges[1]) << std::endl;
            assert(mathRound(vertex.x*quad.getScale().x + quad.getPos().x + quad.getOffset().x) != mathRound(edges[1]));
            edges[1] = mathRound(quad.getOffset().x + (vertex.x * quad.getScale().x + quad.getPos().x));
        }

        if(mathRound(vertex.y*quad.getScale().y + quad.getPos().y + quad.getOffset().y) < edges[2]){
            //std::cout << "2: new: " << mathRound(vertex.y*quad.getScale().y + quad.getPos().y + quad.getOffset().y) << ", old: " << mathRound(edges[2]) << std::endl;
            assert(mathRound(vertex.y*quad.getScale().y + quad.getPos().y + quad.getOffset().y) != mathRound(edges[2]));
            edges[2] = mathRound(quad.getOffset().y + (vertex.y*quad.getScale().y + quad.getPos().y));
        }
        if(mathRound(vertex.y*quad.getScale().y + quad.getPos().y + quad.getOffset().y) > edges[3]){
            //std::cout << "3: new: " << mathRound(vertex.y*quad.getScale().y + quad.getPos().y + quad.getOffset().y) << ", old: " << mathRound(edges[3]) << std::endl;
            assert(mathRound(vertex.y*quad.getScale().y + quad.getPos().y + quad.getOffset().y) != mathRound(edges[3]));
            edges[3] = mathRound(quad.getOffset().y + (vertex.y*quad.getScale().y + quad.getPos().y));
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