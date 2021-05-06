#pragma once
#include "Entity.h"

class Arme : public Entity
{
public:
	enum class Type
	{
		Canon,
		Grenade,
		Dynamite,
		Uzi
	};

private:
	sf::CircleShape Shape;
	bool Life{ true };
	float Radius{ 0 };
	int Damage{ 0 };
	int ID{ 0 };
	float timer{ 0 };
	Type type;

	void Get_NextPos(const float& dt, sf::Image& image);
	void Collide(sf::Image& image);

public:
	Arme(sf::Vector2f pos, float angle, int id, float power, Type arme = Type::Canon);
	~Arme() = default;

	bool Get_Life() { return Life; }
	float Get_Timer() { return timer; }
	float Get_Radius() { return Radius; }
	int Get_Damage() { return Damage; }
	sf::Vector2f Get_Position() { return Shape.getPosition(); }
	int Get_ID() { return ID; }

	void Update(const float& dt, int vent, sf::Image& image);
	void Display(sf::RenderWindow* window, sf::Font& font);
};