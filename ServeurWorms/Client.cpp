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
			int 
			receivePacket >> pos.x >> pos.y >>;

			Worms.push_back(pos);

			sf::Packet sendPacket; // D�claration d'un packet pour l'envoi
			sendPacket << state << Add_Worms << Id << pos.x << pos.y; // Pr�paration d'un packet

			for (int j = 0; j < client.size(); j++) {
				if (client[j] != this) {
					client[j]->socket->send(sendPacket);
				}
			}
		}

		if (type == Delete_Worms) {
			sf::Vector2f pos;
			receivePacket >> pos.x >> pos.y;



			sf::Packet sendPacket; // D�claration d'un packet pour l'envoi
			sendPacket << state << Add_Worms << Id << pos.x << pos.y; // Pr�paration d'un packet

			for (int j = 0; j < client.size(); j++) {
				if (client[j] != this) {
					client[j]->socket->send(sendPacket);
				}
			}
		}

		if (type == Update_Pos) {
			for (int i = 0; i < Worms.size(); i++) {
				receivePacket >> Worms[i].x >> Worms[i].y;
			}

			sf::Packet sendPacket; // D�claration d'un packet pour l'envoi
			sendPacket << state << Update_Pos << Id; // Pr�paration d'un packet

			for (int i = 0; i < Worms.size(); i++) {
				receivePacket << Worms[i].x << Worms[i].y;
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
