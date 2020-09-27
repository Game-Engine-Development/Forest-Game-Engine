#pragma once

#include <cassert>

#include "../libraries/entt/single_include/entt/entt.hpp"

namespace EnttWrapper {
    class Scene;
    class Entity {
        entt::entity entityID{};
        Scene *currentScene = nullptr;

    public:
        Entity() = default;
        Entity(const entt::entity entity, Scene *scene)
                : entityID(entity), currentScene(scene)
        {}

        template<typename T>
        bool hasComponent();

        template<typename T>
        T& getComponent();

        template<typename T, typename ...Args>
        void addComponent(Args&& ...args);

        template<typename T>
        void removeComponent();
    };

    class Scene {
        friend class Entity;
        entt::registry registry;
        //insert scene graph at some point

    public:
        Entity createEntity() {
            return Entity{registry.create(), this};
        }
    };


    template<typename T>
    bool Entity::hasComponent() {
        return currentScene->registry.template has<T>(entityID);
    }

    template<typename T>
    T& Entity::getComponent() {
        assert(hasComponent<T>());
        return currentScene->registry.template get<T>(entityID);
    }

    template<typename T, typename ...Args>
    void Entity::addComponent(Args&& ...args) {
        assert(!hasComponent<T>());
        currentScene->registry.template emplace<T>(entityID, std::forward<Args>(args)...);
    }

    template<typename T>
    void Entity::removeComponent() {
        assert(hasComponent<T>());
        currentScene->registry.template remove<T>(entityID);
    }
}