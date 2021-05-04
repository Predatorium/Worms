#include "GameState.h"
#include "State_Manager.h"

GameState::GameState(State_Manager* game, sf::RenderWindow* _window)
	: State(game, _window)
{
	PosTeam.push_back(std::vector<sf::Vector2f>());

	PosTeam[0].push_back(sf::Vector2f(1800, 100));
	PosTeam[0].push_back(sf::Vector2f(500, 100));

	Player.push_back(Joueur(PosTeam[0], 1));
}

void GameState::HandleEvents(sf::Event e)
{
	switch (e.type)
	{
	case sf::Event::Closed:
		window->close();
		break;
	case sf::Event::KeyPressed:
		switch (e.key.code)
		{
		case sf::Keyboard::Key::Escape:
			Game->PopState();
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}
}

void GameState::Update(const float& dt)
{
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && !Click) {
		Carte.DestroyMap(50, sf::Vector2f(sf::Mouse::getPosition(*window)));
		Click = true;
	}
	else if (!sf::Mouse::isButtonPressed(sf::Mouse::Left))
		Click = false;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Add) && !Switch) {
		Switch = true;
		Player[Turn].NextWorms();
	}
	else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Add))
		Switch = false;

	if (Me == Turn) {
		Player[Turn].Controle(Carte.Get_Collid(), Arme, dt);
	}

	for (auto& it : Player) {
		it.Update(dt, Carte.Get_Collid());
	}

	for (auto& it : Arme) {
		it.Update(dt, Carte.Get_Collid());
	}

	for (auto& it : Explo) {
		it.Update(dt);
	}

	for (auto it = std::begin(Arme); it != std::end(Arme);) {
		if (!it->Get_Life()) {
			Explo.push_back(Explosion(it->Get_Position(), it->Get_Radius(), it->Get_Damage()));
			Explo.back().Affect_Damage(Player);
			Carte.DestroyMap(Explo.back().Get_Radius(), Explo.back().Get_Position());
			it = Arme.erase(it);
		}
		else {
			it++;
		}
	}

	for (auto it = std::begin(Explo); it != std::end(Explo);) {
		if (!it->Get_Life()) {
			it = Explo.erase(it);
		}
		else {
			it++;
		}
	}
}

void GameState::Display()
{
	Carte.Display(window);

	for (auto& it : Player) {
		it.Display(window);
	}

	for (auto& it : Arme) {
		it.Display(window);
	}

	for (auto& it : Explo) {
		it.Display(window);
	}
}