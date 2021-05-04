#pragma once
#include "Entity.h"
#include "Canon.h"

const float pi = 3.141592653589793238462643383279502884197169399375105820974944592307816406286;

class Worms : public Entity
{
private:
	int Life{ 100 };
	float Angle{ 0 };

public:
	Worms() = default;
	Worms(sf::Vector2f _pos, sf::Color _team);
	~Worms() = default;

	bool Movable{ true };

	int Get_Life() { return Life; }

	void Move(float move) { Velocity.x = move; }
	void Move_Angle(float move, const float& dt);
	void Set_Angle(float angle) { Angle = angle; }
	void StopVeloX() { Velocity.x = 0; }
	void Jump(float jump, sf::Image& image);
	void Shoot(std::vector<Arme>& shoot);
	void Damage(int damage) { Life += damage; }

	void Update(const float& dt, sf::Image& image);
	void Display(sf::RenderWindow* window);
	void DisplayDirAttack(sf::RenderWindow* window);
};