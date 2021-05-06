#pragma once
#include "SFML/Graphics.hpp"

class Entity
{
protected:
	sf::Vector2f Velocity{ 0,0 };

	bool CheckCollid(sf::Vector2f _pos, sf::Image& image);
	void Move(sf::Vector2f move);

public:
	Entity() = default;
	~Entity() = default;
};