#include "Headers/Engine/Camera/Camera.h"

// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
// Constructor with vectors
Camera::Camera(const glm::vec3 position, const glm::vec3 up, const float yaw, const float pitch) noexcept
    : Position(position), WorldUp(up), Yaw(yaw), Pitch(pitch)
{
    updateCameraVectors();
}

void Camera::setAspectRatio(const float ratio) noexcept {
    aspectRatio = ratio;
}

// Returns the view matrix calculated using Euler Angles and the LookAt Matrix
[[nodiscard]] glm::mat4 Camera::GetViewMatrix() const noexcept {
    return glm::lookAt(Position, Position + Front, Up);
}

// Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
void Camera::ProcessMouseMovement(float xOffset, float yOffset, const GLboolean constrainPitch) noexcept {
    xOffset *= MouseSensitivity;
    yOffset *= MouseSensitivity;

    Yaw   += xOffset;
    Pitch += yOffset;

    // Make sure that when pitch is out of bounds, screen doesn't get flipped
    if(constrainPitch) {
        Pitch = std::clamp(Pitch, -89.0f, 89.0f);
    }

    // Update Front, Right and Up Vectors using the updated Euler angles
    updateCameraVectors();
}

// Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
void Camera::ProcessMouseScroll(const float yOffset) noexcept {
    if (Zoom >= 1.0f && Zoom <= 45.0f) {
        Zoom -= yOffset;
    }

    Zoom = std::clamp(ZOOM, 1.0f, 45.0f);
}

// Calculates the front vector from the Camera's (updated) Euler Angles
void Camera::updateCameraVectors() noexcept {
    // Calculate the new Front vector
    const glm::vec3 front{
            cos(glm::radians(Yaw)) * cos(glm::radians(Pitch)),
            sin(glm::radians(Pitch)),
            sin(glm::radians(Yaw)) * cos(glm::radians(Pitch)),
    };
    Front = glm::normalize(front);

    // Also re-calculate the Right and Up vector
    Right = glm::normalize(glm::cross(Front, WorldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
    Up    = glm::normalize(glm::cross(Right, Front));
}

[[nodiscard]] glm::mat4 Camera::getProjectionMatrix() const noexcept {
    return glm::perspective(glm::radians(Zoom), aspectRatio, 0.1f, 100000.0f);
}

[[nodiscard]] glm::mat4 Camera::getViewMatrix() const noexcept {
    return GetViewMatrix();
}

void Camera::setMatrices(const Shader &shader) const noexcept {
    const glm::mat4 view = GetViewMatrix();
    const glm::mat4 projection = glm::perspective(glm::radians(Zoom), aspectRatio, 0.1f, 100000.0f);

    const int viewLoc = glGetUniformLocation(shader.ID, "view");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    const int projectionLoc = glGetUniformLocation(shader.ID, "projection");
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
}

void Camera::setSkyboxMatrices(const Shader &shader) const noexcept {
    const glm::mat4 view = glm::mat4(glm::mat3(GetViewMatrix()));
    const glm::mat4 projection = glm::perspective(glm::radians(Zoom), aspectRatio, 0.1f, 100000.0f);

    const int viewLoc = glGetUniformLocation(shader.ID, "view");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    const int projectionLoc = glGetUniformLocation(shader.ID, "projection");
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
}

[[nodiscard]] glm::vec3 Camera::getPos() const noexcept {
    return Position;
}

void Camera::setYPos(const float newY) noexcept {
    Position.y = newY;
}

[[nodiscard]] float Camera::getYaw() const noexcept {
    return Yaw;
}
[[nodiscard]] glm::vec3 Camera::getFront() const noexcept {
    return Front;
}
[[nodiscard]] glm::vec3 Camera::getRight() const noexcept {
    return Right;
}
[[nodiscard]] glm::vec3 Camera::getPosition() const noexcept {
    return Position;
}

void Camera::setPosition(const glm::vec3 pos) noexcept {
    Position = pos;
}
