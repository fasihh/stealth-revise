#pragma once
#include <SFML/System/Vector2.hpp>
#include <cmath>

// static class for utility functions
class Utils {
public: 
    // optimized inverse square root algorithm from quake 3
    static float q_rsqrt(float number) {
        if (!number) return 1.f;

        float x2 = number * 0.5F;
        float y = number;
        long i = *(long *)&y;
        i = 0x5f3759df - ( i >> 1 );
        y = *(float *) &i;
        y *= ( 1.5f - ( x2 * y * y ) );

        return y;
    }

    static float distance(sf::Vector2f a, sf::Vector2f b) {
        float dy = a.y-b.y, dx = a.x-b.x;
        return 1/Utils::q_rsqrt(dy*dy + dx*dx);
    }

    static Intersection checkIntersection(Line ray, std::vector<Object> objects, float angle = 0.f) {
        Intersection closest(false);
        for (Object object : objects) {
            for (Line segment : object.getSegments()) {
                Intersection intersect = Utils::getIntersection(ray, segment);
                intersect.angle = angle;
                if (!intersect.status) continue; // if doesn't intersect then pass
                // if no previous intersection OR current intersection closer to previous closest
                if (!closest.status || intersect.T1 < closest.T1)
                    closest = intersect;
            }
        }
        return closest;
    }

    static Intersection getIntersection(const Line ray, const Line segment) {
        float r_px = ray.a.x; // ray x
        float r_py = ray.a.y; // ray y
        float r_dx = ray.b.x-ray.a.x; // direction of ray x
        float r_dy = ray.b.y-ray.a.y; // direction of ray y

        float s_px = segment.a.x; // segment x
        float s_py = segment.a.y; // segment y
        float s_dx = segment.b.x-segment.a.x; // direction of segment x
        float s_dy = segment.b.y-segment.a.y; // direction of segment y

        // finding unit vector of both ray and segment to check if they are parallel
        float r_mag = std::sqrt(r_dx*r_dx+r_dy*r_dy);
        float s_mag = std::sqrt(s_dx*s_dx+s_dy*s_dy);
        // in case they are, return a false status
        if(r_dx/r_mag==s_dx/s_mag && r_dy/r_mag==s_dy/s_mag)
            return { false };

        // SOLVE FOR T1 & T2
        // r_px+r_dx*T1 = s_px+s_dx*T2 && r_py+r_dy*T1 = s_py+s_dy*T2
        // ==> T1 = (s_px+s_dx*T2-r_px)/r_dx = (s_py+s_dy*T2-r_py)/r_dy
        // ==> s_px*r_dy + s_dx*T2*r_dy - r_px*r_dy = s_py*r_dx + s_dy*T2*r_dx - r_py*r_dx
        // ==> T2 = (r_dx*(s_py-r_py) + r_dy*(r_px-s_px))/(s_dx*r_dy - s_dy*r_dx)
        float T2 = (r_dx*(s_py-r_py) + r_dy*(r_px-s_px))/(s_dx*r_dy - s_dy*r_dx);
        float T1 = (s_px+s_dx*T2-r_px)/r_dx;

        // conditions for intersection
        if(T1<0) return { false };
        if(T2<0 || T2>1) return { false };

        // if all conditions satisfy, return an intersection with true status, point and the value T
        return {
            true,
            r_px+r_dx*T1,
            r_py+r_dy*T1,
            T1
        };
    }

    static sf::Vector2f normalize(sf::Vector2f vect) {
        float invMag = Utils::q_rsqrt(vect.x*vect.x + vect.y*vect.y);
        
        return sf::Vector2f{ vect.x*invMag, vect.y*invMag };
    }
};