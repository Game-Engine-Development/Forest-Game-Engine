#pragma once

#include <utility>
#include <cstdint>

#include "Headers/Engine/Camera/Camera.h"
#include "Headers/Engine/Shader/Shader.h"
#include "Headers/Engine/Scene/ENTTWrapper.h"
#include "Headers/Engine/Scene/Components.h"
#include "Headers/Engine/Utils/MiscUtils.h"
#include "Headers/Engine/Utils/CommonDeclarations.h"

struct Visitor {
    Visitor() = delete;
    explicit Visitor(const int uniformId) : id(uniformId) {}

    void operator()(const float data) const {
        glUniform1f(id, data);
    }
    void operator()(const glm::vec2 data) const {
        glUniform2fv(id, 1, glm::value_ptr(data));
    }
    void operator()(const glm::vec3 data) const {
        glUniform3fv(id, 1, glm::value_ptr(data));
    }
    void operator()(const glm::vec4 data) const {
        glUniform4fv(id, 1, glm::value_ptr(data));
    }

private:
    const int id;
};

void renderEntity(EnttWrapper::Scene &scene, const entt::entity id, const Camera &camera,
                  const std::vector<Light> &lights) //@todo move lights into being part of the scene
{
    if(scene.getEntity(id).hasComponent<Component::Drawable>()) {
        auto entity = scene.getEntity(id);

        auto &drawable = entity.getComponent<Component::Drawable>();

        drawable.shader.use();

        drawable.mesh.bindVAO();
        for(Component::TextureComponent &texture : drawable.textures) {
            texture.bind(drawable.shader);
        }

        camera.setMatrices(drawable.shader);

        const int modelLoc = glGetUniformLocation(drawable.shader.ID, "model"); //@todo cache this somewhere
        auto &posRotScale = entity.getComponent<Component::PosRotationScale>();
        if(!posRotScale.getTransformUpdated()) {
            drawable.modelMatrix = createModelMatrix(posRotScale);
            posRotScale.setTransformUpdated();
        }
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(drawable.modelMatrix));

        for(auto &e : lights) {
            const int lightLoc = glGetUniformLocation(drawable.shader.ID, e.lightPosName.c_str());
            glUniform3fv(lightLoc, 1, glm::value_ptr(e.lightPos));

            const int lightCol = glGetUniformLocation(drawable.shader.ID, e.lightColorName.c_str());
            glUniform3fv(lightCol, 1, glm::value_ptr(e.lightColor));
        }

        //float = 0, glm::vec2 = 1, glm::vec3 = 2, glm::vec4 = 3,
        for(auto u : drawable.uniforms) {
            const int f = glGetUniformLocation(drawable.shader.ID, u.name.c_str());
            std::visit(Visitor{f}, u.data);
        }

        const int viewLoc = glGetUniformLocation(drawable.shader.ID, "viewPos"); //@todo cache this somewhere
        glUniform3fv(viewLoc, 1, glm::value_ptr(camera.getPos()));

        glDrawArrays(GL_TRIANGLES, 0, drawable.mesh.getNumOfVertices());

        for (Component::TextureComponent &texture : drawable.textures) {
            texture.unbind();
        }
        Component::MeshComponent::unbindVAO();
    }
}

void renderScene(EnttWrapper::Scene &scene, const Camera &camera,
                 const std::vector<Light> &lights)
{
    for(const auto id : scene.getEntities()) {
        renderEntity(scene, id, camera, lights);
    }
}
