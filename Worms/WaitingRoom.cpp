#include "WaitingRoom.h"
#include "State_Manager.h"
#include "Tools.h"
#include "Menu.h"
#include "GameState.h"

WaitingRoom::WaitingRoom(State_Manager* game, sf::RenderWindow* _window)
	: State(game, _window)
{
	font.loadFromFile("../Ressources/Sunflower.otf");
	Title = CreateText("Worms Revogeddon", font, 100);
	Title.setPosition(960, 50);

	Socket = std::make_shared<sf::TcpSocket>();

	Connecting = Button(CreateText("Connect", font, 50),
		CreateRectangle(1, { 200,50 }), { 960,650 },
		[this] {
			if (Socket->connect(Ip, 9666) == sf::Socket::Done)
			{
				Socket->setBlocking(false);			// On rend le socket non bloquant
				sf::Packet sendPacket;				// Déclaration d'un packet
				sendPacket << New;					// Préparation d'un packet
				Socket->send(sendPacket);			// Envoi de ce paquet au serveur

				Current = Etat::Connect;
			}
			else {
				Ip.clear();
			}
		});

	Quit = Button(CreateText("Quit", font, 50),
		CreateRectangle(1, { 200,50 }), { 960,850 },
		[this] { 
			Game->ChangeState<Menu>(Game, window);
			if (Current == Etat::Connect) {
				sf::Packet sendPacket;				// Déclaration d'un packet
				sendPacket << Disconnect;			// Préparation d'un packet
				Socket->send(sendPacket);			// Envoi de ce paquet au serveur
			}
		});

	IReady = Button(CreateText("Ready", font, 50),
		CreateRectangle(1, { 200,50 }), { 860,750 },
		[this] {

			sf::Packet sendPacket;				// Déclaration d'un packet
			sendPacket << Ready;				// Préparation d'un packet
			Socket->send(sendPacket);			// Envoi de ce paquet au serveur

		});

	INoReady = Button(CreateText("No Ready", font, 50),
		CreateRectangle(1, { 200,50 }), { 1060,750 },
		[this] {

			sf::Packet sendPacket;				// Déclaration d'un packet
			sendPacket << NoReady;				// Préparation d'un packet
			Socket->send(sendPacket);			// Envoi de ce paquet au serveur

		});
}

void WaitingRoom::ReceptionServeur()
{
	sf::Packet receivePacket; // Déclaration d'un packet
		// On vérifie si le serveur nous a envoyé un packet 
	if (Socket->receive(receivePacket) == sf::Socket::Done) {

		int state;
		receivePacket >> state;

		if (state == 0) {
			int type;
			receivePacket >> type;

			if (type == ME) {
				receivePacket >> Me;
			}
			if (type == New) {
				int id;
				std::string name;
				receivePacket >> id >> name;
				OtherPlayer.emplace(std::make_pair(id, name));
			}
			if (type == Disconnect) {
				int id;
				receivePacket >> id;

				OtherPlayer.erase(OtherPlayer.find(id));
			}
			if (type == AddOtherPlayer) {
				int id;
				std::string name;
				receivePacket >> id >> name;
				OtherPlayer.emplace(std::make_pair(id, name));
			}
		}
		else {
			Game->ChangeState<GameState>(Game, window, OtherPlayer, Me, Socket);
		}
	}
}

void WaitingRoom::HandleEvents(sf::Event e)
{
	switch (e.type)
	{
	case sf::Event::TextEntered: {
		if (Current == Etat::Disco) {
			if (std::isprint(e.text.unicode)) {
				Ip += e.text.unicode;
			}
		}
		break;
	}
	case sf::Event::Closed:
		window->close();
		break;
	case sf::Event::KeyPressed:
		switch (e.key.code)
		{
		case sf::Keyboard::Key::BackSpace: {
			if (Current == Etat::Disco) {
				if (!Ip.empty())
					Ip.pop_back();
			}
			break;
		}
		default:
			break;
		}
		break;
	default:
		break;
	}

	if (!window->isOpen()) {
		sf::Packet sendPacket;				// Déclaration d'un packet
		sendPacket << Disconnect;			// Préparation d'un packet
		Socket->send(sendPacket);			// Envoi de ce paquet au serveur
	}
}

void WaitingRoom::Update(const float& dt)
{
	timer += dt;

	ReceptionServeur();

	if (!Click) {
		if (Current == Etat::Disco) {
			Click = Connecting.Update(window);
		}

		if (Current == Etat::Connect) {
			Click = IReady.Update(window);
			Click = INoReady.Update(window);
		}

		Click = Quit.Update(window);
	}
	if (!sf::Mouse::isButtonPressed(sf::Mouse::Left))
		Click = false;

	if (timer > 2.f) {
		sf::Packet sendPacket;				// Déclaration d'un packet
		sendPacket << 5000;					// Préparation d'un packet
		Socket->send(sendPacket);			// Envoi de ce paquet au serveur
		timer = 0;
	}
}

void WaitingRoom::Display()
{
	if (Current == Etat::Disco) {
		Connecting.Display(window);
	}
	if (Current == Etat::Connect) {
		IReady.Display(window);
		INoReady.Display(window);
	}

	Quit.Display(window);

	window->draw(Title);

	if (Current == Etat::Disco) {
		sf::Text tmp = CreateText(Ip, font, 50);
		tmp.setPosition(960, 500);

		sf::RectangleShape Rtmp = CreateRectangle(1, { 500, 50 });
		Rtmp.setPosition(tmp.getPosition());
		Rtmp.setFillColor(sf::Color(150, 150, 150));

		window->draw(Rtmp);
		window->draw(tmp);
	}
}