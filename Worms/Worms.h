#pragma once
#include "SFML/Graphics.hpp"

class Worms
{
private:
	sf::RectangleShape Shape;
	sf::Vector2f Velocity{ 0,0 };
	int Life{ 100 };

	bool CheckCollid(sf::Vector2f _pos, sf::Image& image);
	void Get_NextPos(const float& dt, sf::Image& image);

public:
	Worms() = default;
	Worms(sf::Vector2f _pos, sf::Color _team);
	~Worms() = default;

	void Move(sf::Vector2f _move);
	void Update(const float& dt, sf::Image& image);
	void Display(sf::RenderWindow* window);
};