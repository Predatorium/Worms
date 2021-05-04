#pragma once
#include "State.h"
#include "Map.h"
#include "Joueur.h"
#include <vector>
#include "Canon.h"
#include "Explosion.h"

class GameState : public State
{
private:
	Map Carte;
	bool Click{ false };
	std::vector<std::vector<sf::Vector2f>> PosTeam;
	std::vector<Joueur> Player;
	std::vector<Arme> Arme;
	std::vector<Explosion> Explo;
	bool Switch{ false };
	int Me{ 0 };
	int Turn{ 0 };

public:
	GameState() = default;
	GameState(State_Manager * game, sf::RenderWindow * _window);
	~GameState() = default;

	void Pause() {};
	void Resume() {};

	void HandleEvents(sf::Event e);
	void Update(const float& dt);
	void Display();

};

