#include "Headers/Game/Entities/Animal.h"

Animal::Animal(Entity &entity, Player* player, int health, int dmg, float move_speed, float jump_power) : m_entity(entity), m_collisionHandler(CollisionHandler(&m_entity)), m_player(player), m_health(health), m_damage(dmg), m_move_speed(move_speed), m_jump_power(jump_power) {}

Animal::Animal(Entity &&entity, Player* player, int health, int dmg, float move_speed, float jump_power) : m_entity(entity), m_collisionHandler(CollisionHandler(&m_entity)), m_player(player), m_health(health), m_damage(dmg), m_move_speed(move_speed), m_jump_power(jump_power) {}

Animal::Animal(Animal &animal) : m_entity(animal.m_entity), m_collisionHandler(CollisionHandler(&m_entity)), m_player(animal.m_player), m_health(animal.m_health), m_damage(animal.m_damage), m_move_speed(animal.m_move_speed), m_jump_power(animal.m_jump_power) {}

void Animal::create(Entity& entity, Player* player, int health, int dmg, float move_speed, float jump_power) {
    m_entity = entity;
    m_collisionHandler = CollisionHandler(&m_entity);
    m_player = player;

    m_move_speed = move_speed;
    m_jump_power = jump_power;
    m_health = health;
    m_damage = dmg;
}
void Animal::create(Entity&& entity, Player* player, int health, int dmg, float move_speed, float jump_power) {
    m_entity = entity;
    m_collisionHandler = CollisionHandler(&m_entity);
    m_player = player;

    m_move_speed = move_speed;
    m_jump_power = jump_power;
    m_health = health;
    m_damage = dmg;
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
    m_player->takeDamage(m_damage);
}

void Animal::takeDamage(int damage) {
    m_health -= damage;
}

bool Animal::isDead() {
    return m_dead;
}