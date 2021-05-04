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
			int 
			receivePacket >> pos.x >> pos.y >>;

			Worms.push_back(pos);

			sf::Packet sendPacket; // Déclaration d'un packet pour l'envoi
			sendPacket << state << Add_Worms << Id << pos.x << pos.y; // Préparation d'un packet

			for (int j = 0; j < client.size(); j++) {
				if (client[j] != this) {
					client[j]->socket->send(sendPacket);
				}
			}
		}

		if (type == Delete_Worms) {
			sf::Vector2f pos;
			receivePacket >> pos.x >> pos.y;



			sf::Packet sendPacket; // Déclaration d'un packet pour l'envoi
			sendPacket << state << Add_Worms << Id << pos.x << pos.y; // Préparation d'un packet

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

			sf::Packet sendPacket; // Déclaration d'un packet pour l'envoi
			sendPacket << state << Update_Pos << Id; // Préparation d'un packet

			for (int i = 0; i < Worms.size(); i++) {
				receivePacket << Worms[i].x << Worms[i].y;
			}

			// envoi de ce paquet à tous les clients sauf à celui qui à envoyé
			for (int j = 0; j < client.size(); j++) {
				if (client[j] != this) {
					client[j]->socket->send(sendPacket);
				}
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
