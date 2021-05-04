#pragma once
#include "SFML/Graphics.hpp"

class Entity
{
protected:
	sf::RectangleShape Shape;
	sf::Vector2f Velocity{ 0,0 };

	bool CheckCollid(sf::Vector2f _pos, sf::Image& image);
	void Get_NextPos(const float& dt, sf::Image& image);

public:
	Entity() = default;
	~Entity() = default;

	sf::Vector2f Get_Position() { return Shape.getPosition(); }

	void Move(sf::Vector2f move);
	virtual void Update(const float& dt, sf::Image& image) = 0;
	virtual void Display(sf::RenderWindow* window) = 0;
};