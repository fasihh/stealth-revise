#pragma once
#include <SFML/System/Vector2.hpp>

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

    static sf::Vector2f normalize(sf::Vector2f vect) {
        float invMag = Utils::q_rsqrt(vect.x*vect.x + vect.y*vect.y);
        
        return sf::Vector2f{ vect.x*invMag, vect.y*invMag };
    }
};