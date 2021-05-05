#include "Menu.h"
#include "State_Manager.h"
#include "Tools.h"
#include "WaitingRoom.h"
#include "GameState.h"

Menu::Menu(State_Manager* game, sf::RenderWindow* _window)
	: State(game, _window)
{
	font.loadFromFile("../Ressources/Sunflower.otf");
	Title = CreateText("Worms Revogeddon", font, 100);
	Title.setPosition(960, 50);

	Bouton.push_back(Button(CreateText("New Game", font, 50),
		CreateRectangle(1, { 200,50 }), { 500,500 },
		[this] { Game->ChangeState<WaitingRoom>(Game, window); }));

	Bouton.push_back(Button(CreateText("Quit", font, 50),
		CreateRectangle(1, { 200,50 }), { 500,600 },
		[this] { Game->PopState(); }));
}

void Menu::HandleEvents(sf::Event e)
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

void Menu::Update(const float& dt)
{
	if (!Click) {
		for (auto& it : Bouton) {
			if (it.Update(window)) {
				Click = true;
				break;
			}
		}
	}
	if (!sf::Mouse::isButtonPressed(sf::Mouse::Left))
		Click = false;
}

void Menu::Display()
{
	std::for_each(std::begin(Bouton), std::end(Bouton), [this](Button& b) {b.Display(window); });

	window->draw(Title);
}