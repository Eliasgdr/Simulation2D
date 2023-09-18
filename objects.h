#ifndef SIMULATION2D_OBJECTS_H
#define SIMULATION2D_OBJECTS_H

#include <random>
#include "vectors.h"

#define MAX_X 1280.0f
#define MAX_Y 720.0f
#define MAX_SPEED 5.0f
#define MIN_RADIUS 5.0f
#define MAX_RADIUS 20.0f //Bizarre je peux pas mettre à 2.0f
#define GRAVITY 0.0f
#define FRICTION 0.8f
#define NUM_BALLS 200 //Bizarre je peux pas augmenter

const int NUM_CASES_X = MAX_X / (int)MAX_RADIUS;
const int NUM_CASES_Y = MAX_Y / (int)MAX_RADIUS;


class Point{
    public:
        Vector2f pos;
        Vector2f speed;

        Point(){
            pos = Vector2f();
            speed = Vector2f();
        }

        Point(float x, float y, float sx, float sy){
            pos = Vector2f(x,y);
            speed = Vector2f(sx,sy);
        }
};

class Ball : public Point {
    public:
        float radius;
        float mass;
        Uint8 color[3]{};

        Ball() : Point() {
            radius = 0.0f;
            mass = 0.0f;
            color[0] = 0;
            color[1] = 0;
            color[2] = 0;
        }

        Ball(float x, float y, float sx, float sy, float r, Uint8 rgb[3]) : Point(x, y, sx, sy) {
            radius = r;
            mass = 3.14f*r*r;
            color[0] = rgb[0];
            color[1] = rgb[1];
            color[2] = rgb[2];

        }

        void update(){
            speed.y += GRAVITY;
            pos += speed;
            if(pos.x - radius < 0.0f){
                pos.x += 2.0f * (radius - pos.x);
                speed.x *= FRICTION * -1.0f;
            }
            else if(pos.x + radius >= MAX_X){
                pos.x -= 2.0f * (pos.x + radius - MAX_X);
                speed.x *= FRICTION * -1.0f;
            }
            if(pos.y - radius < 0.0f){
                pos.y += 2.0f * (radius - pos.y);
                speed.y *= FRICTION * -1.0f;
            }
            else if(pos.y + radius >= MAX_Y){
                pos.y -= 2.0f * (pos.y + radius - MAX_Y);
                speed.y *= FRICTION * -1.0f;
            }

        }
};


bool collision(Ball* b1, Ball* b2){
    float sum_radius = b1->radius + b2->radius;
    Vector2f dist = b1->pos - b2->pos;
    float dist_squared = dist.x * dist.x + dist.y * dist.y;
    if( dist_squared <= sum_radius * sum_radius){ //Comparer la distance au carré est plus rapide

        Vector2f normal = b2->pos - b1->pos;
        float distance = length(normal);
        if (distance <= b1->radius + b2->radius) {
            Vector2f collision_normal = normalize(normal);

            // Relative velocity along the collision normal
            Vector2f relativeVelocity = b2->speed - b1->speed;
            float collision_speed = dotProduct(relativeVelocity, collision_normal);

            // Impulse calculation
            float impulse = (2.0f * collision_speed) / (b1->mass + b2->mass);

            // Apply impulse to velocities
            b1->speed += impulse * b2->mass * collision_normal;
            b2->speed -= impulse * b1->mass * collision_normal;

            // Move the balls to avoid overlap
            float overlap = (b1->radius + b2->radius) - distance;
            b1->pos -= overlap * 0.5f * collision_normal;
            b2->pos += overlap * 0.5f * collision_normal;
        }
        // https://www.vobarian.com/collisions/2dcollisions2.pdf
        return true;
    }
    return false;
}

void randomBalls(Ball balls[]){
    std::random_device rd;
    std::mt19937 gen(rd());
    for(int i=0;i<NUM_BALLS;i++){
        std::uniform_real_distribution<float> dis_radius(MIN_RADIUS, MAX_RADIUS);
        balls[i].radius = dis_radius(gen);
        balls[i].mass = M_PI * balls[i].radius * balls[i].radius;

        std::uniform_real_distribution<float> dis_x(balls[i].radius, MAX_X - balls[i].radius);
        std::uniform_real_distribution<float> dis_y(balls[i].radius, MAX_Y - balls[i].radius);
        balls[i].pos = Vector2f(dis_x(gen), dis_y(gen));

        std::uniform_real_distribution<float> dis_speed(-MAX_SPEED, MAX_SPEED);
        balls[i].speed = Vector2f(dis_speed(gen), dis_speed(gen));

        std::uniform_int_distribution<Uint8> dis_color(160, 255);
        balls[i].color[0] = dis_color(gen);
        balls[i].color[1] = dis_color(gen);
        balls[i].color[2] = dis_color(gen);
    }
}

void placeBalls(Ball balls[],std::vector<int> grid[NUM_CASES_X][NUM_CASES_Y], CircleShape circles[]){
    int taille;
    for(int x=0;x<NUM_CASES_X;x++){
        for(int y=0;y<NUM_CASES_Y;y++){
            taille = grid[x][y].size();
            grid[x][y].clear();
            grid[x][y].shrink_to_fit();

        }
    }

    int grid_x, grid_y;
    for(int i=0;i<NUM_BALLS;i++){
        grid_x = (int)balls[i].pos.x / (int)MAX_RADIUS;
        grid_y = (int)balls[i].pos.y / (int)MAX_RADIUS;
        if(grid_x>=NUM_CASES_X){
            grid_x = NUM_CASES_X - 1;
        }
        if(grid_y>=NUM_CASES_X){
            grid_y = NUM_CASES_X - 1;
        }
        grid[grid_x][grid_y].push_back(i); //Rempli la grille des indices de toutes les balles en fonction de leur position
        circles[i].setRadius(balls[i].radius);
        circles[i].setFillColor(Color(balls[i].color[0],balls[i].color[1],balls[i].color[2]));
    }
}

#endif //SIMULATION2D_OBJECTS_H
