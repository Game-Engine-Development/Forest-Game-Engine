#include "Headers/Engine/GUI/Quad.h"

Quad::Quad() = default;

Quad::Quad( //@todo make an interface for text and Texture to inherit from so that polymorphic pointers can be used
        Text &&staticTextPar,
        const glm::vec2 &position,
        const glm::vec2 &scale) : staticText(std::move(staticTextPar)), position(position), scale(scale) {

    usingText = true;

    createBuffers();
}

Quad::Quad(
        Texture&& texturePar,
        const glm::vec2 &position,
        const glm::vec2 &scale) : texture(std::move(texturePar)), position(position), scale(scale) {

    usingText = false;

    createBuffers();
}

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

    if(usingText) {
        staticText.bind(shader);
    }
    else {
        texture.bind(shader);
    }

    int posInt = glGetUniformLocation(shader.ID, "position");
    glUniform2fv(posInt, 1, glm::value_ptr(position));

    int scaleInt = glGetUniformLocation(shader.ID, "scale");
    glUniform2fv(scaleInt, 1, glm::value_ptr(scale));

    int offsetInt = glGetUniformLocation(shader.ID, "offset");
    glUniform2fv(offsetInt, 1, glm::value_ptr(offset));

    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);

    if(usingText) {
        Text::unbind();
    }
    else {
        texture.unbind();
    }

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

Quad& Quad::operator=(Quad &&quad) noexcept {
    usingText = quad.usingText;

    VAO = quad.VAO;
    quad.VAO = 0;
    VBO = quad.VBO;
    quad.VBO = 0;
    TBO = quad.TBO;
    quad.TBO = 0;
    IBO = quad.IBO;
    quad.IBO = 0;

    vertices = std::move(quad.vertices);
    textureCoords = std::move(quad.textureCoords);
    indices = std::move(quad.indices);

    position = quad.position;
    scale = quad.scale;
    offset = quad.offset;

    if(quad.usingText) {
        staticText = std::move(quad.staticText); //@todo make an interface for text and Texture to inherit from so that polymorphic pointers can be used
    }
    else {
        texture = std::move(quad.texture);
    }

    return *this;
}
