#pragma once
#include "object.hpp"

class Entity {
protected:
    sf::CircleShape entity;
    sf::Vector2f velocity;
    const float maxVelocity;

    void checkCollisions(std::vector<Object> objects);
public:
    Entity(const float radius, const float maxVelocity);
    virtual ~Entity();

    sf::FloatRect getGlobalBounds() const;
    sf::Vector2f getPosition() const;
    float getRadius() const;

    void setPosition(const sf::Vector2f position);
    void setPosition(const float x, const float y);
    void setFillColor(const sf::Color color);
    void setRadius(const float radius);

    void move(const sf::Vector2f velocity, const float& dt);
    virtual void update(const float& dt, std::vector<Object> objects) = 0;
    virtual void render(sf::RenderTarget* target = nullptr) = 0;
};

Entity::Entity(const float radius = 0.f, const float maxVelocity = 0.f) : maxVelocity(maxVelocity) {
    this->entity.setRadius(radius);
}

Entity::~Entity() {}

void Entity::checkCollisions(std::vector<Object> objects) {
    sf::FloatRect entityBounds = entity.getGlobalBounds();
    sf::FloatRect nextPos = entityBounds;
    nextPos.left += velocity.x;
    nextPos.top += velocity.y;

    for (Object object : objects) {
        sf::FloatRect objectBounds = object.getGlobalBounds();
        if(objectBounds.intersects(entityBounds)) {
            //bottom collision
            if (entityBounds.top < objectBounds.top && entityBounds.top + entityBounds.height < objectBounds.top + objectBounds.height && entityBounds.left < objectBounds.left + objectBounds.width
            && entityBounds.left + entityBounds.width > objectBounds.left){
                velocity.y = 0.f;
                entity.setPosition(entityBounds.left, objectBounds.top - entityBounds.height);
            }

            //Top collision
            else if (entityBounds.top > objectBounds.top && entityBounds.top + entityBounds.height > objectBounds.top + objectBounds.height && entityBounds.left < objectBounds.left+ objectBounds.width
            && entityBounds.left + entityBounds.width > objectBounds.left) {
                velocity.y = 0.f;
                entity.setPosition(entityBounds.left, objectBounds.top + objectBounds.height);
            }

            //Right collision
            if (entityBounds.left < objectBounds.left && entityBounds.left + entityBounds.width < objectBounds.left + objectBounds.width && entityBounds.top < objectBounds.top + objectBounds.height
            && entityBounds.top + entityBounds.height > objectBounds.top){
                velocity.x = 0.f;
                entity.setPosition(objectBounds.left - entityBounds.width, entityBounds.top);
            }

                //left collision
            else if (entityBounds.left > objectBounds.left &&
                entityBounds.left + entityBounds.width > objectBounds.left + objectBounds.width && entityBounds.top < objectBounds.top + objectBounds.height && entityBounds.top + entityBounds.height > objectBounds.top){
                velocity.x = 0.f;
                entity.setPosition(objectBounds.left + objectBounds.width, entityBounds.top);
            }
        }
    }

    if(entity.getPosition().x < 0.f) entity.setPosition(0.f, entity.getPosition().y);
    if(entity.getPosition().y < 0.f) entity.setPosition(entity.getPosition().x, 0.f);
    if(entity.getPosition().x + entity.getRadius()*2 > 800) entity.setPosition(800 - entity.getRadius()*2, entity.getPosition().y);
    if(entity.getPosition().y + entity.getRadius()*2 > 600) entity.setPosition(entity.getPosition().x, 600 - entity.getRadius()*2);
}

sf::FloatRect Entity::getGlobalBounds() const { return this->entity.getGlobalBounds(); }
sf::Vector2f Entity::getPosition() const { return this->entity.getPosition(); }
float Entity::getRadius() const { return this->entity.getRadius(); }

void Entity::setRadius(const float radius) { this->entity.setRadius(radius); }
void Entity::setPosition(const sf::Vector2f position) { this->entity.setPosition(position); }
void Entity::setPosition(const float x, const float y) { this->entity.setPosition(sf::Vector2f(x, y)); }
void Entity::setFillColor(const sf::Color color) { this->entity.setFillColor(color); }

void Entity::move(const sf::Vector2f velocity, const float& dt) {
    this->entity.setPosition(this->entity.getPosition() + velocity*dt);
}