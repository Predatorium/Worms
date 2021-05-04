#pragma once
#include "State.h"
#include "Map.h"
#include "Worms.h"

class GameState : public State
{
private:
	Map Carte;
	bool Click{ false };
	Worms worms;

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

