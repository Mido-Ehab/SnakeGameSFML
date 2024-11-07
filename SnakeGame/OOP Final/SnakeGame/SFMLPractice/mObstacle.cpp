#include "mObstacle.h"

//update pos based on speed and dir
void mObstacle::update(float deltaTime)
{
	position += direction * speed * deltaTime;
	shape.setPosition(position);
}
// lf we erg3 tany lw Hit elWall
void mObstacle::handleCollisionWithBoundary(const Vector2f& boundarySize)
{
	if (position.x <= 0 || position.x + shape.getSize().x + shape.getSize().x >= boundarySize.x) {
		direction.x = -direction.x; //---- reverse bl3ard 
	}
	if (position.y <= 0 || position.y + shape.getSize().y + shape.getSize().y >= boundarySize.y) {
		direction.y = -direction.y; //---- reverse beltool 
	}

}
