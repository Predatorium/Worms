#include "State.h"
#include "State_Manager.h"

State::State(State_Manager* game, sf::RenderWindow* _window) : window{ _window }, Game{ game }
{
}

State::~State()
{
	delete Game;
	delete window;
}