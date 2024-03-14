#pragma once
#include "entity.hpp"

class Player :
    public Entity 
{
private:
public:
    Player(const float radius, const float maxVelocity);
    virtual ~Player();

    void update(const float& dt, std::vector<Object> objects);
    void render(sf::RenderTarget* target = nullptr);
};

Player::Player(const float radius, const float maxVelocity) : Entity(radius, maxVelocity) {}

Player::~Player() {}

void Player::update(const float& dt, std::vector<Object> objects) {
    this->velocity *= 0.85f;

    if ((this->velocity.x > -0.1f && this->velocity.x < 0.f))
        this->velocity.x = 0.f;
    if ((this->velocity.y > -0.1f && this->velocity.y < 0.f))
        this->velocity.y = 0.f;

    bool keys[4] = {
        sf::Keyboard::isKeyPressed(sf::Keyboard::W),
        sf::Keyboard::isKeyPressed(sf::Keyboard::A),
        sf::Keyboard::isKeyPressed(sf::Keyboard::S),
        sf::Keyboard::isKeyPressed(sf::Keyboard::D)
    };

    float diagonalSpeed = 70*this->maxVelocity / 99;

    // up-left
    if (keys[0] && keys[1])
        this->velocity.x = -diagonalSpeed, this->velocity.y = -diagonalSpeed;
    // up-right
    else if (keys[0] && keys[3])
        this->velocity.x = diagonalSpeed, this->velocity.y = -diagonalSpeed;
    // down-left
    else if (keys[2] && keys[1])
        this->velocity.x = -diagonalSpeed, this->velocity.y = diagonalSpeed;
    // down-right
    else if (keys[2] && keys[3])
        this->velocity.x = diagonalSpeed, this->velocity.y = diagonalSpeed;
    // up
    else if (keys[0])
        this->velocity.y = -this->maxVelocity;
    // left
    else if (keys[1])
        this->velocity.x = -this->maxVelocity;
    // down
    else if (keys[2])
        this->velocity.y = this->maxVelocity;
    // right
    else if (keys[3])
        this->velocity.x = this->maxVelocity;

    this->move(this->velocity, dt);
}

void Player::render(sf::RenderTarget* target) {
    target->draw(this->entity);
}