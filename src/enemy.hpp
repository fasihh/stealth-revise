#pragma once
#include <cmath>
#include <numeric>
#include "utils.hpp"
#include "entity.hpp"
#include "gameState.hpp"
#include "intersection.hpp"

class Enemy : public Entity {
private:
    Entity *target;
    sf::Clock delay;
public:
    Enemy(const float radius, const float maxVelocity, Entity *target);

    void update(const float& dt, std::vector<Object> objects);
    void render(sf::RenderTarget* target = nullptr);
};

Enemy::Enemy(
    const float radius = 0.f,
    const float maxVelocity = 0.f,
    Entity *target = nullptr
) :
    Entity(radius, maxVelocity),
    target(target)
{}

void Enemy::update(const float& dt, std::vector<Object> objects) {
    sf::Vector2f entityPos = this->entity.getPosition() + this->entity.getRadius() * sf::Vector2f{ 1.f, 1.f };
    sf::Vector2f targetPos = this->target->getPosition() + this->target->getRadius() * sf::Vector2f{ 1.f, 1.f };

    float angle = std::atan2(targetPos.y-entityPos.y, targetPos.x-entityPos.x);

    Line entityToTarget{ entityPos, entityPos + sf::Vector2f{ std::cos(angle), std::sin(angle) } };

    // managing velocity and chasing player
    this->velocity *= 0.85f;

    // if enemy within player's light
    // chase player
    if (Line{ Utils::checkIntersection(entityToTarget, objects).point, entityPos } >= Line{ entityPos, targetPos })
        this->velocity = Utils::normalize(targetPos - entityPos)*this->maxVelocity;

    if (Line{ entityPos, targetPos }.distance() <= target->getRadius())
        if (delay.getElapsedTime().asSeconds() >= 2)
            Enemy::status = -1;
    else
        delay.restart();
    
    this->move(this->velocity, dt);
    this->checkCollisions(objects);
}

void Enemy::render(sf::RenderTarget* target) {
    target->draw(this->entity);
}