#include "Headers/Engine/GUI/Button.h"

Button::Button() = default;
Button::Button(char *textureLocation, int type, glm::vec2 position, glm::vec2 scale, void (*action)()) : texture(Texture(textureLocation, type, 0)), position(position), scale(scale), action(action) {}

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

void Button::render(Camera &camera, Shader &shader) {
    camera.setMatrices(shader);

    glDrawArrays(GL_TRIANGLES, 0, 2);
}