#pragma once

#include <utility>

#include "Headers/Engine/Camera/Camera.h"
#include "Headers/Engine/Shader/Shader.h"
#include "Headers/Engine/Scene/ENTTWrapper.h"
#include "Headers/Engine/Scene/Components.h"
#include "Headers/Engine/Utils/MiscUtils.h"
#include "Headers/Engine/Utils/CommonDeclarations.h"

void renderEntity(EnttWrapper::Scene &scene, const entt::entity id, const Camera &camera,
                  const Shader &inputShader, const std::vector<Light> &lights)
{
    if(scene.getEntity(id).hasComponent<Component::Drawable>()) {
        auto entity = scene.getEntity(id);

        inputShader.use();

        auto &drawable = entity.getComponent<Component::Drawable>();

        drawable.mesh.bindVAO();
        for(Component::TextureComponent &texture : drawable.textures) {
            texture.bind(inputShader);
        }

        camera.setMatrices(inputShader);

        const int modelLoc = glGetUniformLocation(inputShader.ID, "model"); //@todo cache this somewhere
        auto &posRotScale = entity.getComponent<Component::PosRotationScale>();
        if(!posRotScale.getTransformUpdated()) {
            drawable.modelMatrix = createModelMatrix(posRotScale);
            posRotScale.setTransformUpdated();
        }
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(drawable.modelMatrix));

        for(auto &e : lights) {
            const int lightLoc = glGetUniformLocation(inputShader.ID, e.lightPosName.c_str());
            glUniform3fv(lightLoc, 1, glm::value_ptr(e.lightPos));

            const int lightCol = glGetUniformLocation(inputShader.ID, e.lightColorName.c_str());
            glUniform3fv(lightCol, 1, glm::value_ptr(e.lightColor));
        }

        const int viewLoc = glGetUniformLocation(inputShader.ID, "viewPos"); //@todo cache this somewhere
        glUniform3fv(viewLoc, 1, glm::value_ptr(camera.getPos()));

        glDrawArrays(GL_TRIANGLES, 0, drawable.mesh.getNumOfVertices());

        for (Component::TextureComponent &texture : drawable.textures) {
            texture.unbind();
        }
        Component::MeshComponent::unbindVAO();
    }
}

void renderScene(EnttWrapper::Scene &scene, const Camera &camera,
                 const Shader &inputShader, const std::vector<Light> &lights)
{
    for(const auto id : scene.getEntities()) {
        renderEntity(scene, id, camera, inputShader, lights);
    }
}
