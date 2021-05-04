#pragma once
#pragma once
#include "SFML/Graphics.hpp"

class State_Manager;

class State
{
public:
	State() = default;
	State(State_Manager* game, sf::RenderWindow* _window);
	~State();

	virtual void Pause() = 0;
	virtual void Resume() = 0;

	virtual void HandleEvents(sf::Event e) = 0;
	virtual void Update(const float& dt) = 0;
	virtual void Display() = 0;

protected:

	sf::RenderWindow* window;
	State_Manager* Game;
};