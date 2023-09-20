#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
#include "objects.h"


int main(){
//Setup
    Ball balls[NUM_BALLS]; //Tableau contenant toutes les balles

    randomBalls(balls); //Rempli le tableau de balles aux valeurs aléatoires
    RenderWindow window(sf::VideoMode(MAX_X, MAX_Y), "2D Simulation"); //Initialise la fenêtre de rendu

    std::vector<int> grid[NUM_CASES_X][NUM_CASES_Y]; // Créer une grille 3D de taille variable sur l'axe z

    //Créer le fond
    RectangleShape rectangle(Vector2f(MAX_X, MAX_Y));
    rectangle.setFillColor(Color(48, 48, 64));
    rectangle.setOutlineThickness(10);
    rectangle.setOutlineColor(Color(212, 212, 212));

    /*
    //Créer la grille visuelle
    int num_segments = NUM_CASES_X + NUM_CASES_Y - 2;
    sf::VertexArray visual_grid(sf::Lines, 2*(num_segments));
    // row separators
    for(int i=0; i < NUM_CASES_Y-1; i++){
        int r = i+1;
        auto rowY = (float) (CELL_SIZE*r);
        visual_grid[i*2].position = {0, rowY};
        visual_grid[i*2+1].position = {MAX_X, rowY};
    }
    // column separators

    for(int i=NUM_CASES_Y-1; i < num_segments; i++){
        int c = i-NUM_CASES_Y+2;
        auto colX = (float) (CELL_SIZE*c);
        visual_grid[i*2].position = {colX, 0};
        visual_grid[i*2+1].position = {colX, MAX_Y};
    }*/



    Clock clock;
    window.setFramerateLimit(60);
    Time t;

    const int relative_grid[8][2] = {{-1,-1}, //Utile plus tard pour parcourir les 3 cases à droite et en bas de la case actuelle
                                    {-1,0},
                                    {-1,1},
                                    {0,-1},
                                    {0,1},
                                    {1,-1},
                                    {1,0},
                                    {1,1}};

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
                    for(int r=0; r<8; r++){
                        std::vector<int> other_vector = grid[ix + relative_grid[r][0]][iy + relative_grid[r][1]]; //Parcours les 3 cases à droite et en bas
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
        //Changer les positions des balles dans la grille





        window.display();
        t = clock.getElapsedTime();
        std::cout << 1.0 / t.asSeconds() << " fps\n";
    }
}