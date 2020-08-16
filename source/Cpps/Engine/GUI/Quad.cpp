#include "Headers/Engine/GUI/Quad.h"

Quad::Quad(
        std::variant<Text, Texture> textureParam,
        const glm::vec2 &position,
        const glm::vec2 &scale,
        bool isTransparentPar,
        bool hasBackgroundImg,
        std::optional<Texture> backgroundImg) noexcept
        : texture(std::move(textureParam)), position(position), scale(scale),
          backgroundTexture(std::move(backgroundImg)),
          isTransparent(isTransparentPar), hasBackgroundImg(hasBackgroundImg)
{
    if(!quadBuffers.has_value()) {
        createBuffers();
    }
}

void Quad::bindVAO() const noexcept {
    glBindVertexArray(VAOCache);
}

void Quad::unbindVAO() noexcept {
    glBindVertexArray(0);
}

void Quad::createBuffers() noexcept {
    unsigned int VAO{}, VBO{}, TBO{}, IBO{};

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &TBO);
    glGenBuffers(1, &IBO);
    glBindVertexArray(VAO);

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

    quadBuffers = std::optional(QuadResourceContainer(VAO, VBO, TBO, IBO));
    VAOCache = quadBuffers->getVAO();
}

void Quad::render(const Shader &shader) {
    shader.use();

    bindVAO();

    std::visit([&](auto&& arg){arg.bind(shader);}, texture);

    if(hasBackgroundImg) backgroundTexture->bind(shader);

    const int posInt = glGetUniformLocation(shader.ID, "position");
    glUniform2fv(posInt, 1, glm::value_ptr(position));

    const int scaleInt = glGetUniformLocation(shader.ID, "scale");
    glUniform2fv(scaleInt, 1, glm::value_ptr(scale));

    const int offsetInt = glGetUniformLocation(shader.ID, "offset");
    glUniform2fv(offsetInt, 1, glm::value_ptr(offset));

    const int isTransparentInt = glGetUniformLocation(shader.ID, "isTransparent");
    glUniform1i(isTransparentInt, isTransparent);

    const int hasBackgroundInt = glGetUniformLocation(shader.ID, "backgroundImg");
    glUniform1i(hasBackgroundInt, hasBackgroundImg);

    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);

    std::visit([&](auto&& arg){arg.unbind();}, texture);

    unbindVAO();
}

[[nodiscard]] std::array<glm::vec2, 4> Quad::getVertices() noexcept {
    return vertices;
}

[[nodiscard]] glm::vec2 Quad::getScale() const noexcept {
    return scale;
}

[[nodiscard]] glm::vec2 Quad::getPos() const noexcept {
    return position;
}

[[nodiscard]] glm::vec2 Quad::getOffset() const noexcept {
    return offset;
}

void Quad::setOffset(const glm::vec2 newOffset) noexcept {
    offset = newOffset;
}
