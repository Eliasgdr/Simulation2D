/*
A faire:
- Ajouter la gravité
 */

#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
#include "objects.hpp"



int main(){
//Setup
    Ball* balls = new Ball[NUM_BALLS];//Tableau contenant toutes les balles

    randomBalls(balls); //Rempli le tableau de balles aux valeurs aléatoires
    std::vector<int> grid[NUM_CASES_X][NUM_CASES_Y]; // Créer une grille 3D de taille variable sur l'axe z
    //Test avec conditions fixes :
    /*
    Uint8 red[3] = {255,0,0};
    Uint8 green[3] = {0,255,0};
    Uint8 blue[3] = {0,0,255};
    balls[0] = Ball(100.0, 500.0, 0.0, 0.0, 20.0, red);
    balls[1] = Ball(100.0, 100.0, 0.0, 0.0, 30.0, green);
    balls[2] = Ball(500.0, 500.0, -50.0, 0.0, 20.0, blue);
     */

    RenderWindow window(sf::VideoMode(MAX_X, MAX_Y), "2D Simulation"); //Initialise la fenêtre de rendu

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
    while (window.isOpen())
    {
        Event event{};
        clock.restart().asSeconds();

        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();
        }

        window.clear();
        window.draw(rectangle);

        placeBalls(balls,grid);

        if (DYNAMIC_BACKGROUND) {
            for (int x = 0; x < NUM_CASES_X; x++) {
                for (int y = 0; y < NUM_CASES_Y; y++) {
                    dynamic_background[x][y].setPosition((float)x * CELL_SIZE, (float)y * CELL_SIZE);
                    dynamic_background[x][y].setSize(sf::Vector2f(CELL_SIZE, CELL_SIZE));
                    dynamic_background[x][y].setOutlineThickness(2);
                    dynamic_background[x][y].setOutlineColor(Color(255, 255, 255));
                    if (grid[x][y].empty()) {
                        dynamic_background[x][y].setFillColor(Color(0, 0, 0));
                    } else {
                        dynamic_background[x][y].setFillColor(Color(0, 0, 200));
                    }
                    window.draw(dynamic_background[x][y]);
                }
            }
        }

        for (int step = 0; step < SUB_STEPS; step++) {
            for (int ix = 0; ix < NUM_CASES_X; ix++) {
                for (int iy = 0; iy < NUM_CASES_Y; iy++) {
                    std::vector<int>& current_vector = grid[ix][iy]; // Use a reference to avoid unnecessary copying
                    if (current_vector.size() <= 1) {
                        continue; // Skip if there is only one or zero balls in this cell
                    }
                    for (int current_ball : current_vector) {
                        for (int rx = -1; rx <= 1; rx++) {
                            for (int ry = -1; ry <= 1; ry++) {
                                if (ix + rx < 0 || ix + rx >= NUM_CASES_X || iy + ry < 0 || iy + ry >= NUM_CASES_Y) {
                                    continue;
                                }
                                std::vector<int>& other_vector = grid[ix + rx][iy + ry];
                                for (int other_ball : other_vector) {
                                    if (current_ball != other_ball) {
                                        collision(&balls[current_ball], &balls[other_ball]);
                                    }
                                }
                            }
                        }
                    }
                }
            }
            /*
            for (int i=0;i<NUM_BALLS;i++){ Résolveur de collisions plus basique
                for (int j=i+1;j<NUM_BALLS;j++){
                    collision(&balls[i],&balls[j]);
                }
            }*/
        }



        for (int a=0;a<NUM_BALLS;a++){
            balls[a].update(t.asSeconds());
            window.draw(balls[a].circle);
        }



        window.display();
        t = clock.getElapsedTime();
        std::cout << 1.0 / t.asSeconds() << " fps\n";
    }
    delete[](balls);
}