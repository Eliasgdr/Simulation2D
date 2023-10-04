#ifndef SIMULATION2D_CONSTANTS_HPP
#define SIMULATION2D_CONSTANTS_HPP

//objects.h
#define MAX_X 1280.0f //1280.0f
#define MAX_Y 720.0f //720.0f
#define MAX_SPEED 100.0f //200.0f
#define FIXED_RADIUS false
#define MIN_RADIUS 1.0f
#define MAX_RADIUS 2.0f
#define GRAVITY 0.0f //400.0f
#define FRICTION 0.8f //0.9f
#define NUM_BALLS 10000

const int CELL_SIZE = std::max(3 * (int)MAX_RADIUS, 12);
const int NUM_CASES_X = ((int)MAX_X / CELL_SIZE)+1;
const int NUM_CASES_Y = ((int)MAX_Y / CELL_SIZE)+1;

//main.cpp
#define SUB_STEPS 1
#define DYNAMIC_BACKGROUND false
#define FIXED_COLORS true


#endif //SIMULATION2D_CONSTANTS_HPP
