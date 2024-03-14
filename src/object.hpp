#pragma once
#include "line.hpp"
#include <SFML/Graphics.hpp>

class Object {
private:
    sf::RectangleShape object;
    std::vector<Point> vertices;
    std::vector<Line> segments;
public:
    Object(sf::Vector2f size, sf::Color color = sf::Color::White);
    virtual ~Object();

    const std::vector<Point>& getVerticies();
    const std::vector<Line>& getSegments();
    void setPosition(sf::Vector2f position);

    void render(sf::RenderTarget* target = nullptr);
};

Object::Object(sf::Vector2f size, sf::Color color) : 
    object(size),
    vertices({
        Point{ 0.f, 0.f },
        Point{ size.x, 0.f },
        Point{ size.x, size.y },
        Point{ 0.f, size.y }
    }),
    segments({
        Line{ this->vertices[0], this->vertices[1] },
        Line{ this->vertices[1], this->vertices[2] },
        Line{ this->vertices[2], this->vertices[3] },
        Line{ this->vertices[3], this->vertices[0] }
    })
{
    this->object.setFillColor(color);
}

Object::~Object() {

}

const std::vector<Point>& Object::getVerticies() {
    return this->vertices;
}

const std::vector<Line>& Object::getSegments() {
    return this->segments;
}

void Object::setPosition(sf::Vector2f position) {
    this->object.setPosition(position);

    sf::Vector2f size = this->object.getSize();
    this->vertices.assign({
        Point{ position.x, position.y },
        Point{ position.x + size.x, position.y },
        Point{ position.x + size.x, position.y + size.y },
        Point{ position.x, position.y + size.y }
    });

    this->segments.assign({
        Line{ this->vertices[0], this->vertices[1] },
        Line{ this->vertices[1], this->vertices[2] },
        Line{ this->vertices[2], this->vertices[3] },
        Line{ this->vertices[3], this->vertices[0] }
    });
}

void Object::render(sf::RenderTarget* target) {
    target->draw(this->object);
}
