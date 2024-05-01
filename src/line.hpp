#pragma once
#include "point.hpp"
#include <cmath>

struct Line {
    Point a, b;

    Line(Point a = {}, Point b = {}) : a(a), b(b) {}

    sf::VertexArray vertexArray(sf::Color color = sf::Color::White) {
        sf::VertexArray arr(sf::Lines, 2);
        arr[0] = a.vertex(color), arr[1] = b.vertex(color);
        return arr;
    }

    float distance() {
        float dx = a.x - b.x, dy = a.y - b.y;
        return std::sqrt(dx*dx + dy*dy);
    }

    bool operator >= (Line other) { return distance() >= other.distance(); }
};
