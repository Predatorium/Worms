#include "Client.h"
#include "Gestion.h"

void Client::CheckPacket(std::vector<Client*>& client)
{
	sf::Packet receivePacket; // Déclaration d'un packet pour la reception
	// Si les données ont toutes été reçus
	if (socket->receive(receivePacket) == sf::Socket::Done)
	{
		timeout = 0;
		int state;
		int type;
		receivePacket >> state >> type;

		if (type == Add_Worms) {
			sf::Vector2f pos;
			int WormsId;
			receivePacket >> WormsId >> pos.x >> pos.y;

			std::cout << "New Worms" << std::endl;

			Worms.push_back(std::make_pair(pos, WormsId));
		}
		if (type == Delete_Worms) {
			int WormsId;
			receivePacket >> WormsId;

			std::cout << "Worms " << WormsId << std::endl;

			for (int i = 0; i < Worms.size(); i++) {
				if (Worms[i].second == WormsId) {
					Worms.erase(Worms.begin() + i);
					break;
				}
			}

			sf::Packet packet;
			packet << state << Delete_Worms << Id << WormsId;

			for (int j = 0; j < client.size(); j++) {
				if (client[j] != this) {
					client[j]->socket->send(packet);
				}
			}
		}

		if (type == EXPLO) {
			sf::Vector2f Pos;
			int damage;
			float radius;
			receivePacket >> Pos.x >> Pos.y >> radius >> damage;

			std::cout << "Explosion" << std::endl;

			sf::Packet sendPacket; // Déclaration d'un packet pour l'envoi
			sendPacket << state << EXPLO << Id <<Pos.x << Pos.y << radius << damage; // Préparation d'un packet

			// envoi de ce paquet à tous les clients sauf à celui qui à envoyé
			for (int j = 0; j < client.size(); j++) {
				if (client[j] != this) {
					client[j]->socket->send(sendPacket);
				}
			}
		}
		if (type == Attack) {
			sf::Vector2f Pos;
			float Angle;
			int AttackType;
			float Power;
			receivePacket >> Angle >> Power >> Pos.x >> Pos.y >> AttackType;

			std::cout << "Attaque" << std::endl;

			sf::Packet sendPacket; // Déclaration d'un packet pour l'envoi
			sendPacket << state << Attack << Id << Angle << Power << Pos.x << Pos.y << AttackType; // Préparation d'un packet

			// envoi de ce paquet à tous les clients sauf à celui qui à envoyé
			for (int j = 0; j < client.size(); j++) {
				if (client[j] != this) {
					client[j]->socket->send(sendPacket);
				}
			}
		}
		if (type == Update_Pos) {
			int WormsId;
			int life = 100;
			receivePacket >> WormsId;
			for (int i = 0; i < Worms.size(); i++) {
				if (WormsId == Worms[i].second) {
					receivePacket >> Worms[i].first.x >> Worms[i].first.y >> life;
					break;
				}
			}

			sf::Packet sendPacket; // Déclaration d'un packet pour l'envoi
			sendPacket << state << Update_Pos << Id << WormsId; // Préparation d'un packet

			for (int i = 0; i < Worms.size(); i++) {
				if (WormsId == Worms[i].second) {
					sendPacket << Worms[i].first.x << Worms[i].first.y << life;
					break;
				}
			}

			// envoi de ce paquet à tous les clients sauf à celui qui à envoyé
			for (int j = 0; j < client.size(); j++) {
				if (client[j] != this) {
					client[j]->socket->send(sendPacket);
				}
			}
		}
		if (type == ChangeTurn) {
			
			float timerturn;
			float timer;

			receivePacket >> timerturn >> timer;

			int next = Id;
			int Vent = (rand() % 21) - 10;

			for (int j = 0; j < client.size(); j++) {
				if (client[j]->Id > next) {
					next = client[j]->Id;
					break;
				}
			}

			if (next == Id) {
				for (int j = 0; j < client.size(); j++) {
					if (client[j]->Id < next) {
						next = client[j]->Id;
					}
				}
			}

			sf::Packet sendPacket;									// Déclaration d'un packet
			sendPacket << state << ChangeTurn << next << Vent << timerturn << timer;

			for (int j = 0; j < client.size(); j++) {
				client[j]->socket->send(sendPacket);
			}
		}
		if (type == Disconnect) {

			sf::Packet sendPacket;									// Déclaration d'un packet
			sendPacket << state << Disconnect << Id;

			for (int j = 0; j < client.size(); j++) {
				if (client[j] != this) {
					client[j]->socket->send(sendPacket);
				}
			}

			Ready = false;
		}
	}
}