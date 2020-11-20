#include "Headers/Engine/Models/Sphere.h"

Sphere::Sphere(const Component::PosRotationScale &transform, Component::MeshComponent mesh)
: transform(transform), sphereMesh(std::move(mesh)), modelMatrix(createModelMatrix(transform))
{}

void Sphere::render(const Camera &camera, const Shader &shader, const bool blue) noexcept {
    shader.use();

    sphereMesh.bindVAO();

    camera.setMatrices(shader);

    const int modelLoc = glGetUniformLocation(shader.ID, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    const int blueLoc = glGetUniformLocation(shader.ID, "blue");
    const float blueAsFloat = (blue) ? 1.0f : 0.0f;
    glUniform1f(blueLoc, blueAsFloat);

    glDrawArrays(GL_TRIANGLES, 0, sphereMesh.getNumOfVertices());

    Component::MeshComponent::unbindVAO();
}

glm::vec3 Sphere::getPos() const noexcept {
    return transform.getPos();
}
