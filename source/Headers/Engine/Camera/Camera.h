#pragma once

#include <algorithm>
#include <vector>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Headers/Engine/Shader/Shader.h"

// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
class Camera {
public:
    // Constructor with vectors
    explicit Camera(
            glm::vec3 position = glm::vec3(0.0f, 10.0f, 0.0f),
            glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
            float yaw = YAW,
            float pitch = PITCH
    ) noexcept;

    void setAspectRatio(float ratio) noexcept;
    // Returns the view matrix calculated using Euler Angles and the LookAt Matrix
    [[nodiscard]] glm::mat4 GetViewMatrix() const noexcept;

    // Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
    void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true) noexcept;

    // Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
    void ProcessMouseScroll(float yoffset) noexcept;

    void setMatrices(const Shader& shader) const noexcept;

    [[nodiscard]] glm::mat4 getProjectionMatrix() const noexcept;

    [[nodiscard]] glm::mat4 getViewMatrix() const noexcept;

    void setSkyboxMatrices(const Shader& shader) const noexcept;

    [[nodiscard]] glm::vec3 getPos() const noexcept;

    void setYPos(float y) noexcept;

    [[nodiscard]] float getYaw() const noexcept;
    [[nodiscard]] glm::vec3 getFront() const noexcept;
    [[nodiscard]] glm::vec3 getRight() const noexcept;
    [[nodiscard]] glm::vec3 getPosition() const noexcept;

    void setPosition(glm::vec3 pos) noexcept;

private:
    // Default camera values
    static constexpr float YAW         =  -90.0f;
    static constexpr float PITCH       =  0.0f;
    static constexpr float SENSITIVITY =  0.5f;
    static constexpr float ZOOM        =  45.0f;

    // Camera Attributes
    glm::vec3 Position;
    glm::vec3 Front{0.0f, 0.0f, -1.0f};
    glm::vec3 Up{};
    glm::vec3 Right{};
    glm::vec3 WorldUp;

    // Euler Angles
    float Yaw;
    float Pitch;

    // Camera options
    float MouseSensitivity{SENSITIVITY};
    float Zoom{ZOOM};
    float aspectRatio = 800.0f/600.0f;

    // Calculates the front vector from the Camera's (updated) Euler Angles
    void updateCameraVectors() noexcept;
};