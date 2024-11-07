#pragma once
#include <vector>
#include <string>
#include <fstream>
#include <SFML/Graphics.hpp>

using namespace sf;

class Map {
private:
    std::vector<std::string> mapData; // Stores the map from the file
    std::vector<RectangleShape> boundaries; // Stores boundary shapes

    void loadMapFromFile(const std::string& filename);

public:
    Map() {};
    Map(const std::string& filename);
    void render(RenderTarget& target);
    bool checkCollision(const Vector2f& position) const;
};
