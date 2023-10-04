#ifndef SIMULATION2D_VECTORS_HPP
#define SIMULATION2D_VECTORS_HPP

#include <cmath>
#include <SFML/System/Vector2.hpp>

using namespace sf;

float dotProduct(Vector2f v1, Vector2f v2){//Produit Scalaire
    return v1.x * v2.x + v1.y * v2.y;
}

float length(Vector2f v){//Longueur du vecteur
    return sqrtf(v.x * v.x + v.y * v.y);
}

Vector2f normalize(Vector2f v){//Renvoie le vecteur de longueur 1
    float len = length(v);
    if (len > 0.0f) {
        return {v.x / len, v.y / len};
    } else {
        return {0.0f, 0.0f};
    }
}

#endif //SIMULATION2D_VECTORS_HPP
