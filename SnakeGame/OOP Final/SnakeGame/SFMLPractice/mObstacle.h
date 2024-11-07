#pragma once
#include "Game1.h"

class mObstacle
{
public:
	Vector2f position;
	Vector2f direction;
	float speed;
	RectangleShape shape;

	mObstacle();

	mObstacle(sf::Vector2f pos, sf::Vector2f dir, float spd, float size = 20.0f)
		: position(pos), direction(dir), speed(spd) {
		shape.setSize(sf::Vector2f(size, size));
		shape.setFillColor(sf::Color::Red);
		shape.setPosition(position);
	}
	void update(float deltaTime);

	void handleCollisionWithBoundary(const Vector2f& boundarySize);

	~mObstacle();
};
