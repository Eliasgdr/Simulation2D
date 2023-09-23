#ifndef SIMULATION2D_OBJECTS_H
#define SIMULATION2D_OBJECTS_H

#include <random>
#include "vectors.h"

#define MAX_X 1280.0f
#define MAX_Y 720.0f
#define MAX_SPEED 200.0f
#define FIXED_RADIUS true
#define MIN_RADIUS 1.0f
#define MAX_RADIUS 3.0f //Bizarre, je peux pas mettre à 2.0f
#define GRAVITY 0.0f //800
#define FRICTION 0.8f
#define NUM_BALLS 2000 //Bizarre je peux pas augmenter

const int CELL_SIZE = 5 * (int)MAX_RADIUS;
const int NUM_CASES_X = ((int)MAX_X / CELL_SIZE)+1;
const int NUM_CASES_Y = ((int)MAX_Y / CELL_SIZE)+1;

class Ball{
    private:
        CircleShape circle;
        Vector2f velocity;
        float mass;

    public:
        Ball(){
            circle = CircleShape();
            velocity = Vector2f();
            mass=0;
        }
        Ball(float x, float y, float vx, float vy, float r, Uint8 rgb[3]) {
            setPos(x,y);
            setVel(vx,vy);
            setRadius(r);
            setColor(rgb[0],rgb[1],rgb[2]);

        }

        Vector2f getPos(){
            return circle.getPosition();
        }

        Vector2f getVel(){
            return velocity;
        }

        float getRadius(){
            return circle.getRadius();
        }

        float getMass() const{
            return mass;
        }

        CircleShape getCircle(){
            return circle;
        }

        void setPos(float x, float y){
            if(_isnan(x) || _isnan(y)){
                std::cout<<"setPos : value error : x="<<x<<" y="<<y<<'\n';
                return;
            }
            circle.setPosition(x,y);
        }

        void setPos(Vector2f v){
            if(_isnan(v.x) || _isnan(v.y)){
                std::cout<<"setPos : value error : x="<<v.x<<" y="<<v.y<<'\n';
                return;
            }
            circle.setPosition(v.x,v.y);
        }

        void setVel(float x, float y){
            if(_isnan(x) || _isnan(y)){
                std::cout<<"setVel : value error : vx="<<x<<" vy="<<y<<'\n';
                return;
            }
            velocity.x = x;
            velocity.y = y;
        }

        void setVel(Vector2f v) {
            if (_isnan(v.x) || _isnan(v.y)) {
                std::cout << "setPos : value error : vx="<<v.x<<" vy="<<v.y<<'\n';
                return;
            }
            velocity.x = v.x;
            velocity.y = v.y;
        }

        void setRadius(float r){
            if(_isnan(r) || r < MIN_RADIUS || r > MAX_RADIUS){
                std::cout<<"setRadius : value error : r="<<r<<'\n';
                return;
            }
            circle.setRadius(r);
            circle.setOrigin(r, r);
            mass = 3.1415f*r*r;
        }

        void setColor(int r, int g, int b){
            if(_isnan(r) || _isnan(g) || _isnan(b) || r < 0 || g < 0 || b < 0 || r > 255 || g > 255 || b > 255){
                std::cout<<"setColor : value error : r="<<r<<" g="<<g<<" b="<<b<<'\n';
                return;
            }
            circle.setFillColor(Color(r,g,b));
        }

        void update(float delta_sec){
            if(delta_sec<=0){
                return;
            }
            Vector2f vel = getVel();
            Vector2f pos = getPos();
            float r = getRadius();
            vel.y += GRAVITY * delta_sec;
            pos += vel * delta_sec;
            if(pos.x - r < 0.0f){
                pos.x += 2.0f * (r - pos.x);
                vel.x *= FRICTION * -1.0f;
            }
            else if(pos.x + r >= MAX_X){
                pos.x -= 2.0f * (pos.x + r - MAX_X);
                vel.x *= FRICTION * -1.0f;
            }
            if(pos.y - r < 0.0f){
                pos.y += 2.0f * (r - pos.y);
                vel.y *= FRICTION * -1.0f;
            }
            else if(pos.y + r >= MAX_Y){
                pos.y -= 2.0f * (pos.y + r - MAX_Y);
                vel.y *= FRICTION * -1.0f;
            }
            if(_isnan(pos.x)){
                std::cout<<"update NaN : "<<delta_sec<<'\n';
            }
            setPos(pos.x,pos.y);
            setVel(vel.x,vel.y);
        }
};


