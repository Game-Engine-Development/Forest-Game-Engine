#include "Headers/Game/Entities/Animal.h"

Animal::Animal() = default;

Animal::Animal(Entity &entity, Player* player, Spirit* spirit, float move_speed, float jump_power) : m_entity(entity), m_collisionHandler(CollisionHandler(&m_entity)), m_player(player), m_spirit(spirit), m_move_speed(move_speed), m_jump_power(jump_power) {}

Animal::Animal(Entity &&entity, Player* player, Spirit* spirit, float move_speed, float jump_power) : m_entity(entity), m_collisionHandler(CollisionHandler(&m_entity)), m_player(player), m_spirit(spirit), m_move_speed(move_speed), m_jump_power(jump_power) {}

void Animal::die() {
    m_spirit->animalDied();
}

void Animal::create(Entity& entity, Player* player, Spirit* spirit, float move_speed, float jump_power) {
    m_entity = entity;
    m_collisionHandler = CollisionHandler(&m_entity);
    m_player = player;
    m_spirit = spirit;

    m_move_speed = move_speed;
    m_jump_power = jump_power;
}
void Animal::create(Entity&& entity, Player* player, Spirit* spirit, float move_speed, float jump_power) {
    m_entity = entity;
    m_collisionHandler = CollisionHandler(&m_entity);
    m_player = player;
    m_spirit = spirit;

    m_move_speed = move_speed;
    m_jump_power = jump_power;
}

Entity Animal::getEntity() {
    return m_entity;
}

Entity* Animal::getEntityPointer() {
    return &m_entity;
}

void Animal::render(Camera &camera, Shader &shader, glm::vec3 &lightPos, glm::vec3 &lightColor) {
    m_entity.render(camera, shader, lightPos, lightColor);
}

void Animal::hitPlayer() {
    m_player->takeDamage(static_cast<float>(m_damage));
}

void Animal::takeDamage(int damage) {
    m_health -= damage;
}