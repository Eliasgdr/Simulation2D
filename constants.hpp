#ifndef SIMULATION2D_CONSTANTS_HPP
#define SIMULATION2D_CONSTANTS_HPP

//objects.h
#define MAX_X 1280.0f //1280.0f
#define MAX_Y 720.0f //720.0f
#define START_SPEED 200.0f //200.0f
#define FIXED_RADIUS false
#define MIN_RADIUS 4.0f
#define MAX_RADIUS 4.0f
#define GRAVITY 0.0f //300.0f
#define FRICTION 0.9999f //0.9999f
#define ELASTICITY 0.99f //0.999f
#define NUM_BALLS 10000

const int CELL_SIZE = std::max(3 * (int)MAX_RADIUS, (int)MAX_X / 100);
const int NUM_CASES_X = ((int)MAX_X / CELL_SIZE)+1;
const int NUM_CASES_Y = ((int)MAX_Y / CELL_SIZE)+1;

//main.cpp
#define NUM_THREADS 12
#define FIXED_DELTA true
#define DYNAMIC_BACKGROUND false
#define FIXED_COLORS false


#endif //SIMULATION2D_CONSTANTS_HPP
