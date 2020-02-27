#include "Headers/Engine/GUI/Quad.h"

Quad::Quad() = default;

Quad::Quad(Texture &&texture, glm::vec2 &position, glm::vec2 &scale, std::vector <glm::vec2> verts, std::vector <glm::vec2> texts, std::vector<unsigned int> inds) : texture(texture), position(position), scale(scale), vertices(verts), textureCoords(texts), indices(inds) {}

void Quad::bindVAO() {
    glBindVertexArray(VAO);
}

void Quad::unbindVAO() {
    glBindVertexArray(0);
}

void Quad::createBuffers() {
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

void Quad::render(Shader &shader) {
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

Quad::~Quad() {
    glDeleteBuffers(1, &IBO);
    glDeleteBuffers(1, &TBO);
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);
}

std::vector<glm::vec2> Quad::getVertices() {
    return vertices;
}

glm::vec2 Quad::getScale() {
    return scale;
}

glm::vec2 Quad::getPos() {
    return position;
}

glm::vec2 Quad::getOffset() {
    return offset;
}

void Quad::setOffsetX(float x) {
    offset.x = x;
}

void Quad::setOffsetY(float y) {
    offset.y = y;
}
