#include "GameState.h"
#include "State_Manager.h"

GameState::GameState(State_Manager* game, sf::RenderWindow* _window)
	: State(game, _window), worms({ 100,900 }, sf::Color::Blue)
{
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

	worms.Update(dt, Carte.Get_Collid());
}

void GameState::Display()
{
	Carte.Display(window);
	worms.Display(window);
}
