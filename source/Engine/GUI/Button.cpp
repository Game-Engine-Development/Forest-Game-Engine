#include "Headers/Engine/GUI/Button.h"

Button::Button() = default;
Button::Button(char *textureLocation, int type, glm::vec2 position, glm::vec2 scale, glm::vec2 rotation, void (*action)(), std::vector<glm::vec2> &&verts, std::vector<glm::vec2> &&texts, std::vector<unsigned int> &&inds) : texture(Texture(textureLocation, type, 0)), position(position), scale(scale), rotation(rotation), action(action), vertices(verts), textureCoords(texts), indices(inds) {
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
    createModelMatrix();

    shader.use();

    bindVAO();
    texture.bind(shader);

    int modelLoc = glGetUniformLocation(shader.ID, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMatrix));

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

void Button::createModelMatrix() {
    modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(position.x, position.y, 0));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(scale, 0));
}