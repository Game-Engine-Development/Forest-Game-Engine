#pragma once

#include <variant>

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

#include "Headers/Engine/Scene/ENTTWrapper.h"

using Filename = std::string;
using RefCount = std::size_t;

enum struct ImageType {
    RGBA_IMG = 0,
    RGB_IMG = 1,
};

struct Uniform {
    std::string name;
    //shader uniform id
    std::variant<float, glm::vec2, glm::vec3, glm::vec4> data;
};


struct Light {
    std::string lightPosName;
    glm::vec3 lightPos;

    std::string lightColorName;
    glm::vec3 lightColor;
};

namespace Component {
    struct PosRotationScale {
        PosRotationScale() = default;

        PosRotationScale(const glm::vec3 newPos, const glm::vec3 newRotation, const glm::vec3 newScale)
        : pos(newPos), rotation(newRotation), scale(newScale), transformUpdated(false)
        {}

        [[nodiscard]] glm::vec3 getPos() const {
            return pos;
        }
        [[nodiscard]] glm::vec3 getRotation() const {
            return rotation;
        }
        [[nodiscard]] glm::vec3 getScale() const {
            return scale;
        }
        [[nodiscard]] bool getTransformUpdated() const {
            return transformUpdated;
        }

        void setPos(const glm::vec3 newPos) {
            pos = newPos;
            transformUpdated = false;
        }
        void setRotation(const glm::vec3 newRotation) {
            rotation = newRotation;
            transformUpdated = false;
        }
        void setScale(const glm::vec3 newScale) {
            scale = newScale;
            transformUpdated = false;
        }
        void setTransformUpdated() {
            transformUpdated = true;
        }

    private:
        glm::vec3 pos{}, rotation{}, scale{};
        bool transformUpdated = false;
    };

    using Transform = glm::mat4;
}

struct Ray {
    glm::vec3 origin{}, slope{};
};
