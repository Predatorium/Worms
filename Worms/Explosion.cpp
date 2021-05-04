#include "Explosion.h"

Explosion::Explosion(sf::Vector2f pos, float radius, int damage)
	: Damage{ damage }
{
	Shape.setRadius(radius);
	Shape.setOrigin(radius, radius);
	Shape.setPosition(pos);
	Shape.setFillColor(sf::Color::Red);
}

void Explosion::Affect_Damage(std::vector<Joueur>& joueurs)
{
	for (auto& it : joueurs) {
		it.Damage(-this->Damage);
	}
}

void Explosion::Update(const float& dt)
{
	Time += dt;

	if (Time > 0.5f)
		Life = false;
}

void Explosion::Display(sf::RenderWindow* window)
{
	window->draw(Shape);
}
