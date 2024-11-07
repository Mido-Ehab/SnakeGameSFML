#pragma once
#include <iostream>
#include <vector>
#include <ctime>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include "Map.h"
#include"MovingObstacle.h"


using namespace sf;

/* Class acts as the game engine */
class Game1 {
private:
    // --------------------------------Variables
    RenderWindow* window;
    VideoMode videoMode;
    Map map;
    
    Event ev;
    
    Font fonts;
    Text uiText;

    bool endGame;

    // Snake and Apple
    struct point { int x = 0; int y = 0; };
    std::vector<point> snake; // Snake segments
   
    std::vector<MovingObstacle> obstacles;
    std::vector<MovingObstacle> obstaclesPoints;


    point apple; // Apple position
    int dir; // Snake direction
    bool grow; // Flag to grow snake when it eats an apple

    //----------------------------------------pickups  
        unsigned points;
        int health;
    //--------bonus-----$$$$$$$$$$$$$$$$$
        point BonusApple; // var from struct datatype
        bool bonusActive; // trigger to make the pickup appears from time to time
    //--------Time----------
        Clock gameClock;
        Clock timerClock;
        int Timer;
        point timePickup; // var from struct datatype
        bool Timeboost;   // trigger to make the pickup appears from time to time



    // Private functions
    void inVar(); // Initialize variables
    void createWindow(); // Create game window
    void spawnApple(); // Spawn apple at a random position
    void initFonts(); //--|Points to Text 
    void initText(); //--|^^^^^^^^^^^^^^^^
    void checkObstacleCollision();
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
    void updateText();
    void update(); // Update game state
    
    void renderText(RenderTarget& target);
    void render(); // Render game objects

};
