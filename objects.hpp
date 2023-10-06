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
            if(std::isnan(x) || std::isnan(y)){
                std::cerr << "setPos: value error: x=" << x << " y=" << y << '\n';
                return;
            }
            circle.setPosition(x,y);
        }

        void setPos(Vector2f v){
            if(std::isnan(v.x) || std::isnan(v.y)){
                std::cerr << "setPos: value error: x=" << v.x << " y=" << v.y << '\n';
                return;
            }
            circle.setPosition(v.x,v.y);
        }

        void setVel(float x, float y){
            if(std::isnan(x) || std::isnan(y)){
                std::cerr << "setVel: value error: x=" << x << " y=" << y << '\n';
                return;
            }
            velocity.x = x;
            velocity.y = y;
        }

        void setVel(Vector2f v) {
            if(std::isnan(v.x) || std::isnan(v.y)){
                std::cerr << "setVel: value error: x=" << v.x << " y=" << v.y << '\n';
                return;
            }
            velocity.x = v.x;
            velocity.y = v.y;
        }

        void setRadius(float r){
            if(std::isnan(r) || r < MIN_RADIUS || r > MAX_RADIUS){
                std::cerr << "setRadius: value error: r=" << r << '\n';
                return;
            }
            circle.setRadius(r);
            circle.setOrigin(r, r);
            mass = M_PI*r*r;
        }

        void setColor(int r, int g, int b){
            if(std::isnan(r) || std::isnan(g) || std::isnan(b) || r < 0 || g < 0 || b < 0 || r > 255 || g > 255 || b > 255){
                std::cerr << "setColor: value error: r=" << r << " g=" << g <<" b=" << b << '\n';
                return;
            }
            circle.setFillColor(Color(r,g,b));
        }

        void update(const float delta_sec){//Change les valeurs de la balle pour la prochaine frame
            if(delta_sec==0.0f){
                return;
            }
            Vector2f vel = getVel();
            Vector2f pos = getPos();
            const float r = getRadius();
            vel.y += GRAVITY * delta_sec;
            pos += vel * delta_sec;

            if(pos.x - r < 0.0f){
                pos.x += 2.0f * (r - pos.x);
                vel.x *= ELASTICITY *-1.0f;
            }
            else if(pos.x + r >= MAX_X){
                pos.x -= 2.0f * (pos.x + r - MAX_X);
                vel.x *= ELASTICITY * -1.0f;
            }
            if(pos.y - r < 0.0f){
                pos.y += 2.0f * (r - pos.y);
                vel.y *= ELASTICITY * -1.0f;
            }
            else if(pos.y + r >= MAX_Y){
                pos.y -= 2.0f * (pos.y + r - MAX_Y);
                vel.y *= ELASTICITY -1.0f;
            }

            setPos(pos);
            setVel(FRICTION * vel);
        }

};

void mouseInteraction(const float delta_sec, RenderWindow& win, Ball balls[], int start=0, int end=NUM_BALLS) {//Clic gauche pour attirer et clic droit pour repousser les balles
    if(start<0){
        start=0;
    }
    if(end>NUM_BALLS){
        end=NUM_BALLS;
    }
    if (Mouse::isButtonPressed(Mouse::Left) || Mouse::isButtonPressed(Mouse::Right)) {
        const Vector2f mouse_pos = (Vector2f) Mouse::getPosition(win);
        for (int i = start; i < end; i++) {
            const Vector2f normal = mouse_pos - balls[i].getPos();
            const float distance = length(normal);
            float impulse=10000.0f;
            if (Mouse::isButtonPressed(Mouse::Right)) {
                impulse *= -1.0f;
            }
            balls[i].setVel(balls[i].getVel() + (impulse / sqrtf(distance + 0.5f)) * delta_sec * normalize(normal));
        }
    }
}

void updateGrid(float delta_sec, RenderWindow* win, Ball balls[], int start= 0, int end= NUM_BALLS){
    if(start<0){
        start=0;
    }
    if(end>NUM_BALLS){
        end=NUM_BALLS;
    }
    for (int i=start; i<end; i++){
        balls[i].update(delta_sec);
    }
}

