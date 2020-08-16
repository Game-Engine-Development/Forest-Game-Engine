#include "Headers/Engine/GUI/Button.h"

Button::Button(
        std::variant<Text, Texture> textureParam,
        glm::vec2 position,
        glm::vec2 scale,
        std::function<void(void)> action,
        const Window *const window,
        bool isTransparentPar,
        bool hasBackgroundImg,
        std::optional<Texture> backgroundImg) noexcept : window(window), action(std::move(action)),
        quad(std::move(textureParam), position, scale, isTransparentPar, hasBackgroundImg, std::move(backgroundImg))
{
    clampToScreen();
}

void Button::onClick() noexcept {
    const double xpos = Input::getMouseX();
    const double ypos = -(Input::getMouseY() - window->getHeight());

    const int leftButtonState = Input::isButtonDown(GLFW_MOUSE_BUTTON_LEFT);

    pressed = (notPressed && (leftButtonState == GLFW_PRESS));
    notPressed = (leftButtonState != GLFW_PRESS);

    for(int i = 0; i < edges.size(); ++i) {
        edges[i] += 1.0f;
        edges[i] /= 2.0f;

        edges[i] *= (i < edges.size()/2) ? window->getWidth() : window->getHeight();
    }

    const bool xValid = xpos >= edges[0] && xpos <= edges[1];
    const bool yValid = ypos >= edges[2] && ypos <= edges[3];

    if((xValid && yValid) && pressed) { //if button pressed
        if(action != nullptr) {
            action();
        }
    }
}

void Button::detectEdges() noexcept {
    edges = {1.0, -1.0, 1.0, -1.0};

    for (const glm::vec2 vertex : quad.getVertices()){
        const float cursorEdgeX = mathRound(vertex.x*quad.getScale().x + quad.getPos().x + quad.getOffset().x);
        edges[0] = std::clamp(edges[0], edges[0], cursorEdgeX);
        edges[1] = std::clamp(edges[1], cursorEdgeX, edges[1]);

        const float cursorEdgeY = mathRound(vertex.y*quad.getScale().y + quad.getPos().y + quad.getOffset().y);
        edges[2] = std::clamp(edges[2], edges[2], cursorEdgeY);
        edges[3] = std::clamp(edges[3], cursorEdgeY, edges[3]);
    }
}

void Button::clampToScreen() {
    quad.setOffset(glm::vec2(0));

    offscreen.fill(false);

    for (const glm::vec2 vertex : quad.getVertices()){
        const glm::vec2 cursorEdge = vertex*quad.getScale() + quad.getPos();
        quad.setOffset(clampSides(cursorEdge, quad.getOffset()));

        const glm::vec2 scale = vertex*quad.getScale();
        offscreen[0] |= (scale.x < -1.0f);
        offscreen[1] |= (scale.x > 1.0f);

        offscreen[2] |= (scale.y < -1.0f);
        offscreen[3] |= (scale.y > 1.0f);
    }

    if(offscreen[0] && offscreen[1]){
        std::cerr << "scale is too big on the x axis" << std::endl;
    }
    if(offscreen[2] && offscreen[3]){
        std::cerr << "scale is too big on the y axis" << std::endl;
    }
}

void Button::render(const Shader &shader) noexcept {
    detectEdges();

    onClick();

    clampToScreen();

    quad.render(shader);
}

[[nodiscard]] glm::vec2 Button::clampSides(const glm::vec2 cursorEdge, glm::vec2 offsetXY) noexcept {
    for(int i = 0; i < glm::vec2::length(); ++i) {
        if((std::abs(cursorEdge[i]) > 1.0f) && (std::abs(1.0f - std::abs(cursorEdge[i])) > std::abs(offsetXY[i]))) {
            offsetXY[i] = (cursorEdge[i]/std::abs(cursorEdge[i])) - cursorEdge[i];
        }
    }
    return offsetXY;
}
