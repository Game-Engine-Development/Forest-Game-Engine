#pragma once

#include "Headers/Engine/Scene/ENTTWrapper.h"
#include "Headers/Engine/Scene/Components.h"
#include "Headers/Engine/Shader/Shader.h"
#include "Headers/Engine/Utils/DataStructures/Octree.h"
#include "Headers/Engine/Terrain/Terrain.h"
#include "Headers/Engine/Utils/MiscUtils.h"
#include "Headers/Engine/Utils/Raycast.h"

std::optional<EnttWrapper::Entity> createBaseEntity(EnttWrapper::Scene &scene, const std::string &modelFilename, Shader shader, Component::PosRotationScale posRotScale) {
    auto model = loadModel(modelFilename);

    if(!model.has_value()) return std::nullopt;

    return scene.createEntity()
                .addComponent<Component::Drawable, Component::Model, Shader>(std::move(model.value()), std::move(shader))
                .addComponent<Component::PosRotationScale>(posRotScale);
}

std::optional<EnttWrapper::Entity> createBaseEntity(EnttWrapper::Scene &scene, const std::string &modelFilename, Shader shader, std::vector<Uniform> uniforms, Component::PosRotationScale posRotScale) {
    auto model = loadModel(modelFilename);

    if(!model.has_value()) return std::nullopt;

    return scene.createEntity()
                .addComponent<Component::Drawable, Component::Model, Shader, std::vector<Uniform>>(std::move(model.value()), std::move(shader), std::move(uniforms))
                .addComponent<Component::PosRotationScale>(posRotScale);
}

DataStructures::Octree createTreeWithTerrain(const Terrain &terrain) {
    std::vector<Coordinate> points;
    for(int i = 0; i < terrain.terrainMesh->getWidth(); ++i) {
        for (int j = 0; j < terrain.terrainMesh->getWidth(); ++j) {
            const glm::vec2 x_z = terrain.terrainMesh->getX_Y_InWorldCoords(i, j);
            points.push_back(glm::vec3{x_z.x, terrain.terrainMesh->getHeightFromHeightmap(i, j), x_z.y} + terrain.getPos());
        }
    }


    const int lastIndex = terrain.terrainMesh->getWidth()-1;
    const glm::vec2 x_z = terrain.terrainMesh->getX_Y_InWorldCoords(lastIndex, lastIndex);
    return DataStructures::Octree{BoundingBox{.center=terrain.getPos() + glm::vec3(TerrainMesh::SIZE/2.f, (terrain.terrainMesh->getMaxHeight() + terrain.terrainMesh->getMinHeight())/2.f, TerrainMesh::SIZE/2.f),
            .halfWidths=glm::vec3(x_z.x/2.f, ((terrain.terrainMesh->getMaxHeight())-((terrain.terrainMesh->getMaxHeight() + terrain.terrainMesh->getMinHeight())/2.f))+0.00001, x_z.x/2.f)}, points};
}

std::optional<Coordinate> getTerrainIntersection(const Window &window, const Camera &camera, const DataStructures::Octree &tree, EnttWrapper::Entity terrain) {
    double xposLocal, yposLocal;
    glfwGetCursorPos(window.getWindow(), &xposLocal, &yposLocal);
    // work out ray
    const glm::vec3 ray_wor = get_ray_from_mouse(window, camera, static_cast<float>(xposLocal), static_cast<float>(yposLocal));
    return tree.firstRayintersection(Ray{.origin=camera.getPos(), .slope=ray_wor});
}

inline int g_selected_sphere = -1;

void raycastPickSphere(const Window &window, const Camera &camera, EnttWrapper::Scene &scene, const int startIndex, const int numberOfSpheres) {
    // Note: could query if window has lost focus here
    if ((Input::isButtonDown(GLFW_MOUSE_BUTTON_LEFT) || Input::isButtonDown(GLFW_MOUSE_BUTTON_RIGHT)) && Input::getCursor()) {
        double xposLocal, yposLocal;
        glfwGetCursorPos(window.getWindow(), &xposLocal, &yposLocal);
        // work out ray
        const glm::vec3 ray_wor = get_ray_from_mouse(window, camera, (float)xposLocal, (float)yposLocal);
        //const glm::vec3 ray_wor = getRayFromScreenSpace(glm::vec2{(float)xposLocal, (float)yposLocal}, (*instance->m_camera), (*instance->m_window));
        // check ray against all spheres in scene
        int closest_sphere_clicked = -1;
        float closest_intersection = 0.0f;
        for (int i = startIndex; i < /*instance->spheres->size()*/ startIndex+numberOfSpheres; i++) {
            float t_dist = 0.0f;
            constexpr float sphere_radius = 1.0f; //temporary @todo change
            if (ray_sphere(camera.getPos(), ray_wor, /*sphere_pos_wor[i]*/ /*instance->spheres->at(i).getPos()*/ scene.getEntity(scene.getEntities().at(i)).getComponent<Component::PosRotationScale>().getPos(), sphere_radius, t_dist)) {
                // if more than one sphere is in path of ray, only use the closest one
                if (-1 == closest_sphere_clicked || t_dist < closest_intersection) {
                    closest_sphere_clicked = i;
                    closest_intersection = t_dist;
                }
            }
        }
        g_selected_sphere = closest_sphere_clicked;
        std::cout << "sphere " << closest_sphere_clicked << " was clicked\n";
    }
}

