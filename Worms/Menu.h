#pragma once
#include "State.h"
#include "Button.h"
#include <vector>

class Menu : public State
{
private:
	sf::Font font;
	sf::Text Title;
	std::vector<Button> Bouton;
	bool Click{ true };

public:
	Menu() = default;
	Menu(State_Manager * game, sf::RenderWindow * _window);
	~Menu() = default;

	void Pause() {};
	void Resume() {};

	void HandleEvents(sf::Event e);
	void Update(const float& dt);
	void Display();
};

