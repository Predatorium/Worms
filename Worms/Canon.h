#pragma once
#include "Entity.h"

class Arme : public Entity
{
public:
	enum class Type{Canon, Grenade, Dynamite, Uzi};

private:
	bool Life{ true };
	float Radius{ 0 };
	int Damage{ 0 };
	int ID{ 0 };

	void Collide(sf::Image& image);

public:
	Arme(sf::Vector2f pos, float angle, int id, Type arme = Type::Canon);
	~Arme() = default;

	bool Get_Life() { return Life; }
	float Get_Radius() { return Radius; }
	int Get_Damage() { return Damage; }
	int Get_ID() { return ID; }

	void Update(const float& dt, sf::Image& image);
	void Display(sf::RenderWindow* window);
};

