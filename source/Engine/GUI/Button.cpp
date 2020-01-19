#include <array>
#include "Headers/Engine/GUI/Button.h"

Button::Button() = default;
Button::Button(char *textureLocation, int type, glm::vec2 position, glm::vec2 scale, void (*action)(), std::vector<glm::vec2> &&verts, std::vector<glm::vec2> &&texts, std::vector<unsigned int> &&inds) : texture(Texture(textureLocation, type, 0)), position(position), scale(scale), action(action), vertices(verts), textureCoords(texts), indices(inds) {
    createBuffers();
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
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);

    glBindBuffer(GL_ARRAY_BUFFER, TBO);
    glBufferData(GL_ARRAY_BUFFER, textureCoords.size() * sizeof(glm::vec2), &textureCoords[0], GL_STATIC_DRAW);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    unbindVAO();
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Button::render(Shader &shader) {
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

    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

    texture.unbind();
    unbindVAO();
}

Button::~Button() {
    glDeleteBuffers(1, &IBO);
    glDeleteBuffers(1, &TBO);
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);
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
        if((vertex.x*scale.x + position.x) > 1.0f){
            if(std::abs(1.0f - (vertex.x*scale.x + position.x)) > std::abs(offset.x)){
                offset.x = 1.0f - (vertex.x * scale.x + position.x);
            }
        }

        if((vertex.y*scale.y + position.y) < -1.0f){
            if(std::abs(-1.0f - (vertex.y*scale.y + position.y)) > std::abs(offset.y)){
                offset.y = -1.0f - (vertex.y*scale.y + position.y);
            }
        }
        if((vertex.y*scale.y + position.y) > 1.0f){
            if(std::abs(1.0f - (vertex.y*scale.y + position.y)) > std::abs(offset.y)){
                offset.y = 1.0f - (vertex.y*scale.y + position.y);
            }
        }


        if((vertex.x*scale.x) < -1.0f) {
            offscreen[0] = true;
        }
        if((vertex.x*scale.x) > 1.0f){
            offscreen[1] = true;
        }

        if((vertex.y*scale.y) < -1.0f){
            offscreen[2] = true;
        }
        if((vertex.y*scale.y) > 1.0f){
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