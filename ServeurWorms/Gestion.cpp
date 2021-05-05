#include "Gestion.h"

int Gestion::Init(int _port)
{
	// Vérification que le port soit libre si il est libre 
	// on commence à écouter les nouvelles connexions
	if (listener.listen(_port) == sf::Socket::Done)
	{
		std::cout << "Serveur en ligne" << std::endl;
	}
	else
	{
		// Si le port n'est pas libre on quitte
		return EXIT_FAILURE;
	}
	// Si il est libre on ajoute le listener au selector
	selector.add(listener);
	// On crée un container pour stocker tout les sockets

	DispoId.push_back(4);
	DispoId.push_back(3);
	DispoId.push_back(2);
	DispoId.push_back(1);
}

void Gestion::Update(const float& dt)
{
	if (selector.wait())
	{
		switch (state)
		{
		case Waiting:
			CheckNewPlayer();
			WaitingFullReday();
			break;
		case Game:

			for (int i = 0; i < client.size(); i++) {
				client[i]->CheckPacket(client);
			}

			for (int i = 0; i < client.size(); i++) {
				if (!client[i]->Ready) {
					DispoId.push_back(client[i]->Id);
					client.erase(client.begin() + i);
					i--;
				}
			}
			break;
		default:
			break;
		}

		//for (int i = 0; i < client.size(); i++) {
		//	client[i]->timeout += dt;

		//	if (client[i]->timeout > 10.f) {

		//		sf::Packet sendPacket;									// Déclaration d'un packet
		//		sendPacket << state << Disconnect << client[i]->Id;
		//		
		//		DispoId.push_back(client[i]->Id);
		//		client.erase(client.begin() + i);
		//		i--;

		//		for (int j = 0; j < client.size(); j++) {
		//			client[j]->socket->send(sendPacket);
		//		}
		//	}
		//}
	}
}

void Gestion::CheckNewPlayer()
{
	if (selector.isReady(listener) && client.size() < 4)
	{
		std::string pseudo;

		if (client.size() == 0)
			pseudo = Name1;
		if (client.size() == 1)
			pseudo = Name2;
		if (client.size() == 2)
			pseudo = Name3;
		if (client.size() == 3)
			pseudo = Name4;

		Client* socket = new Client(pseudo);
		socket->socket = new sf::TcpSocket();
		listener.accept(*socket->socket);
		selector.add(*socket->socket);
		int id = DispoId.back();
		DispoId.pop_back();

		int type;

		sf::Packet receivePacket;
		if (socket->socket->receive(receivePacket) == sf::Socket::Done)
		{
			receivePacket >> type;
			socket->Id = id;
			socket->timeout = 0;
			std::cout << socket->username << " vient de se connecter." << id << std::endl;
		}

		client.push_back(socket);

		sf::Packet NsendPacket; // Déclaration d'un packet pour l'envoi
		NsendPacket << state << ME << id; // Préparation d'un packet
		client.back()->socket->send(NsendPacket);

		std::string ip = client.back()->socket->getRemoteAddress().toString();

		std::cout << "Recu du client " << socket->username << " ip: " << ip << std::endl;

		sf::Packet sendPacket; // Déclaration d'un packet pour l'envoi
		sendPacket << state << New << id << socket->username; // Préparation d'un packet

		// envoi de ce paquet à tous les clients sauf à celui qui à envoyé
		for (int j = 0; j < client.size(); j++) {
			if (client[j] != client.back()) {
				client[j]->socket->send(sendPacket);

				sf::Packet sendPacketP; // Déclaration d'un packet pour l'envoi
				sendPacketP << state << AddOtherPlayer << client[j]->Id << client[j]->username; // Préparation d'un packet

				client.back()->socket->send(sendPacketP);
			}
		}
	}
}

void Gestion::WaitingFullReday()
{
	if (!selector.isReady(listener))
	{
		// pour chaque client connecté
		for (int i = 0; i < client.size(); i++)
		{
			// si il reçoit des données
			if (selector.isReady(*client[i]->socket))
			{
				sf::Packet receivePacket; // Déclaration d'un packet pour la reception
				// Si les données ont toutes été reçus
				if (client[i]->socket->receive(receivePacket) == sf::Socket::Done)
				{
					client[i]->timeout = 0;
					int type;
					receivePacket >> type;

					if (type == Ready) {
						client[i]->Ready = true;
						std::cout << client[i]->username << "est pret." << std::endl;
					}
					if (type == NoReady) {
						client[i]->Ready = false;
						std::cout << client[i]->username << "n est pas pret." << std::endl;
					}

					if (type == Disconnect) {

						sf::Packet sendPacket;					// Déclaration d'un packet
						sendPacket << state << Disconnect << client[i]->Id;

						DispoId.push_back(client[i]->Id);

						std::cout << client[i]->username << "c est deco." << std::endl;

						client.erase(client.begin() + i);
						i--;

						for (int j = 0; j < client.size(); j++) {
							client[j]->socket->send(sendPacket);
						}
					}
				}
			}
		}

		if (client.size() > 1) {
			int FullReady = 0;
			for (int j = 0; j < client.size(); j++) {
				if (client[j]->Ready)
					FullReady++;
			}

			if (FullReady == client.size()) {
				state = Game;
				sf::Packet sendPacket; // Déclaration d'un packet pour l'envoi
				sendPacket << state; // Préparation d'un packet

				std::cout << "Tous le monde est pret" << std::endl;

				for (int j = 0; j < client.size(); j++) {
					client[j]->socket->send(sendPacket);
				}
			}
		}
	}
}