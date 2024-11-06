#include <iostream>
#include "Game1.h"
#include <ctime>

int main() {
    // Initialize random seed
    srand(static_cast<unsigned>(time(NULL)));

    // Create the game
    Game1 game;

    // Main game loop
    while (game.running() && !game.getEndGame()) {
        // Update game state
        game.update();

     // Render game elements
        game.render();
    }

    return 0;
}
