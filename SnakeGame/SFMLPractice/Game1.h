#pragma once
#include <iostream>
#include <vector>
#include <ctime>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>

using namespace sf;

/* Class acts as the game engine */
class Game1 {
private:
    // Variables
    RenderWindow* window;
    VideoMode videoMode;
    Event ev;
    bool endGame;

    // Snake and Apple
    struct point { int x = 0; int y = 0; };
    std::vector<point> snake; // Snake segments
    point apple; // Apple position
    int dir; // Snake direction
    bool grow; // Flag to grow snake when it eats an apple

    // Private functions
    void inVar(); // Initialize variables
    void createWindow(); // Create game window
    void spawnApple(); // Spawn apple at a random position
    bool checkCollision(); // Check for snake collision

public:
    // Constructors & Destructors
    Game1();
    ~Game1();

    // Accessors
    const bool running() const;
    const bool getEndGame() const;

    // Functions
    void takeEvents(); // Handle input events
    void updateSnake(); // Update snake movement and logic
    void update(); // Update game state
    void render(); // Render game objects
};
