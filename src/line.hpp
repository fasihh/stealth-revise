#pragma once
#include "point.hpp"

struct Line {
    Point a, b;

    Line(Point a = {}, Point b = {}) : a(a), b(b) {}

    sf::VertexArray vertexArray(sf::Color color = sf::Color::White) {
        sf::VertexArray arr(sf::Lines, 2);
        arr[0] = a.vertex(color), arr[1] = b.vertex(color);
        return arr;
    }
};
