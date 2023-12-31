# 2D Simulation Project

![Screenshot_Simulation](https://github.com/Eliasgdr/Simulation2D/assets/130139790/d8e02f37-02aa-45e6-9a9d-64c7b08e2c55)

This project is a 2D physics simulation of thousands of bouncing balls implemented in C++ using the SFML library. The simulation includes features such as gravitational forces, elastic collisions, and mouse interactions to attract or repel balls.

## Files

### `vectors.hpp`

This header file contains vector operations used in the simulation, such as dot product, vector length, and normalization.

### `objects.hpp`

This header file defines the `Ball` class, representing a bouncing ball in the simulation. It also includes functions for mouse interaction, updating the simulation grid, handling collisions, and placing balls in the grid.

### `constants.hpp`

This header file contains constants used throughout the project, such as simulation parameters, window dimensions, and ball properties.

### `main.cpp`

The main source file initializes the simulation, creates a window using SFML, and runs the main simulation loop. It includes features such as dynamic background, multithreading for collision detection, and the display of frames per second (fps).

## Compilation

Ensure you have SFML installed, and compile the project using your preferred C++ compiler. For example:

```bash
g++ main.cpp -o simulation -lsfml-graphics -lsfml-window -lsfml-system
```

## Usage

Run the compiled executable (`simulation` in the example above) to launch the 2D simulation. Interact with the simulation using the left mouse button to attract balls and the right mouse button to repel them.

## Configuration

Adjust simulation parameters and behavior by modifying constants in the `constants.hpp` file. You can experiment with different values to observe varying simulation outcomes.

Feel free to explore and modify the code to enhance or customize the simulation according to your preferences.
