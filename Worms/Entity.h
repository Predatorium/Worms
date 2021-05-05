#pragma once
#include "SFML/Graphics.hpp"

class Entity
{
protected:
	sf::RectangleShape Shape;
	sf::Vector2f Velocity{ 0,0 };

	bool CheckCollid(sf::Vector2f _pos, sf::Image& image);
	void Get_NextPos(const float& dt, sf::Image& image);
	void Move(sf::Vector2f move);

public:
	Entity() = default;
	~Entity() = default;

	bool Sol{ false };
	bool Movable{ true };
	sf::Vector2f Get_Position() { return Shape.getPosition(); }

	virtual void Update(const float& dt, sf::Image& image) = 0;
	virtual void Display(sf::RenderWindow* window) = 0;
};