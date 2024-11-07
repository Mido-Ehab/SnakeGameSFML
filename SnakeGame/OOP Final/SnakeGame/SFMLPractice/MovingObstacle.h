#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;

class MovingObstacle {
public:
    RectangleShape shape; // The shape of the obstacle
    Vector2f position;
    Vector2f direction;    // Movement direction (x, y)
    float speed;     // Speed of movement
    Vector2f size;


    // Constructor
    MovingObstacle(float x, float y, float width, float height, float speed, char Dir, Vector2f direction) {

        if (Dir == 'b') {
            this->speed = speed;
            this->direction = direction;
            shape.setSize(Vector2f(width, height));
            shape.setFillColor(Color::Black);  // Color for the obstacle
            shape.setPosition(x, y);

        }
        else if (Dir == 'p') {
            this->speed = speed;
            this->direction = direction;
            shape.setSize(Vector2f(width, height));
            shape.setFillColor(Color::Blue);  // Color for the obstacle
            shape.setPosition(x, y);
        }
    }
    void update(float deltaTime) {
        // Update position based on velocity and deltaTime
        
        position += direction * speed * deltaTime;

        // Check for boundary collision (assuming a window size of 500x500)
        if (position.x <= 0 || position.x + size.x >= 500) {
            // Reverse the horizontal direction
            direction.x = -direction.x;

            // Ensure the obstacle stays within bounds
            if (position.x < 0) position.x = 0;
            if (position.x + size.x > 500) position.x = 500 - size.x;
        }

        if (position.y <= 0 || position.y + size.y >= 500) {
            // Reverse the vertical direction
            direction.y = -direction.y;

            // Ensure the obstacle stays within bounds
            if (position.y < 0) position.y = 0;
            if (position.y + size.y > 500) position.y = 500 - size.y;
        }
        shape.move(direction * speed * deltaTime);
    }
    

    bool checkCollision(const Vector2f& snakeHead) const
    {

        return shape.getGlobalBounds().contains(snakeHead);
    }

    void render(RenderTarget& target)
    {
        target.draw(shape);
    }
};
