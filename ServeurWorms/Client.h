#pragma once
#include "SFML/Graphics.hpp"
#include "SFML/Network.hpp"
#include <iostream>

class Client
{
public:
	Client(std::string _name) : username(_name) { ; }
	~Client() { delete socket; }

	bool Ready{ false };
	sf::TcpSocket* socket{ nullptr };
	int Id{ 0 };
	std::string username;
	float timeout{ 0 };

	std::vector<std::pair<sf::Vector2f, int>> Worms;

	void CheckPacket(std::vector<Client*>& client);
};