#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
#include "objects.h"


int main(){
//Setup
    Ball balls[NUM_BALLS]; //Tableau contenant toutes les balles
    CircleShape circles[NUM_BALLS]; //Tableau contenant les données graphiques des balles

    randomBalls(balls); //Rempli le tableau de balles aux valeurs aléatoires

    RenderWindow window(sf::VideoMode(MAX_X, MAX_Y), "2D Simulation"); //Initialise la fenêtre de rendu

    std::vector<int> grid[NUM_CASES_X][NUM_CASES_Y]; // Créer une grille 3D de taille variable sur l'axe z

    //Créer le fond
    RectangleShape rectangle(Vector2f(MAX_X, MAX_Y));
    rectangle.setFillColor(Color(48, 48, 64));
    rectangle.setOutlineThickness(10);
    rectangle.setOutlineColor(Color(212, 212, 212));


    Clock clock;
    window.setFramerateLimit(60);
    Time t;

    const int relative_grid[3][2] = {{0,1}, //Utile plus tard pour parcourir les 3 cases à droite et en bas de la case actuelle
                                    {1,0},
                                    {1,1}};

//Boucle principale
    while (window.isOpen())
    {
        Event event;
        clock.restart().asSeconds();

        window.clear();
        window.draw(rectangle);

        placeBalls(balls,grid,circles);

        for(int ix=0;ix<NUM_CASES_X-1;ix++){ //Parcours toutes les cases de la grille
            for(int iy=0;iy<NUM_CASES_Y-1;iy++){
                std::vector<int> current_vector = grid[ix][iy];
                if (current_vector.empty()){
                    continue; //Passe à l'itération suivante si le vecteur est vide
                }
                for(int j=0;j<current_vector.size()-1;j++) {
                    int current_ball = current_vector.at(j);  //On parcourt en 3D la grille et sélectionne une balle
                    for(int k=j+1;k<current_vector.size();k++){
                        collision(&balls[current_ball],&balls[current_vector.at(k)]); // Collision entre la balle actuelle et les autres de la même case
                    }

                    //Collision avec les balles dans d'autres cases
                    for(auto relative_pos : relative_grid){
                        std::vector<int> other_vector = grid[ix + relative_pos[0]][iy + relative_pos[1]]; //Parcours les 3 cases à droite et en bas
                        if (other_vector.empty()){
                            continue; //Passe à l'itération suivante si le vecteur est vide
                        }
                        for(int other_ball : other_vector){
                            collision(&balls[current_ball], &balls[other_ball]);
                        }
                    }
                }
            }
        }

        for (int i=0;i<NUM_BALLS;i++) {
            circles[i].setPosition(balls[i].pos.x - balls[i].radius, balls[i].pos.y - balls[i].radius);
            window.draw(circles[i]);
            balls[i].update();
        }
        //Changer les positions des balles dans la grille


        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();
        }


        window.display();
        t = clock.getElapsedTime();
        std::cout << 1.0 / t.asSeconds() << " fps\n";
    }
}