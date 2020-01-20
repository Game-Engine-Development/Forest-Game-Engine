#include <array>
#include "Headers/Engine/GUI/Button.h"

Button::Button() = default;
Button::Button(char *textureLocation, glm::vec2 position, glm::vec2 scale, void (*action)(), GLFWwindow* window, std::vector<glm::vec2> &&verts, std::vector<glm::vec2> &&texts, std::vector<unsigned int> &&inds) : texture(Texture(textureLocation, 0)), position(position), scale(scale), action(action), window(window), vertices(verts), textureCoords(texts), indices(inds) {
    createBuffers();
    clampToScreen();
}

void Button::bindVAO() {
    glBindVertexArray(VAO);
}

void Button::unbindVAO() {
    glBindVertexArray(0);
}

void Button::createBuffers() {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &TBO);
    glGenBuffers(1, &IBO);
    bindVAO();

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec2), &vertices[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), nullptr);

    glBindBuffer(GL_ARRAY_BUFFER, TBO);
    glBufferData(GL_ARRAY_BUFFER, textureCoords.size() * sizeof(glm::vec2), &textureCoords[0], GL_STATIC_DRAW);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), nullptr);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    unbindVAO();
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Button::render(Shader &shader) {
    detectEdges();

    onClick();

    clampToScreen();

    shader.use();

    bindVAO();
    texture.bind(shader);

    int posInt = glGetUniformLocation(shader.ID, "position");
    glUniform2fv(posInt, 1, glm::value_ptr(position));

    int scaleInt = glGetUniformLocation(shader.ID, "scale");
    glUniform2fv(scaleInt, 1, glm::value_ptr(scale));

    int offsetInt = glGetUniformLocation(shader.ID, "offset");
    glUniform2fv(offsetInt, 1, glm::value_ptr(offset));

    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);

    texture.unbind();
    unbindVAO();
}

Button::~Button() {
    glDeleteBuffers(1, &IBO);
    glDeleteBuffers(1, &TBO);
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);
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

    for (const glm::vec2& vertex : vertices){
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
    offset.x = 0;
    offset.y = 0;

    std::array<bool, 4> offscreen{}; //x < -1.0f, x > 1.0f, y < -1.0f, y > 1.0f

    for (const glm::vec2& vertex : vertices){
        if((vertex.x*scale.x + position.x) < -1.0f){
            if(std::abs(-1.0f - (vertex.x*scale.x + position.x)) > std::abs(offset.x)){
                offset.x = -1.0f - (vertex.x * scale.x + position.x);
            }
        }
        else if((vertex.x*scale.x + position.x) > 1.0f){
            if(std::abs(1.0f - (vertex.x*scale.x + position.x)) > std::abs(offset.x)){
                offset.x = 1.0f - (vertex.x * scale.x + position.x);
            }
        }

        if((vertex.y*scale.y + position.y) < -1.0f){
            if(std::abs(-1.0f - (vertex.y*scale.y + position.y)) > std::abs(offset.y)){
                offset.y = -1.0f - (vertex.y*scale.y + position.y);
            }
        }
        else if((vertex.y*scale.y + position.y) > 1.0f){
            if(std::abs(1.0f - (vertex.y*scale.y + position.y)) > std::abs(offset.y)){
                offset.y = 1.0f - (vertex.y*scale.y + position.y);
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