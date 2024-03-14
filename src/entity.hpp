#pragma once
#include "object.hpp"

class Entity
{
protected:
    sf::CircleShape entity;
    sf::Vector2f velocity;
    const float maxVelocity;
public:
    Entity(const float radius, const float maxVelocity);
    virtual ~Entity();

    sf::FloatRect getGlobalBounds() const;
    sf::Vector2f getPosition() const;

    void setPosition(const sf::Vector2f position);
    void setPosition(const float x, const float y);
    void setFillColor(const sf::Color color);


    void move(const sf::Vector2f velocity, const float& dt);
    virtual void update(const float& dt, std::vector<Object> objects) = 0;
    virtual void render(sf::RenderTarget* target = nullptr) = 0;
};

Entity::Entity(const float radius, const float maxVelocity) : maxVelocity(maxVelocity) {
    this->entity.setRadius(radius);
}

Entity::~Entity() {}

sf::FloatRect Entity::getGlobalBounds() const { return this->entity.getGlobalBounds(); }
sf::Vector2f Entity::getPosition() const { return this->entity.getPosition(); }

void Entity::setPosition(const sf::Vector2f position) { this->entity.setPosition(position); }
void Entity::setPosition(const float x, const float y) { this->entity.setPosition(sf::Vector2f(x, y)); }
void Entity::setFillColor(const sf::Color color) { this->entity.setFillColor(color); }

void Entity::move(const sf::Vector2f velocity, const float& dt) {
    this->entity.setPosition(this->entity.getPosition() + velocity*dt);
}