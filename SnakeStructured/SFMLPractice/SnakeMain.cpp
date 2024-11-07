#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <vector>
#include <sstream>
#include <cmath>
#include <fstream>
#include <vector>

using namespace sf;

// Map structure to handle map data and boundaries
struct Map {
    std::vector<std::string> mapData; // Stores the map from the file
    std::vector<RectangleShape> boundaries; // Stores boundary shapes
    const int cellSize = 25;

    Map(const std::string& filename) {
        loadMapFromFile(filename);
    }

    void loadMapFromFile(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Failed to open map file: " << filename << std::endl;
            return;
        }

        std::string line;
        int y = 0;

        while (std::getline(file, line)) {
            mapData.push_back(line);
            for (int x = 0; x < line.size(); ++x) {
                if (line[x] == '1') { // '1' represents a boundary
                    RectangleShape boundary(Vector2f(cellSize, cellSize));
                    boundary.setFillColor(Color::Red); // Color for boundary
                    boundary.setPosition(x * cellSize, y * cellSize);
                    boundaries.push_back(boundary);
                }
            }
            y++;
        }
    }

    void render(RenderTarget& target) {
        for (const auto& boundary : boundaries) {
            target.draw(boundary);
        }
    }

    bool isBoundaryCell(int x, int y) const {
        return x >= 0 && y >= 0 && y < mapData.size() && x < mapData[y].size() && mapData[y][x] == '1';
    }
};

// Clock for snake movement timing
Clock snakeMoveClock;
Time snakeMoveInterval = milliseconds(100); // Adjust this value to control snake speed

// Global variables for game state
RenderWindow* window;
Font font;
Text uiText;

int points = 0;
int health = 100;
int Timer = 180;
int speed = 15;  // Speed for the snake
bool endGame = false;

// Snake and Apple
struct point { int x = 0; int y = 0; };
std::vector<point> snake;  // Snake segments
point apple;               // Apple position
point BonusApple;          // Bonus apple position
point timePickup;          // Time boost pickup position

bool grow = false;
bool bonusActive = false;
bool Timeboost = false;

//MovingObstacle obstacle1(100, 100, 25, 25, 150.0f, Vector2f(1.0f, 0.5f));
//MovingObstacle obstacle2(500, 500, 25, 25, 200.0f, Vector2f(-0.7f, 1.0f));


// Direction and movement
int dir = 0; // Snake's direction (0=Right, 1=Left, 2=Down, 3=Up)

// Timer and Clock
Clock gameClock;
Clock timerClock;

// Function prototypes
void initGame(Map& map);
void createWindow();
void spawnApple(Map& map);
void handleInput();
void updateSnake(Map& map);
void updateText();
void checkCollisions(Map& map);
void render(Map& map);
void renderText(RenderTarget& target);
void update(Map& map);
void initFonts();

void initGame(Map& map) {
    // Initialize the window
    createWindow();

    // Load font for UI text
    initFonts();

    // Initialize snake and apple
    snake.push_back({ 400, 400 });  // Snake starting position
    spawnApple(map);
}

void createWindow() {
    window = new RenderWindow(VideoMode(800, 800), "Snake", Style::Titlebar | Style::Close | Style::Resize);
    window->setFramerateLimit(30);
}

void initFonts() {
    if (!font.loadFromFile("Fonts/Roboto-Black.ttf")) {
        std::cout << "Error loading font\n";
        endGame = true;
    }

    uiText.setFont(font);
    uiText.setCharacterSize(30);
    uiText.setFillColor(Color::White);
    uiText.setString("Points: 0  Health: 100  Time: 180");
}

void spawnApple(Map& map) {
    const int gridSize = 25;
    int gridWidth = 800 / gridSize;
    int gridHeight = 800 / gridSize;

    // Spawn the main apple
    do {
        apple.x = (rand() % gridWidth) * gridSize;
        apple.y = (rand() % gridHeight) * gridSize;
    } while (map.isBoundaryCell(apple.x / gridSize, apple.y / gridSize));

    // Conditionally spawn bonus apple
    if (rand() % 4 == 0) {
        do {
            BonusApple.x = (rand() % gridWidth) * gridSize;
            BonusApple.y = (rand() % gridHeight) * gridSize;
        } while (map.isBoundaryCell(BonusApple.x / gridSize, BonusApple.y / gridSize));
        bonusActive = true;
    }
    else {
        bonusActive = false;
    }

    // Conditionally spawn time boost pickup
    if (rand() % 4 == 0) {
        do {
            timePickup.x = (rand() % gridWidth) * gridSize;
            timePickup.y = (rand() % gridHeight) * gridSize;
        } while (map.isBoundaryCell(timePickup.x / gridSize, timePickup.y / gridSize));
        Timeboost = true;
    }
    else {
        Timeboost = false;
    }
}
// Struct to represent a moving obstacle
struct MovingObstacle {
    RectangleShape shape;
    Vector2f position;
    Vector2f direction;
    float speed;
    Vector2f size;

    MovingObstacle(float x, float y, float width, float height, float spd, Vector2f dir) {
        shape.setSize(Vector2f(width, height));
        shape.setPosition(x, y);
        shape.setFillColor(Color::Red); // Red color for the obstacle
        speed = spd;
        direction = dir;
        size = Vector2f(width, height);
    }