bool collision(Ball* b1, Ball* b2) {//Vérifie si les 2 balles entrent en collision et change leurs positions et vitesses
    const Vector2f normal = b2->getPos() - b1->getPos();
    const float distance = length(normal);
    if (distance <= b1->getRadius() + b2->getRadius()) {//Vérifie que les balles sont en contact
        const Vector2f collision_normal = normalize(normal);

        //Vélocité relative au vecteur normal
        const Vector2f relativeVelocity = b2->getVel() - b1->getVel();
        const float collision_speed = dotProduct(relativeVelocity, collision_normal);

        //Calcule la force de l'impact
        const float impulse = (2.0f * collision_speed) / (b1->getMass() + b2->getMass());

        //Ajoute cette force aux vélocités
        b1->setVel(ELASTICITY * (b1->getVel() + impulse * b2->getMass() * collision_normal));
        b2->setVel(ELASTICITY * (b2->getVel() - impulse * b1->getMass() * collision_normal));

        //Déplace les balles pour qu'elles ne restent pas en contact
        const float overlap = (b1->getRadius() + b2->getRadius()) - distance;
        b1->setPos(b1->getPos() - overlap * 0.5f * collision_normal);
        b2->setPos(b2->getPos() + overlap * 0.5f * collision_normal);

        return true;
    }
    return false;
}// https://www.vobarian.com/collisions/2dcollisions2.pdf

void collisionGrid(Ball balls[], std::vector<unsigned short> grid[NUM_CASES_X][NUM_CASES_Y], int start= 0, int end= NUM_CASES_X){
    if(start<0){
        start=0;
    }
    if(end>NUM_CASES_X){
        end=NUM_CASES_X;
    }
     for (int ix = start; ix < end; ix++) {//Parcourir la grille
            for (int iy = 0; iy < NUM_CASES_Y; iy++) {
                std::vector<short unsigned int>& current_vector = grid[ix][iy];
                if (current_vector.size() <= 1) {
                    continue; //Passer s'il y a 0 ou 1 balle dans cette case
                }
                for (int current_ball : current_vector) {
                    for (int rx = -1; rx <= 1; rx++) {//Parcourir un carré de 3x3 autour de la case actuelle
                        for (int ry = -1; ry <= 1; ry++) {
                            if (ix + rx < 0 || ix + rx >= NUM_CASES_X || iy + ry < 0 || iy + ry >= NUM_CASES_Y) {//Passer si on sort de la grille
                                continue;
                            }
                            std::vector<short unsigned int>& other_vector = grid[ix + rx][iy + ry];
                            for (int other_ball : other_vector) {
                                if (current_ball != other_ball) {
                                    collision(&balls[current_ball], &balls[other_ball]);//On essaye les collisions entre toutes les balles assez proches
                                }
                            }
                        }
                    }
                }
            }
        }
}

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

        std::uniform_real_distribution<float> dis_speed(-START_SPEED, START_SPEED);
        balls[i].setVel(dis_speed(gen), dis_speed(gen));
    }
}

void clearGrid(std::vector<short unsigned int> grid[NUM_CASES_X][NUM_CASES_Y]){
    for(int x=0;x<NUM_CASES_X;x++){//Vide la grille
        for(int y=0;y<NUM_CASES_Y;y++){
            grid[x][y].clear();
            grid[x][y].shrink_to_fit();

        }
    }
}

void placeBalls(Ball balls[],std::vector<short unsigned int> grid[NUM_CASES_X][NUM_CASES_Y], int start = 0, int end = NUM_BALLS){//Place les balles dans le tableau selon leur position
    if(start<0){
        start=0;
    }
    if(end>NUM_BALLS){
        end=NUM_BALLS;
    }
    int grid_x, grid_y;
    for(int i=start;i<end;i++){
        //Calcule la position des balles dans la grille
        grid_x = (int)(balls[i].getPos().x) / CELL_SIZE;
        grid_y = (int)(balls[i].getPos().y) / CELL_SIZE;

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
