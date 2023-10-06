#include <iostream>
#include <vector>
#include <string>
#include <SFML/Graphics.hpp>
#include "objects.hpp"
#include <thread>



int main(){
//Setup
    Ball* balls = new Ball[NUM_BALLS];//Tableau contenant toutes les balles

    randomBalls(balls); //Rempli le tableau de balles aux valeurs aléatoires
    std::vector<short unsigned int> grid[NUM_CASES_X][NUM_CASES_Y]; // Créer une grille 3D de taille variable sur l'axe z

    RenderWindow window(VideoMode(MAX_X, MAX_Y), "2D Simulation"); //Initialise la fenêtre de rendu

    //Créer le fond
    RectangleShape background(Vector2f(MAX_X, MAX_Y));
    background.setFillColor(Color(48, 48, 64, 128));

    //Créer le texte
    Font arial;
    arial.loadFromFile("../arial.ttf");
    Text text;
    text.setFont(arial);
    text.setCharacterSize(24);
    text.setFillColor(Color::White);
    RectangleShape rectangle(Vector2f(30, 30));
    rectangle.setFillColor(Color(48, 48, 64));


    //Initialise le timer pour afficher les fps
    Clock clock;
    Time time;

    //Boucle principale
    int frame=0;
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

        float delta;
        if(FIXED_DELTA){
            delta = 1.0f/300.0f;
        }
        else{
            delta = time.asSeconds();
        }

        window.draw(background);

        if (DYNAMIC_BACKGROUND) {
            RectangleShape dynamic_background[NUM_CASES_X][NUM_CASES_Y];
            for (int x = 0; x < NUM_CASES_X; x++) {
                for (int y = 0; y < NUM_CASES_Y; y++) {
                    dynamic_background[x][y].setPosition((float)x * CELL_SIZE, (float)y * CELL_SIZE);
                    dynamic_background[x][y].setSize(Vector2f(CELL_SIZE, CELL_SIZE));
                    dynamic_background[x][y].setOutlineThickness(2);
                    dynamic_background[x][y].setOutlineColor(Color(255, 255, 255));
                    if (grid[x][y].empty()) {
                        dynamic_background[x][y].setFillColor(Color::Black);
                    }
                    else if(grid[x][y].size()==1) {
                        dynamic_background[x][y].setFillColor(Color(64,64,64));
                    }
                    else if(grid[x][y].size()==2) {
                        dynamic_background[x][y].setFillColor(Color(128,128,128));
                    }
                    else if(grid[x][y].size()==3) {
                        dynamic_background[x][y].setFillColor(Color(196,196,196));
                    }
                    else {
                        dynamic_background[x][y].setFillColor(Color::White);
                    }
                    window.draw(dynamic_background[x][y]);
                }
            }
        }

        clearGrid(grid);
        placeBalls(balls,grid);

        mouseInteraction(delta, window, balls);
        updateGrid(delta, &window, balls);

        //Multithreading :
        std::thread collision_thread[NUM_THREADS];
        const int collision_thread_size = NUM_CASES_X / NUM_THREADS+1;
        for (int i=0; i<NUM_THREADS; i++){//On divise la grille de collision entre différents threads
            collision_thread[i] = std::thread(collisionGrid, balls, grid, i * collision_thread_size,(i + 1) * collision_thread_size);
        }
        for (auto & i : collision_thread){
            i.join();
        }


        //Affichage :
        for (int i=0; i < NUM_BALLS; i++){//On calcule les couleurs et affiche toutes les balles
            if(frame==1 || FIXED_COLORS) {//Les couleurs des balles changent en fonction de leur position
                float rgb_factor_x = MAX_X / 256.0f;
                float rgb_factor_y = MAX_Y / 256.0f;
                Vector2f pos = balls[i].getPos();
                balls[i].setColor((int) (pos.y / rgb_factor_y), (int) (pos.x / rgb_factor_x), (int) (255 - pos.y / rgb_factor_y));
            }
            window.draw(balls[i].circle);
        }

        text.setString(std::to_string((int)(1.0 / time.asSeconds())));
        window.draw(rectangle);
        window.draw(text);

        window.display();
        time = clock.getElapsedTime();
        //std::cout << 1.0 / time.asSeconds() << " fps\n";
    }

    delete[](balls);
}