#pragma once
#include "Worms.h"
#include <vector>
#include "SFML/Network.hpp"
#include <map>
#include "Button.h"
#include "Canon.h"

class Joueur
{
private:
	std::map<int, Worms> Team;
	int WormsTurn{ 0 };
	int Id{ 0 };
	int iteration{ 0 };
	float Timer{ 0 };
	float TimerSend{ 0 };
	std::string Name{ "Me" };
	bool Shoot{ true };
	bool ReShoot{ true };
	bool Charge{ true };
	float Power{ 0 };

	std::shared_ptr<sf::TcpSocket> Socket;
	Arme::Type Weapon{ Arme::Type::Canon };
	std::vector<Button> SelectWeapon;

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
		ChangeTurn,
	};

public:
	Joueur() = default;
	Joueur(std::vector<sf::Vector2f> pos, int id,
		std::shared_ptr<sf::TcpSocket> socket, sf::Font& font);
	~Joueur() = default;

	void NextWorms();
	int Get_Id() { return Id; }
	std::map<int, Worms> Get_Team() { return Team; }

	void Controle(sf::Image& image, std::vector<Arme>& shoot,
		const float& dt, float& timer, sf::RenderWindow* window);
	void Damage(int damage, sf::CircleShape shape);

	bool EraseWorms();
	void Update(const float& dt, sf::Image& image);
	void Display(sf::RenderWindow* window, sf::Font& font);
	void DisplayTirAngle(sf::RenderWindow* window);
};

class OtherPlayer
{
public:
	OtherPlayer() = default;
	OtherPlayer(std::vector<sf::Vector2f> pos, int id,
		std::string name);
	~OtherPlayer() = default;

	int Get_Id() { return Id; }
	std::string Get_Name() { return Name; }
	std::map<int, Worms> Get_Team() { return Team; }

	void SetPos(sf::Vector2f pos, int id);
	void SetLife(int life, int id);
	void Delete(int id);
	void Display(sf::RenderWindow* window, sf::Font& font);

private:
	int Id{ 0 };
	std::string Name{ "Me" };
	std::map<int, Worms> Team;
};