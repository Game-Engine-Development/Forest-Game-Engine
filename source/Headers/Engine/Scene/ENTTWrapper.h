#pragma once

#include <cassert>

#include "../libraries/entt/single_include/entt/entt.hpp"

namespace EnttWrapper {
    class Scene;
    class Entity {
        const entt::entity entityID;
        Scene *const currentScene;

    public:
        Entity() = delete; //this class should never be stored and only be for temporaries

        constexpr Entity(const entt::entity entity, Scene *const scene)
                : entityID(entity), currentScene(scene)
        {}

        template<typename T>
        [[nodiscard]] constexpr bool hasComponent() const;

        template<typename T>
        [[nodiscard]] constexpr T& getComponent();

        template<typename T>
        [[nodiscard]] constexpr const T& getComponent() const;

        template<typename T, typename ...Args>
        constexpr Entity addComponent(Args&& ...args);

        template<typename T>
        constexpr Entity removeComponent();

        [[nodiscard]] constexpr entt::entity getID() const {
            return entityID;
        };
    };

    class Scene {
        friend class Entity;
        entt::registry registry{};
        std::vector<entt::entity> entities; //@todo replace with scenegraph at some point

    public:
        Scene() = default;

        [[nodiscard]] Entity createEntity() {
            const auto e = registry.create();
            entities.push_back(e);
            return Entity{e, this};
        }

        [[nodiscard]] constexpr Entity getEntity(const entt::entity id) {
            return Entity{id, this};
        }

        [[nodiscard]] constexpr const std::vector<entt::entity>& getEntities() const {
            return entities;
        }
    };


    template<typename T>
    [[nodiscard]] constexpr bool Entity::hasComponent() const {
        return currentScene->registry.template has<T>(entityID);
    }

    template<typename T>
    [[nodiscard]] constexpr T& Entity::getComponent() {
        assert(hasComponent<T>());
        return currentScene->registry.template get<T>(entityID);
    }

    template<typename T>
    [[nodiscard]] constexpr const T& Entity::getComponent() const {
        assert(hasComponent<T>());
        return currentScene->registry.template get<T>(entityID);
    }

    template<typename T, typename ...Args>
    constexpr Entity Entity::addComponent(Args&& ...args) {
        assert(!hasComponent<T>());
        currentScene->registry.template emplace<T>(entityID, std::forward<Args>(args)...);
        return *this; //to allow chaining methods
    }

    template<typename T>
    constexpr Entity Entity::removeComponent() {
        assert(hasComponent<T>());
        currentScene->registry.template remove<T>(entityID);
        return *this; //to allow chaining methods
    }
}