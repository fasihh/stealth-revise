#pragma once
#include "entity.hpp"
#include "intersection.hpp"
#include "utils.hpp"

class Player :
    public Entity
{
private:
    std::vector<sf::Vertex> vertices;
public:
    Player(const float radius, const float maxVelocity);
    virtual ~Player();

    void update(const float& dt, std::vector<Object> objects);
    void render(sf::RenderTarget* target = nullptr);
};

Player::Player(const float radius, const float maxVelocity) : Entity(radius, maxVelocity) {}

Player::~Player() {}

void Player::update(const float& dt, std::vector<Object> objects) {
    this->vertices.clear();
    // managing light
    std::vector<float> angles;
    std::vector<Intersection> intersections;

    sf::Vector2f entityPos = this->entity.getPosition() + this->entity.getRadius()*sf::Vector2f{ 1.f, 1.f };

    for (Object object : objects) {
        for (Point point : object.getVerticies()) {
            float angle = std::atan2(point.y-entityPos.y,point.x-entityPos.x);
            angles.push_back(angle-0.0001f);
            angles.push_back(angle);
            angles.push_back(angle+0.0001f);
        }
    }

    for (float angle : angles) {
        Line ray{ entityPos, entityPos + sf::Vector2f{ std::cos(angle), std::sin(angle) } };
        intersections.push_back(Utils::checkIntersection(ray, objects, angle));
    }
    std::sort(intersections.begin(), intersections.end(), [&](const Intersection& a, const Intersection& b) { return a.angle > b.angle; });
    intersections.emplace_back(intersections[0]); // this completes the triangle fan from all directions

    // storing vertices for the triangle fan
    // initial point for the triangle fan
    unsigned int alpha = 50; // opacity of the trianglefan
    sf::Vertex entityVertex(entityPos);
    entityVertex.color = sf::Color(255, 255, 255, alpha);
    this->vertices.emplace_back(entityVertex);

    // storing every intersection point in the vertices
    for (Intersection intersect : intersections)
        // Line ray{ { entityPos.x, entityPos.y }, { intersect.point.x, intersect.point.y } };
        // rays.push_back(ray);
        this->vertices.emplace_back(intersect.point.vertex(sf::Color(255, 255, 255, alpha)));
    
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

    float diagonalSpeed = 70*this->maxVelocity/99;

    // up-left
    if (keys[0] && keys[1])
        this->velocity = { -diagonalSpeed, -diagonalSpeed };
    // up-right
    else if (keys[0] && keys[3])
        this->velocity = { diagonalSpeed, -diagonalSpeed };
    // down-left
    else if (keys[2] && keys[1])
        this->velocity = { -diagonalSpeed, diagonalSpeed };
    // down-right
    else if (keys[2] && keys[3])
        this->velocity = { diagonalSpeed, diagonalSpeed };
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
    target->draw(this->vertices.data(), vertices.size(), sf::TriangleFan);
    target->draw(this->entity);
}