bool collision(Ball* b1, Ball* b2) {
    float sum_radius = b1->getRadius() + b2->getRadius();
    Vector2f dist = b1->getPos() - b2->getPos();
    float dist_squared = dist.x * dist.x + dist.y * dist.y;
    if (dist_squared <= sum_radius * sum_radius) { //Comparer la distance au carré est plus rapide

        Vector2f normal = b2->getPos() - b1->getPos();
        float distance = length(normal);
        if (distance <= b1->getRadius() + b2->getRadius()) {
            Vector2f collision_normal = normalize(normal);

            // Relative velocity along the collision normal
            Vector2f relativeVelocity = b2->getVel() - b1->getVel();
            float collision_speed = dotProduct(relativeVelocity, collision_normal);

            // Impulse calculation
            float impulse = (2.0f * collision_speed) / (b1->getMass() + b2->getMass());

            // Apply impulse to velocities
            b1->setVel(b1->getVel() + impulse * b2->getMass() * collision_normal);
            b2->setVel(b2->getVel() - impulse * b1->getMass() * collision_normal);

            // Move the balls to avoid overlap
            float overlap = (b1->getRadius() + b2->getRadius()) - distance;
            b1->setPos(b1->getPos() - overlap * 0.5f * collision_normal);
            b2->setPos(b2->getPos() + overlap * 0.5f * collision_normal);
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
        if(FIXED_RADIUS){
            balls[i].setRadius(MIN_RADIUS);
        }
        else {
            std::uniform_real_distribution<float> dis_radius(MIN_RADIUS, MAX_RADIUS);
            balls[i].setRadius(dis_radius(gen));
        }

        std::uniform_real_distribution<float> dis_x(balls[i].getRadius(), MAX_X - balls[i].getRadius());
        std::uniform_real_distribution<float> dis_y(balls[i].getRadius(), MAX_Y - balls[i].getRadius());
        balls[i].setPos(dis_x(gen), dis_y(gen));

        std::uniform_real_distribution<float> dis_speed(-MAX_SPEED, MAX_SPEED);
        balls[i].setVel(dis_speed(gen), dis_speed(gen));

        std::uniform_int_distribution<Uint8> dis_color(160, 255);
        balls[i].setColor(dis_color(gen), dis_color(gen), dis_color(gen));
    }
}

void placeBalls(Ball balls[],std::vector<int> grid[NUM_CASES_X][NUM_CASES_Y]){
    int taille;
    for(int x=0;x<NUM_CASES_X;x++){
        for(int y=0;y<NUM_CASES_Y;y++){
            grid[x][y].clear();
            grid[x][y].shrink_to_fit();

        }
    }

    int grid_x, grid_y;
    for(int i=0;i<NUM_BALLS;i++){
        grid_x = (int)balls[i].getPos().x / CELL_SIZE;
        grid_y = (int)balls[i].getPos().y / CELL_SIZE;
        if(_isnan(balls[i].getPos().x)){
            std::cout<<"placeBalls NaN\n";
        }

        if(grid_x>=NUM_CASES_X){
            grid_x = NUM_CASES_X - 1;
        }
        if(grid_y>=NUM_CASES_Y){
            grid_y = NUM_CASES_Y - 1;
        }

        grid[grid_x][grid_y].push_back(i); //Rempli la grille des indices de toutes les balles en fonction de leur position

    }
}

#endif //SIMULATION2D_OBJECTS_H
