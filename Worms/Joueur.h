#pragma once
#include "Worms.h"
#include <vector>

class Joueur
{
private:
	std::vector<Worms> Team;
	int WormsTurn{ 0 };
	int Id{ 0 };
	bool Shoot{ true };

public:
	Joueur() = default;
	Joueur(std::vector<sf::Vector2f> pos, int id);
	~Joueur() = default;

	void NextWorms();
	int Get_Id() { return Id; }

	void Controle(sf::Image& image, std::vector<Arme>& shoot, const float& dt);
	void Damage(int damage);

	void Update(const float& dt, sf::Image& image);
	void Display(sf::RenderWindow* window);
};

