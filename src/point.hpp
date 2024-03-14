#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>

struct Point {
    float x, y;

    Point(float x = 0.f, float y = 0.f) : x(x), y(y) {};
    sf::Vector2f vector();
    sf::Vertex vertex(sf::Color color = sf::Color::White);

    // test
    void display() {
        std::cout << "{ " << x << ", " << y << " }" << std::endl;
    }
};

sf::Vector2f Point::vector() { return sf::Vector2f{ this->x, this->y }; }
sf::Vertex Point::vertex(sf::Color color) { return sf::Vertex{ this->vector(), color }; }
