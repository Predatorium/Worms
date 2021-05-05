#pragma once
#include "Worms.h"
#include <vector>
#include "SFML/Network.hpp"
#include <map>

class Joueur
{
private:
	std::map<int, Worms> Team;
	int WormsTurn{ 0 };
	int Id{ 0 };
	float TimerSend{ 0 };
	std::string Name{ "Me" };
	bool Shoot{ true };

	std::shared_ptr<sf::TcpSocket> Socket;
	Arme::Type Weapon{ Arme::Type::Canon };

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

public:
	Joueur() = default;
	Joueur(std::vector<sf::Vector2f> pos, int id,
		std::shared_ptr<sf::TcpSocket> socket);
	~Joueur() = default;

	void NextWorms();
	int Get_Id() { return Id; }

	void Controle(sf::Image& image, std::vector<Arme>& shoot, const float& dt, float& timer);
	void Damage(int damage, sf::Vector2f position, float radius);

	void Update(const float& dt, sf::Image& image);
	void Display(sf::RenderWindow* window, sf::Font& font);
};

class OtherPlayer
{
public:
	OtherPlayer() = default;
	OtherPlayer(std::vector<sf::Vector2f> pos, int id,
		std::string name);
	~OtherPlayer() = default;

	int Get_Id() { return Id; }
	void Damage(int damage, sf::Vector2f position, float radius);

	void SetPos(sf::Vector2f pos, int id);
	void SetLife(int life, int id);
	void Delete(int id);
	void Display(sf::RenderWindow* window, sf::Font& font);

private:
	int Id{ 0 };
	std::string Name{ "Me" };
	std::map<int, Worms> Team;
};