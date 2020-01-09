#include "Headers/Engine/GUI/Button.h"

Button::Button() = default;
Button::Button(char *textureLocation, int type, glm::vec2 position, glm::vec2 scale, void (*action)(), std::vector<glm::vec2> verts, std::vector<glm::vec2> texts, std::vector<glm::vec2> inds) : texture(Texture(textureLocation, type, 0)), position(position), scale(scale), action(action) {
    createBuffers();
}

void Button::setTexture(char *textureLocation, int type) {
    texture = Texture(textureLocation, type, 0);
}
void Button::setTexture(Texture newTexture) {
    texture = newTexture;
}
void Button::setPos(glm::vec2 pos) {
    position = pos;
}
void Button::setScale(glm::vec2 newScale) {
    scale = newScale;
}
void Button::setAction(void (*newAction)()) {
    action = newAction;
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
    glGenBuffers(1, &texCoordBuffer);
    glGenBuffers(1, &IBO);
    bindVAO();

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec2), &vertices[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);

    glBindBuffer(GL_ARRAY_BUFFER, texCoordBuffer);
    glBufferData(GL_ARRAY_BUFFER, textureCoords.size() * sizeof(glm::vec2), &textureCoords[0], GL_STATIC_DRAW);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);

    glBindBuffer(GL_ARRAY_BUFFER, IBO);
    glBufferData(GL_ARRAY_BUFFER, indices.size() * sizeof(glm::vec2), &indices[0], GL_STATIC_DRAW);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    unbindVAO();
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Button::render(Camera &camera, Shader &shader) {
    shader.use();

    bindVAO();
    texture.bind(shader);

    camera.setMatrices(shader);

    int modelLoc = glGetUniformLocation(shader.ID, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    glDrawArrays(GL_TRIANGLES, 0, vertices.size());

    texture.unbind();
    unbindVAO();
}

Button::~Button() {
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &texCoordBuffer);
    glDeleteVertexArrays(1, &VAO);
    glDeleteVertexArrays(1, &IBO);
}

glm::mat4 Button::createModelMatrix() {
    modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(position, 0));
    modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation.x), glm::vec3(1,0,0));
    modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation.y), glm::vec3(0,1,0));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(scale, 0));
    return modelMatrix;
}