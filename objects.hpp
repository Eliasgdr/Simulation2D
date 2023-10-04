#ifndef SIMULATION2D_OBJECTS_HPP
#define SIMULATION2D_OBJECTS_HPP

#include <random>
#include "constants.hpp"
#include "vectors.hpp"

class Ball{
    public:
        CircleShape circle;
        Vector2f velocity;
        float mass;

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

        Vector2f getVel() const{
            return velocity;
        }

        float getRadius() const{
            return circle.getRadius();
        }

        float getMass() const{
            return mass;
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

        void update(float delta_sec){//Change les valeurs de la balle pour la prochaine frame
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


bool collision(Ball* b1, Ball* b2) {//Vérifie si les 2 balles entrent en collision et change leurs positions et vitesses
    Vector2f normal = b2->getPos() - b1->getPos();
    float distance = length(normal);
    if (distance <= b1->getRadius() + b2->getRadius()) {//Vérifie que les balles sont en contact
        Vector2f collision_normal = normalize(normal);

        //Vélocité relative au vecteur normal
        Vector2f relativeVelocity = b2->getVel() - b1->getVel();
        float collision_speed = dotProduct(relativeVelocity, collision_normal);

        //Calcule la force de l'impact
        float impulse = (2.0f * collision_speed) / (b1->getMass() + b2->getMass());

        //Ajoute cette force aux vélocités
        b1->setVel(b1->getVel() + impulse * b2->getMass() * collision_normal);
        b2->setVel(b2->getVel() - impulse * b1->getMass() * collision_normal);

        //Déplace les balles pour qu'elles ne restent pas en contact
        float overlap = (b1->getRadius() + b2->getRadius()) - distance;
        b1->setPos(b1->getPos() - overlap * 0.5f * collision_normal);
        b2->setPos(b2->getPos() + overlap * 0.5f * collision_normal);

        return true;
    }
    return false;
}// https://www.vobarian.com/collisions/2dcollisions2.pdf

void randomBalls(Ball balls[]){//Remplit le tableau de balles de valeurs aléatoires
    std::random_device rd;
    std::mt19937 gen(rd());
    for(int i=0;i<NUM_BALLS;i++){
        if(FIXED_RADIUS){
            balls[i].setRadius(MAX_RADIUS);
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
    }
}

void placeBalls(Ball balls[],std::vector<int> grid[NUM_CASES_X][NUM_CASES_Y]){//Place les balles dans le tableau selon leur position
    for(int x=0;x<NUM_CASES_X;x++){//Vide la grille
        for(int y=0;y<NUM_CASES_Y;y++){
            grid[x][y].clear();
            grid[x][y].shrink_to_fit();

        }
    }

    int grid_x, grid_y;
    for(int i=0;i<NUM_BALLS;i++){
        //Calcule la position des balles dans la grille
        grid_x = (int)(balls[i].getPos().x) / CELL_SIZE;
        grid_y = (int)(balls[i].getPos().y) / CELL_SIZE;
        if(_isnan(balls[i].getPos().x)){
            std::cout<<"placeBalls NaN\n";
        }

        //Vérifie les valeurs
        if(grid_x>=NUM_CASES_X){
            grid_x = NUM_CASES_X - 1;
        }
        if(grid_y>=NUM_CASES_Y){
            grid_y = NUM_CASES_Y - 1;
        }

        grid[grid_x][grid_y].push_back(i); //Remplit la grille des indices de toutes les balles en fonction de leur position

    }
}

#endif //SIMULATION2D_OBJECTS_HPP
