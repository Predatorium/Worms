#pragma once
#include "Joueur.h"

class Explosion
{
private:
	sf::CircleShape Shape;
	int Damage{ 0 };
	bool Life{ true };
	float Time{ 0 };

public:
	Explosion(sf::Vector2f pos, float radius, int damage);
	~Explosion() = default;

	bool Get_Life() { return Life; }
	sf::Vector2f Get_Position() { return Shape.getPosition(); }
	float Get_Radius() { return Shape.getRadius(); }
	void Affect_Damage(std::vector<OtherPlayer>& joueurs, Joueur& player);

	void Update(const float& dt);
	void Display(sf::RenderWindow* window);
};