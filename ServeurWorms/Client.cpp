#include "Client.h"
#include "Gestion.h"

void Client::CheckPacket(std::vector<Client*>& client)
{
	sf::Packet receivePacket; // D�claration d'un packet pour la reception
	// Si les donn�es ont toutes �t� re�us
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

			Worms.push_back(std::make_pair(pos, WormsId));

			sf::Packet sendPacket; // D�claration d'un packet pour l'envoi
			sendPacket << state << Add_Worms << Id << WormsId << pos.x << pos.y; // Pr�paration d'un packet

			for (int j = 0; j < client.size(); j++) {
				if (client[j] != this) {
					client[j]->socket->send(sendPacket);
				}
			}
		}
		if (type == Delete_Worms) {
			int WormsId;
			receivePacket >> WormsId;

			sf::Packet sendPacket; // D�claration d'un packet pour l'envoi
			sendPacket << state << Add_Worms << Id << WormsId; // Pr�paration d'un packet

			for (int i = 0; i < Worms.size(); i++) {
				if (Worms[i].second == WormsId) {
					Worms.erase(Worms.begin() + i);
					i--;
				}
			}

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
			receivePacket >> Angle >> Pos.x >> Pos.y >> AttackType;

			sf::Packet sendPacket; // D�claration d'un packet pour l'envoi
			sendPacket << state << Attack << Id << Angle << Pos.x << Pos.y << AttackType; // Pr�paration d'un packet

			// envoi de ce paquet � tous les clients sauf � celui qui � envoy�
			for (int j = 0; j < client.size(); j++) {
				if (client[j] != this) {
					client[j]->socket->send(sendPacket);
				}
			}
		}
		if (type == Update_Pos) {
			int WormsId;
			receivePacket >> WormsId;
			for (int i = 0; i < Worms.size(); i++) {
				if (WormsId == Worms[i].second) {
					receivePacket >> Worms[i].first.x >> Worms[i].first.y;
				}
			}

			sf::Packet sendPacket; // D�claration d'un packet pour l'envoi
			sendPacket << state << Update_Pos << Id << WormsId; // Pr�paration d'un packet

			for (int i = 0; i < Worms.size(); i++) {
				if (WormsId == Worms[i].second) {
					sendPacket << Worms[i].first.x << Worms[i].first.y;
				}
			}

			// envoi de ce paquet � tous les clients sauf � celui qui � envoy�
			for (int j = 0; j < client.size(); j++) {
				if (client[j] != this) {
					client[j]->socket->send(sendPacket);
				}
			}
		}
		if (type == Disconnect) {

			sf::Packet sendPacket;									// D�claration d'un packet
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
