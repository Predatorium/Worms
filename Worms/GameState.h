#pragma once
#include "State.h"
#include "Map.h"
#include "Joueur.h"
#include <vector>
#include "Canon.h"
#include "Explosion.h"
#include "SFML/Network.hpp"

class GameState : public State
{
private:

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
	};

	std::shared_ptr<sf::TcpSocket> Socket;

	Map Carte;
	bool Click{ true };
	sf::Font font;

	Joueur Player;

	std::vector<OtherPlayer> Enemy;
	std::vector<Arme> arme;
	std::vector<Explosion> Explo;

	bool Switch{ true };
	int Me{ 0 };
	int Turn{ 0 };
	float Timer{ 90.f };
	float Nextturn{ 5.f };

	void FindNextPlayer();
	void ReceptionServeur();

public:
	GameState() = default;
	GameState(State_Manager * game, sf::RenderWindow * _window,
		std::map<int, std::string> _player, int id,
		std::shared_ptr<sf::TcpSocket> socket);
	~GameState() = default;

	void Pause() {};
	void Resume() {};

	void HandleEvents(sf::Event e);
	void Update(const float& dt);
	void Display();

};

