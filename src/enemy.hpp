#pragma once
#include <cmath>
#include <numeric>
#include "utils.hpp"
#include "entity.hpp"
#include "gameState.hpp"
#include "intersection.hpp"

class Enemy :
    public Entity 
{
private:
    float detection;
    Entity *target;
    Line ray;
public:
    Enemy(const float radius, const float maxVelocity, Entity *target);

    void update(const float& dt, std::vector<Object> objects);
    void render(sf::RenderTarget* target = nullptr);
};

Enemy::Enemy(
    const float radius,
    const float maxVelocity,
    Entity *target
) :
    Entity(radius, maxVelocity), 
    detection(radius * 10.f),
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
    if (!Utils::checkIntersection(entityToTarget, std::vector<Object>{ objects.begin(), objects.end()-1 })) {
        sf::Vector2f dir = Utils::normalize(targetPos - entityPos);
        this->velocity = dir*this->maxVelocity;
    }
    
    this->move(this->velocity, dt);
}

void Enemy::render(sf::RenderTarget* target) {
    target->draw(this->entity);
    // target->draw(this->ray.vertexArray(sf::Color::Red));
}