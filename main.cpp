/*
A faire:
- Tester Verlet
-
 */

#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
#include "objects.h"

#define SUB_STEPS 1

int main(){
//Setup
    Ball balls[NUM_BALLS]; //Tableau contenant toutes les balles

    randomBalls(balls); //Rempli le tableau de balles aux valeurs aléatoires
    /*
    //Test avec conditions fixes
    Uint8 red[3] = {255,0,0};
    Uint8 green[3] = {0,255,0};
    Uint8 blue[3] = {0,0,255};
    balls[0] = Ball(100.0, 500.0, 0.0, 0.0, 20.0, red);
    balls[1] = Ball(100.0, 100.0, 0.0, 0.0, 30.0, green);
    balls[2] = Ball(500.0, 500.0, -50.0, 0.0, 20.0, blue);
     */
    RenderWindow window(sf::VideoMode(MAX_X, MAX_Y), "2D Simulation"); //Initialise la fenêtre de rendu

    std::vector<int> grid[NUM_CASES_X][NUM_CASES_Y]; // Créer une grille 3D de taille variable sur l'axe z

    //Créer le fond
    RectangleShape rectangle(Vector2f(MAX_X, MAX_Y));
    rectangle.setFillColor(Color(48, 48, 64));
    rectangle.setOutlineThickness(10);
    rectangle.setOutlineColor(Color(212, 212, 212));
    RectangleShape dynamic_background[NUM_CASES_X][NUM_CASES_Y];



    Clock clock;
    //window.setFramerateLimit(60);
    Time t;


//Boucle principale
    int frame = 0;
    while (window.isOpen())
    {
        frame++;
        Event event{};
        clock.restart().asSeconds();

        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();
        }

        window.clear();
        window.draw(rectangle);
        //window.draw(visual_grid);

        placeBalls(balls,grid);

        /*
        //Fond dynamique :
        for(int x=0;x<NUM_CASES_X;x++){
            for(int y=0;y<NUM_CASES_Y;y++){
                dynamic_background[x][y].setPosition(x*CELL_SIZE, y*CELL_SIZE);
                dynamic_background[x][y].setSize(sf::Vector2f(CELL_SIZE, CELL_SIZE));
                dynamic_background[x][y].setOutlineThickness(2);
                dynamic_background[x][y].setOutlineColor(Color(255, 255, 255));
                if(grid[x][y].empty()){
                    dynamic_background[x][y].setFillColor(Color(0, 0, 0));
                }
                else {
                    dynamic_background[x][y].setFillColor(Color(0, 0, 200));
                }
                window.draw(dynamic_background[x][y]);
            }
        }*/

        int rx_max, ry_max, rx_min, ry_min;
        for(int step=0;step<SUB_STEPS;step++) {
            for (int ix = 0; ix < NUM_CASES_X; ix++) { //Parcours toutes les cases de la grille
                for (int iy = 0; iy < NUM_CASES_Y; iy++) {
                    std::vector<int> current_vector = grid[ix][iy];
                    if (current_vector.size() <= 1) {
                        continue; //Passe à l'itération suivante si le vecteur contient une balle ou moins
                    }
                    for (int j = 0; j < current_vector.size() - 1; j++) {
                        int current_ball = current_vector.at(j);  //On parcourt en 3D la grille et sélectionne une balle
                        for (int k = j + 1; k < current_vector.size(); k++) {
                            collision(&balls[current_ball], &balls[current_vector.at(
                                    k)]); // Collision entre la balle actuelle et les autres de la même case
                        }

                        //Collision avec les balles dans d'autres cases
                        std::vector<int> other_vector;
                        rx_max = 1;
                        if (ix == NUM_CASES_X-1) {
                            rx_max = 0;
                        }
                        ry_max = 1;
                        if (iy == NUM_CASES_Y-1) {
                            ry_max = 0;
                        }
                        rx_min = -1;
                        if (ix == 0) {
                            rx_min = 0;
                        }
                        ry_min = -1;
                        if (iy == 0) {
                            ry_min = 0;
                        }
                        for (int rx = rx_min; rx < rx_max; rx++) {
                            for (int ry = ry_min; ry < ry_max; ry++) {
                                other_vector = grid[ix + rx][iy + ry]; //Parcours les 3 cases à droite et en bas
                                if (other_vector.empty()) {
                                    continue; //Passe à l'itération suivante si le vecteur est vide
                                }
                                for (int other_ball: other_vector) {
                                    collision(&balls[current_ball], &balls[other_ball]);
                                }
                            }
                        }
                    }
                }
            }
        }

        /*
        for (int i=0;i<NUM_BALLS;i++){
            for (int j=i+1;j<NUM_BALLS;j++){
                collision(&balls[i],&balls[j]);
            }
        }*/

        for (int a=0;a<NUM_BALLS;a++){
            balls[a].update(t.asSeconds());
            //std::cout<<"b.x = "<<balls[a].pos.x <<" c.x = "<<circles[a].getPosition().x<<'\n';
            window.draw(balls[a].getCircle());
        }



        window.display();
        t = clock.getElapsedTime();
        std::cout << 1.0 / t.asSeconds() << " fps\n";
    }
}