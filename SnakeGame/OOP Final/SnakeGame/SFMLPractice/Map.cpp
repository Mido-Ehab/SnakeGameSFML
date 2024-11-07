#include "Map.h"
#include <iostream>

Map::Map(const std::string& filename) {
    loadMapFromFile(filename);
}

void Map::loadMapFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open map file: " << filename << std::endl;
        return;
    }

    std::string line;
    int y = 0;
    const int cellSize = 25;

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

void Map::render(RenderTarget& target) {
    for (const auto& boundary : boundaries) {
        target.draw(boundary);
    }
}

bool Map::checkCollision(const Vector2f& position) const
{
    for (const auto& boundary : boundaries) {
        if (boundary.getGlobalBounds().contains(position)) {
            return true; // Collision detected
        }
    }
    return false;
}
