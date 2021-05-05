#include "State_Manager.h"
#include "State.h"

State_Manager::State_Manager(sf::RenderWindow* _window) : window(_window) {}

State_Manager::~State_Manager()
{
	while (!states.empty())
		states.pop();
}

void State_Manager::PopState()
{
	if (!states.empty())
		states.pop();

	if (!states.empty())
		states.top()->Resume();
	else
		window->close();
}

void State_Manager::HandleEvents(sf::Event e)
{
	if (!states.empty())
		states.top()->HandleEvents(e);
}

void State_Manager::Update(const float& dt)
{
	if (window->hasFocus()) {
	}
		if (!states.empty())
			states.top()->Update(dt);
}

void State_Manager::Display()
{
	window->clear(sf::Color::Black);

	if (!states.empty())
		states.top()->Display();

	window->display();
}