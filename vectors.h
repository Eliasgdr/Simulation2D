#ifndef SIMULATION2D_VECTORS_H
#define SIMULATION2D_VECTORS_H

#include <cmath>
#include <SFML/System/Vector2.hpp>

using namespace sf;

float dotProduct(Vector2f v1, Vector2f v2){
    return v1.x * v2.x + v1.y * v2.y;
}

float length(Vector2f v){
    return sqrtf(v.x * v.x + v.y * v.y);
}

Vector2f normalize(Vector2f v){
    float len = length(v);
    if (len > 0.0f) {
        return {v.x / len, v.y / len};
    } else {
        return {0.0f, 0.0f};
    }
}

#endif //SIMULATION2D_VECTORS_H
