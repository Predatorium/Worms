#pragma once
#include "SFML/Network.hpp"
#include "Client.h"

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
	Delete_Worms,
	EXPLO,
	ChangeTurn,
};
enum State { Waiting, Game };

class Gestion
{
public:
	Gestion() = default;
	~Gestion() = default;
	int Init(int _port);

	State state{ Waiting };

	std::string Name1{ "Richard" };
	std::string Name2{ "Jhonny" };
	std::string Name3{ "Benjamin" };
	std::string Name4{ "Susan" };

	std::vector<int> DispoId;

	sf::TcpListener listener;		// Déclaration d'un listener
	sf::SocketSelector selector;	// Déclaration d'un SocketSelector
	std::vector<Client*> client;

	void Update(const float& dt);
	void CheckNewPlayer();
	void WaitingFullReday();
};

