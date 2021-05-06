#include "GameState.h"
#include "State_Manager.h"
#include "Menu.h"
#include "Tools.h"

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
		PosTeam[0].push_back(sf::Vector2f(1400, 100));
		PosTeam[0].push_back(sf::Vector2f(700, 100));

		PosTeam[1].push_back(sf::Vector2f(1600, 100));
		PosTeam[1].push_back(sf::Vector2f(100, 100));
		PosTeam[1].push_back(sf::Vector2f(1300, 100));
		PosTeam[1].push_back(sf::Vector2f(300, 100));
	}
	if (_player.size() + 1 > 2) {
		PosTeam[2].push_back(sf::Vector2f(200, 100));
		PosTeam[2].push_back(sf::Vector2f(600, 100));
		PosTeam[2].push_back(sf::Vector2f(1500, 100));
		PosTeam[2].push_back(sf::Vector2f(900, 100));
	}
	if (_player.size() + 1 > 3) {
		PosTeam[3].push_back(sf::Vector2f(1800, 100));
		PosTeam[3].push_back(sf::Vector2f(500, 100));
		PosTeam[3].push_back(sf::Vector2f(1800, 100));
		PosTeam[3].push_back(sf::Vector2f(500, 100));
	}

	Player = Joueur(PosTeam[(long)Me - 1], Me, Socket, font);

	for (auto& it : _player) {
		Enemy.push_back(OtherPlayer(PosTeam[(long)it.first - 1], it.first, it.second));
	}

	sf::Packet packet;
	packet << 1 << ChangeTurn;
	Socket->send(packet);
}

void GameState::HandleEvents(sf::Event e)
{
	switch (e.type)
	{
	case sf::Event::Closed:
		window->close();
		break;
	default:
		break;
	}

	if (!window->isOpen() || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
		sf::Packet sendPacket;					// Déclaration d'un packet
		sendPacket << 1 << Disconnect;			// Préparation d'un packet
		Socket->send(sendPacket);				// Envoi de ce paquet au serveur

		Game->ChangeState<Menu>(Game, window);
	}

}

void GameState::FindNextPlayer()
{
	if (Timer < 0) {

		if (Turn == Me) {
			sf::Packet packet;
			packet << 1 << ChangeTurn;
			Socket->send(packet);
		}

		Timer = 90;
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

		if (type == EXPLO) {
			sf::Vector2f Pos;
			float radius;
			int damage;
			int id;

			receivePacket >> id >> Pos.x >> Pos.y >> radius >> damage;

			Explo.push_back(Explosion(Pos, radius, damage));
			Explo.back().Affect_Damage(Player);
			Carte.DestroyMap(Explo.back().Get_Radius(), Explo.back().Get_Position());
		}
		if (type == Attack) {
			sf::Vector2f Pos;
			float Angle;
			float Power;
			int AttackType;
			int id;

			receivePacket >> id >> Angle >> Power >> Pos.x >> Pos.y >> AttackType;

			arme.push_back(Arme(Pos, Angle, id, Power, static_cast<Arme::Type>(AttackType)));
			Timer = 10;
		}
		if (type == Update_Pos) {
			int IdPlayer;
			int Worms;
			sf::Vector2f pos;
			int life;

			receivePacket >> IdPlayer >> Worms >> pos.x >> pos.y >> life;

			for (auto& it : Enemy) {
				if (it.Get_Id() == IdPlayer) {
					it.SetPos(pos, Worms);
					it.SetLife(life, Worms);
				}
			}
		}
		if (type == ChangeTurn) {
			receivePacket >> Turn >> Vent;

			if (Turn == Me) {
				Player.NextWorms();
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
			Player.Controle(Carte.Get_Collid(), arme, dt, Timer, window);
		}

		if (Player.EraseWorms()) {
			Timer = 0;
		}

		FindNextPlayer();
	}
	else {
		Nextturn -= dt;
	}
	
	Player.Update(dt, Carte.Get_Collid());

	ReceptionServeur();

	for (auto& it : arme) {
		it.Update(dt, Vent, Carte.Get_Collid());
	}

	for (auto& it : Explo) {
		it.Update(dt);
	}

	for (auto it = std::begin(arme); it != std::end(arme);) {
		if (!it->Get_Life() && it->Get_Timer() <= 0) {
			if (it->Get_ID() == Me) {
				Explo.push_back(Explosion(it->Get_Position(), it->Get_Radius(), it->Get_Damage()));
				Explo.back().Affect_Damage(Player);
				Carte.DestroyMap(Explo.back().Get_Radius(), Explo.back().Get_Position());

				sf::Packet packet;
				packet << 1 << EXPLO << Explo.back().Get_Position().x << Explo.back().Get_Position().y
					<< Explo.back().Get_Radius() << it->Get_Damage();

				Socket->send(packet);
			}
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

	if (Turn == Me) {
		Player.DisplayTirAngle(window);
	}

	for (auto& it : arme) {
		it.Display(window, font);
	}

	for (auto& it : Explo) {
		it.Display(window);
	}

	sf::Text tmp = CreateText(std::to_string(static_cast<int>(Timer)), font, 50);
	tmp.setPosition(960, 100);
	tmp.setFillColor(sf::Color::Black);
	window->draw(tmp);
}