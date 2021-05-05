#pragma once
#include "State.h"
#include "Button.h"
#include "SFML/Network.hpp"

class WaitingRoom : public State
{
private:
	sf::Font font;
	sf::Text Title;
	Button Connecting;
	Button IReady;
	Button INoReady;
	Button Quit;
	std::map<int, std::string> OtherPlayer;
	bool Click{ true };
	std::string Ip;

	enum class Etat { Disco, Connect };
	Etat Current{ Etat::Disco };

	enum Type
	{
		Update_Pos,
		ME,
		Ready,
		NoReady,
		New,
		Attack,
		Disconnect,
		AddOtherPlayer,
		Add_Worms,
		Delete_Worms
	};

	std::shared_ptr<sf::TcpSocket> Socket;

	int Me{ 0 };

	void ReceptionServeur();
public:
	WaitingRoom() = default;
	WaitingRoom(State_Manager * game, sf::RenderWindow * _window);
	~WaitingRoom() = default;


	void Pause() { ; }
	void Resume() { ; }

	void HandleEvents(sf::Event e);
	void Update(const float& dt);
	void Display();
};