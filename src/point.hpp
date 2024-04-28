#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>

struct Point {
    float x, y;

    Point(float x = 0.f, float y = 0.f) : x(x), y(y) {};
    Point(sf::Vector2f position) : x(position.x), y(position.y) {}
    
    sf::Vector2f vector();
    sf::Vertex vertex(sf::Color color = sf::Color::White);

    Point& operator = (const sf::Vector2f& vector) {
        this->x = vector.x, this->y = vector.y;
        return *this;
    }
};

sf::Vector2f Point::vector() { return sf::Vector2f{ this->x, this->y }; }
sf::Vertex Point::vertex(sf::Color color) { return sf::Vertex{ this->vector(), color }; }
