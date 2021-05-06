#include "Entity.h"

bool Entity::CheckCollid(sf::Vector2f _pos, sf::Image& image)
{
	if (sf::FloatRect(0, 0, image.getSize().x, image.getSize().y).contains(_pos)) {
		return image.getPixel(_pos.x, _pos.y) == sf::Color::Black;
	}

	return false;
}

void Entity::Move(sf::Vector2f move)
{
	Velocity = move;
}