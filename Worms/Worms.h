#pragma once
#include "Entity.h"
#include "Canon.h"

const float pi(4.f * atan2f(1.f, 1.f));

class Worms : public Entity
{
private:
	int Life{ 100 };
	float Angle{ 0 };

public:
	Worms() = default;
	Worms(sf::Vector2f _pos, sf::Color _team);
	~Worms() = default;

	sf::RectangleShape& Get_Shape() { return Shape; }
	int Get_Life() { return Life; }

	void Move(float move) { Velocity.x = move; }
	void Move(sf::Vector2f move) { Velocity = move; }
	void Set_Position(sf::Vector2f pos) { Shape.setPosition(pos); }
	void Move_Angle(float move, const float& dt);
	float Get_Angle() { return Angle; }
	void Set_Angle(float angle) { Angle = angle; }
	void StopVeloX() { Velocity.x = 0; }
	void Jump(float jump, sf::Image& image);
	void Shoot(std::vector<Arme>& shoot, Arme::Type arme);
	void Damage(int damage) { Life += damage; }

	void Update(const float& dt, sf::Image& image);
	void Display(sf::RenderWindow* window, sf::Font& font);
	void Display(sf::RenderWindow* window) { ; }
	void DisplayDirAttack(sf::RenderWindow* window);
};