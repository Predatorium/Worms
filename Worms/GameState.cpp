#include "GameState.h"
#include "State_Manager.h"

GameState::GameState(State_Manager* game, sf::RenderWindow* _window,
	std::map<int, std::string> _player, int id, std::shared_ptr<sf::TcpSocket> socket)
	: State(game, _window), Me{ id }, Socket{ socket }
{
	font.loadFromFile("../Ressources/Sunflower.otf");
	std::vector<std::vector<sf::Vector2f>> PosTeam;

	for (int i = 0; i < _player.size() + 1; i++) {
		PosTeam.push_back(std::vector<sf::Vector2f>());
	}

	if (_player.size() + 1 > 1) {
		PosTeam[0].push_back(sf::Vector2f(1800, 100));
		PosTeam[0].push_back(sf::Vector2f(500, 100));
		PosTeam[0].push_back(sf::Vector2f(1800, 100));
		PosTeam[0].push_back(sf::Vector2f(500, 100));

		PosTeam[1].push_back(sf::Vector2f(1800, 100));
		PosTeam[1].push_back(sf::Vector2f(500, 100));
		PosTeam[1].push_back(sf::Vector2f(1800, 100));
		PosTeam[1].push_back(sf::Vector2f(500, 100));
	}
	if (_player.size() + 1 > 2) {
		PosTeam[2].push_back(sf::Vector2f(1800, 100));
		PosTeam[2].push_back(sf::Vector2f(500, 100));
		PosTeam[2].push_back(sf::Vector2f(1800, 100));
		PosTeam[2].push_back(sf::Vector2f(500, 100));
	}
	if (_player.size() + 1 > 3) {
		PosTeam[3].push_back(sf::Vector2f(1800, 100));
		PosTeam[3].push_back(sf::Vector2f(500, 100));
		PosTeam[3].push_back(sf::Vector2f(1800, 100));
		PosTeam[3].push_back(sf::Vector2f(500, 100));
	}

	Player = Joueur(PosTeam[Me - 1], Me, Socket);

	for (auto& it : _player) {
		Enemy.push_back(OtherPlayer(PosTeam[it.first - 1], it.first, it.second));
	}
}

GameState::GameState(State_Manager* game, sf::RenderWindow* _window)
	: State(game, _window)
{
	font.loadFromFile("../Ressources/Sunflower.otf");
	std::vector<std::vector<sf::Vector2f>> PosTeam;
	PosTeam.push_back(std::vector<sf::Vector2f>());
	PosTeam[0].push_back(sf::Vector2f(1800, 100));
	PosTeam[0].push_back(sf::Vector2f(500, 100));

	Player = Joueur(PosTeam[0], 0, Socket);
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

void GameState::FindNextPlayer()
{
	if (Timer < 0) {
		if (Turn == Me)
			Player.NextWorms();

		bool done = true;
		while (done) {
			Turn++;
			
			if (Turn > Enemy.size()) {
				Turn = 0;
			}

			if (Turn == Me)
				done = false;

			for (auto& it : Enemy) {
				if (Turn == it.Get_Id()) {
					done = false;
				}
			}
		}

		Nextturn = 5.f;
	}
}

void GameState::ReceptionServeur()
{
	sf::Packet receivePacket;

	if (Socket->receive(receivePacket) == sf::Socket::Done) {
		int etats;
		int type;

		receivePacket >> etats >> type;

		if (type == Attack) {
			sf::Vector2f Pos;
			float Angle;
			int AttackType;
			float Power;

			receivePacket >> Angle >> Power >> Pos.x >> Pos.y >> AttackType;

			arme.push_back(Arme(Pos, Angle, static_cast<Arme::Type>(AttackType)));
		}
		if (type == Update_Pos) {
			int IdPlayer;
			int Worms;
			sf::Vector2f pos;

			receivePacket >> IdPlayer >> Worms >> pos.x >> pos.y;

			for (auto& it : Enemy) {
				if (it.Get_Id() == IdPlayer) {
					it.SetPos(pos, Worms);
				}
			}
		}
		if (type == Delete_Worms) {
			int Id;
			int Worms;
			receivePacket >> Id >> Worms;

			for (auto& it : Enemy) {
				if (it.Get_Id() == Id) {
					it.Delete(Worms);
				}
			}
		}
		if (type == Disconnect) {
			int Id;
			receivePacket >> Id;

			for (auto it = std::begin(Enemy); it != std::end(Enemy);) {
				if (it->Get_Id() == Id) {
					it = Enemy.erase(it);
					break;
				}
				else {
					it++;
				}
			}
		}
	}
}

void GameState::Update(const float& dt)
{
	if (Nextturn > 0.f) {
		Timer -= dt;

		if (Me == Turn) {
			Player.Controle(Carte.Get_Collid(), arme, dt, Timer);
		}

		ReceptionServeur();
	}
	else {
		Nextturn -= dt;
	}	

	for (auto& it : arme) {
		it.Update(dt, Carte.Get_Collid());
	}

	for (auto& it : Explo) {
		it.Update(dt);
	}

	for (auto it = std::begin(arme); it != std::end(arme);) {
		if (!it->Get_Life()) {
			Explo.push_back(Explosion(it->Get_Position(), it->Get_Radius(), it->Get_Damage()));
			Explo.back().Affect_Damage(Enemy, Player);
			Carte.DestroyMap(Explo.back().Get_Radius(), Explo.back().Get_Position());
			it = arme.erase(it);
		}
		else {
			it++;
		}
	}

	for (auto it = std::begin(Explo); it != std::end(Explo);) {
		if (!it->Get_Life()) {
			it = Explo.erase(it);
		}
		else {
			it++;
		}
	}
}

void GameState::Display()
{
	Carte.Display(window);

	for (auto& it : Enemy) {
		it.Display(window, font);
	}

	Player.Display(window, font);

	for (auto& it : arme) {
		it.Display(window);
	}

	for (auto& it : Explo) {
		it.Display(window);
	}
}