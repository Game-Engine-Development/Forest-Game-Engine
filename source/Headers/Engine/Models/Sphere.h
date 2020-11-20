#pragma once

#include <glm/gtc/type_ptr.hpp>

#include "Headers/Engine/Camera/Camera.h"
#include "Headers/Engine/Shader/Shader.h"
#include "Headers/Engine/Utils/CommonDeclarations.h"
#include "Headers/Engine/Scene/Components.h"
#include "Headers/Engine/Utils/MiscUtils.h"


class Sphere {
    const Component::PosRotationScale transform{};
    Component::MeshComponent sphereMesh;
    glm::mat4 modelMatrix{};

public:
    Sphere(const Component::PosRotationScale &transform, Component::MeshComponent mesh);
    void render(const Camera &camera, const Shader &shader, bool blue) noexcept;
    [[nodiscard]] glm::vec3 getPos() const noexcept;
};
