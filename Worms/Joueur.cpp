#include "Joueur.h"

Joueur::Joueur(std::vector<sf::Vector2f> pos, int id) : Id{ id }
{
	sf::Color tmp;
	switch (id)
	{
	case 1:
		tmp = sf::Color::Blue;
		break;
	case 2:
		tmp = sf::Color::Red;
		break;
	case 3:
		tmp = sf::Color::Green;
		break;
	case 4:
		tmp = sf::Color::Magenta;
		break;
	default:
		break;
	}

	for (auto it : pos) {
		Team.push_back(Worms(it, tmp));
	}
}

void Joueur::NextWorms()
{
	WormsTurn++;
	if (WormsTurn > Team.size() - 1) {
		WormsTurn = 0;
	}
}

void Joueur::Controle(sf::Image& image, std::vector<Arme>& shoot, const float& dt)
{
	if (Team.size() > 0) {
		if (Team[WormsTurn].Movable) {
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
				Team[WormsTurn].Move(100);
				Team[WormsTurn].Set_Angle(0);
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {
				Team[WormsTurn].Move(-100);
				Team[WormsTurn].Set_Angle(pi);
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
				Team[WormsTurn].Jump(400, image);
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::E) && Shoot) {
				Team[WormsTurn].Shoot(shoot);
				Shoot = false;
			}
			else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::E))
				Shoot = true;
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z)) {
				Team[WormsTurn].Move_Angle(5, dt);
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
				Team[WormsTurn].Move_Angle(-5, dt);
			}
		}
	}
}

void Joueur::Damage(int damage)
{
	for (auto& it : Team) {
		it.Damage(damage);
	}
}

void Joueur::Update(const float& dt, sf::Image& image)
{
	for (auto& it : Team) {
		it.Update(dt, image);

		if (it.Movable)
			it.StopVeloX();
	}

	for (auto it = std::begin(Team); it != std::end(Team);) {
		if (it->Get_Life() <= 0) {
			it = Team.erase(it);
		}
		else {
			it++;
		}
	}
}

void Joueur::Display(sf::RenderWindow* window)
{
	for (auto& it : Team)
		it.Display(window);

	if (Team.size() > 0)
		Team[WormsTurn].DisplayDirAttack(window);
}