    void update(float deltaTime) {
        position += direction * speed * deltaTime;
        shape.setPosition(position);

        if (position.x <= 0 || position.x + size.x >= 800) {
            direction.x = -direction.x;
        }
        if (position.y <= 0 || position.y + size.y >= 800) {
            direction.y = -direction.y;
        }
    }

    bool checkCollision(const Vector2f& snakeHead) const {
        return shape.getGlobalBounds().contains(snakeHead);
    }

    void render(RenderTarget& target) {
        target.draw(shape);
    }
};

void handleInput() {
    Event event;
    while (window->pollEvent(event)) {
        if (event.type == Event::Closed) {
            window->close();
        }
        else if (event.type == Event::KeyPressed) {
            if (event.key.code == Keyboard::Escape) {
                window->close();
            }
            else if (event.key.code == Keyboard::Right && dir != 1) {
                dir = 0;
            }
            else if (event.key.code == Keyboard::Left && dir != 0) {
                dir = 1;
            }
            else if (event.key.code == Keyboard::Down && dir != 3) {
                dir = 2;
            }
            else if (event.key.code == Keyboard::Up && dir != 2) {
                dir = 3;
            }
        }
    }
}

void updateSnake(Map& map) {
    const int stepSize = 25;
    point newHead = snake[0];

    if (dir == 0) newHead.x += stepSize;
    else if (dir == 1) newHead.x -= stepSize;
    else if (dir == 2) newHead.y += stepSize;
    else if (dir == 3) newHead.y -= stepSize;

    float distance = sqrt(pow(newHead.x - apple.x, 2) + pow(newHead.y - apple.y, 2));
    if (distance < 25) {
        grow = true;
        points += 10;
        health += 5;
        speed++;
        spawnApple(map);
    }

    if (bonusActive) {
        distance = sqrt(pow(newHead.x - BonusApple.x, 2) + pow(newHead.y - BonusApple.y, 2));
        if (distance < 25) {
            grow = true;
            points += 25;
            health -= 50;
            bonusActive = false;
            speed++;
            spawnApple(map);
        }
    }

    if (Timeboost) {
        distance = sqrt(pow(newHead.x - timePickup.x, 2) + pow(newHead.y - timePickup.y, 2));
        if (distance < 25) {
            Timer += 20;
            Timeboost = false;
            speed--;
        }
    }

    snake.insert(snake.begin(), newHead);

    if (!grow) {
        snake.pop_back();
    }
    else {
        grow = false;
    }
}

void checkCollisions(Map& map) {
    if (snake[0].x < 0 || snake[0].x >= 800 || snake[0].y < 0 || snake[0].y >= 800) {
        endGame = true;
    }

    for (int i = 1; i < snake.size(); ++i) {
        if (snake[0].x == snake[i].x && snake[0].y == snake[i].y) {
            endGame = true;
        }
    }

    int gridX = snake[0].x / map.cellSize;
    int gridY = snake[0].y / map.cellSize;
    if (map.isBoundaryCell(gridX, gridY)) {
        endGame = true;
    }

 /*   Vector2f snakeHeadPos(snake[0].x, snake[0].y);
    if (obstacle1.checkCollision(snakeHeadPos) || obstacle2.checkCollision(snakeHeadPos)) {
        endGame = true;
    }*/
}

void updateText() {
    std::stringstream ss;
    ss << "Points: " << points << "\nHealth: " << health << "\nTime: " << Timer;
    uiText.setString(ss.str());
}

void renderText(RenderTarget& target) {
    target.draw(uiText);
}

void render(Map& map) {
    window->clear(Color(26, 28, 36));
    map.render(*window);
    for (const auto& segment : snake) {
        RectangleShape shape(Vector2f(25, 25));
        shape.setPosition(segment.x, segment.y);
        shape.setFillColor(Color::Cyan);
        window->draw(shape);
    }

    RectangleShape appleShape(Vector2f(25, 25));
    appleShape.setPosition(apple.x, apple.y);
    appleShape.setFillColor(Color::Green);
    window->draw(appleShape);

    if (bonusActive) {
        RectangleShape bonusShape(Vector2f(25, 25));
        bonusShape.setPosition(BonusApple.x, BonusApple.y);
        bonusShape.setFillColor(Color::Magenta);
        window->draw(bonusShape);
    }

    if (Timeboost) {
       CircleShape timeShape(10.f);
        timeShape.setPosition(timePickup.x, timePickup.y);
        timeShape.setFillColor(Color::Yellow);
        window->draw(timeShape);
    }
  /*  obstacle1.render(*window);
    obstacle2.render(*window);*/

    renderText(*window);
    window->display();

    
}

void update(Map& map) {
    snakeMoveInterval = milliseconds(1000 / speed);
    if (endGame) {

        window->close();
        return;
    }

    handleInput();

    float deltaTime = timerClock.restart().asSeconds(); // For smoother movement based on time

  /*  obstacle1.update(deltaTime);
    obstacle2.update(deltaTime);*/

    if (snakeMoveClock.getElapsedTime() >= snakeMoveInterval) {
        updateSnake(map);
        snakeMoveClock.restart();
    }

    if (timerClock.getElapsedTime().asSeconds() >= 1) {
        Timer--;
        timerClock.restart();
    }

    checkCollisions(map);
    updateText();
    render(map);

    // Update obstacles' positions

}

int main() {
    Map map("Map.txt");
    initGame(map);

    while (window->isOpen()) {
        update(map);
    }

    return 0;
}
