#pragma once
#include "point.hpp"

struct Intersection {
    bool status;
    Point point;
    float T1, angle;
    Intersection(
        bool s,
        float x = 0,
        float y = 0,
        float T1 = 0.f,
        float a = 0.f
    ) 
    : status(s), point(x, y), angle(a), T1(T1) {}

    bool operator !() { return !status; }
};