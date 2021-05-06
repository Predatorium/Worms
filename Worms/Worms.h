#pragma once
#include "Entity.h"
#include "Canon.h"

const float pi(4.f * atan2f(1.f, 1.f));

class Worms : public Entity
{
private:
	sf::RectangleShape Shape;
	int Life{ 100 };
	float Angle{ 0 };
	float DamageFall{ 0 };
	int Received{ 0 };
	float timer;
	std::string DamageText;
	void Get_NextPos(const float& dt, sf::Image& image);

public:
	Worms() = default;
	Worms(sf::Vector2f _pos, sf::Color _team);
	~Worms() = default;

	bool Sol{ false };
	bool Movable{ true };

	sf::RectangleShape& Get_Shape() { return Shape; }
	int Get_Life() { return Life; }
	void Set_Life(int life);

	sf::Vector2f Get_Position() { return Shape.getPosition(); }
	inline void Move(float move) { Velocity.x = move; }
	inline void Move(sf::Vector2f move) { Velocity = move; }
	inline void Set_Position(sf::Vector2f pos) { Shape.setPosition(pos); }

	void Move_Angle(float move, const float& dt);
	inline float Get_Angle() { return Angle; }
	inline void Set_Angle(float angle) { Angle = angle; }

	inline void StopVeloX() { Velocity.x = 0; }

	void Jump(float jump, sf::Image& image);
	void Shoot(std::vector<Arme>& shoot, int id, Arme::Type arme, float power);
	void Damage(int damage);

	void Update(const float& dt, sf::Image& image);
	void Display(sf::RenderWindow* window, sf::Font& font);
	void DisplayDirAttack(sf::RenderWindow* window);
};