#include "Game1.h"
#include <iostream>

// Constructor
Game1::Game1() {
    this->inVar();
    this->createWindow();
}

// Destructor
Game1::~Game1() {
    delete this->window;
}

// Initialize variables
void Game1::inVar() {
    this->window = nullptr;
    this->endGame = false;
    dir = 0; // Start direction is right
    grow = false;
    snake.push_back({ 400, 400 }); // Start snake in the middle
    spawnApple(); // Position the first apple
}

// Create game window
void Game1::createWindow() {
    this->videoMode.height = 800;
    this->videoMode.width = 800;
    this->window = new sf::RenderWindow(this->videoMode, "Snake", sf::Style::Titlebar | sf::Style::Close | sf::Style::Resize);
    this->window->setFramerateLimit(30);
}

// Check if game is running
const bool Game1::running() const {
    return this->window->isOpen();
}

// Check if game has ended
const bool Game1::getEndGame() const {
    return this->endGame;
}

// Handle input events
void Game1::takeEvents() {
    while (this->window->pollEvent(this->ev)) {
        switch (this->ev.type) {
        case sf::Event::Closed:
            this->window->close();
            break;
        case sf::Event::KeyPressed:
            if (this->ev.key.code == sf::Keyboard::Escape) {
                this->window->close();
            }
            else if (this->ev.key.code == sf::Keyboard::Right && dir != 1) {
                dir = 0;
            }
            else if (this->ev.key.code == sf::Keyboard::Left && dir != 0) {
                dir = 1;
            }
            else if (this->ev.key.code == sf::Keyboard::Down && dir != 3) {
                dir = 2;
            }
            else if (this->ev.key.code == sf::Keyboard::Up && dir != 2) {
                dir = 3;
            }
            break;
        }
    }
}

// Spawn apple at a random position within the grid
void Game1::spawnApple() {
    const int gridSize = 5; // Align with snake movement step size
    apple.x = (rand() % (800 / gridSize)) * gridSize;
    apple.y = (rand() % (800 / gridSize)) * gridSize;
}

int growthAmount = 3; //--------------no. of segments to add on each apple will be used in future to effect diff pickups

// Update the snake's movement and growth
void Game1::updateSnake() {
    const int stepSize = 5;

    //we gonna use tolerance to collect the apple once the head touches it and widen the range to be easier to collect
    const int collisionRadius = 30;
    // Calculate the new head position based on the direction----------------------------------------Movement----------------------------
    point newHead = snake[0];
    if (dir == 0) newHead.x += stepSize; // Move right
    if (dir == 1) newHead.x -= stepSize; // Move left
    if (dir == 2) newHead.y += stepSize; // Move down
    if (dir == 3) newHead.y -= stepSize; // Move up

    // Check for boundary collision with the new head position--------------------------------Boundaries---------------------------------
    if (newHead.x < 0 || newHead.x >= 800 || newHead.y < 0 || newHead.y >= 800) {
        endGame = true;
        return;
    }

    // Check for self-collision
    for (const auto& segment : snake) {
        if (newHead.x == segment.x && newHead.y == segment.y) {
            endGame = true;
            return;
        }
    }

    // Check for apple collision using a larger radius
    float distance = sqrt(pow(newHead.x - apple.x, 2) + pow(newHead.y - apple.y, 2)); //--------uclidean distance formula-----------ITI TECH Interview question
    if (distance < collisionRadius) {
        grow = true;
        growthAmount = 6;
        // Generate a new apple position
        do {
            spawnApple(); // Generate a new apple position
        } while (apple.x == newHead.x && apple.y == newHead.y); // Ensure the new apple does not spawn on the new head
    }

    // Insert the new head at the beginning of the vector
    snake.insert(snake.begin(), newHead);

    // Remove the last segment if not growing
    if (!grow) {
        snake.pop_back();
    }
    else {
        growthAmount--;
        if (growthAmount <= 0)
            // Reset growth flag after growth is applied
        {
            grow = false;
        }
    }

    // Draw the snake segments
    sf::RectangleShape segment(sf::Vector2f(50.f, 50.f));
    segment.setFillColor(sf::Color::Green);
    for (const auto& s : snake) {
        segment.setPosition(s.x, s.y);
        this->window->draw(segment);
    }

    // Draw the apple
    sf::RectangleShape appleShape(sf::Vector2f(50.f, 50.f));
    appleShape.setFillColor(sf::Color::Red);
    appleShape.setPosition(apple.x, apple.y);
    this->window->draw(appleShape);
}




// Check for collisions (boundaries or self)
bool Game1::checkCollision() {
    // Check boundary collision
    if (snake[0].x < 0 || snake[0].x >= 800 || snake[0].y < 0 || snake[0].y >= 800) {
        endGame = true;
    }

    // Check self-collision
    for (int i = 1; i < snake.size(); ++i) {
        if (snake[0].x == snake[i].x && snake[0].y == snake[i].y) {
            endGame = true;
        }
    }
    return endGame;
}

//------------------------------------------------------------------------ Update
void Game1::update() {
    this->takeEvents();
    this->updateSnake();
    if (checkCollision()) {
        this->window->close();
    }
}

//-------------------------------------------------------------------------Render
void Game1::render() {
    this->window->clear(sf::Color(26, 28, 36)); // Background color
    this->updateSnake(); // Draw snake and apple
    this->window->display();
}
