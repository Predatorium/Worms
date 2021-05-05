#include "Joueur.h"
#include "Tools.h"

Joueur::Joueur(std::vector<sf::Vector2f> pos, int id,
	std::shared_ptr<sf::TcpSocket> socket) : Id{ id }, Socket{socket}
{
	sf::Color tmp;
	switch (id)
	{
	case 1:
		tmp = sf::Color::Blue;
		break;
	case 2:
		tmp = sf::Color::Red;
		break;
	case 3:
		tmp = sf::Color::Green;
		break;
	case 4:
		tmp = sf::Color::Magenta;
		break;
	default:
		break;
	}

	int i = 0;
	for (auto& it : pos) {
		Team.emplace(std::make_pair(i, Worms(it, tmp)));

		sf::Packet packet;
		packet << 1 << Add_Worms << i << it.x << it.y;
		Socket->send(packet);
		
		i++;
	}
}

void Joueur::NextWorms()
{
	bool done = true;
	while (done) {
		WormsTurn++;

		if (WormsTurn > Team.size()) {
			WormsTurn = 0;
		}

		for (auto& it : Team) {
			if (WormsTurn == it.first) {
				done = false;
			}
		}
	}
}

void Joueur::Controle(sf::Image& image, std::vector<Arme>& shoot, const float& dt, float& timer)
{
	if (Team.size() > 0) {
		if (Team[WormsTurn].Sol) {
			Team[WormsTurn].Movable = true;
			if (Team[WormsTurn].Movable) {
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
					Team[WormsTurn].Move(100);
					Team[WormsTurn].Set_Angle(0);
				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {
					Team[WormsTurn].Move(-100);
					Team[WormsTurn].Set_Angle(pi);
				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
					Team[WormsTurn].Jump(400, image);
				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::E) && Shoot) {
					Team[WormsTurn].Shoot(shoot, Id, Weapon);

					int arme = static_cast<int>(Weapon);

					sf::Packet packet;
					packet << 1 << Attack << Team[WormsTurn].Get_Angle() << 20 <<
						Team[WormsTurn].Get_Position().x << Team[WormsTurn].Get_Position().y
						<< arme;
					Socket->send(packet);

					Shoot = false;

					NextWorms();
				}
				else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::E))
					Shoot = true;
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z)) {
					Team[WormsTurn].Move_Angle(5, dt);
				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
					Team[WormsTurn].Move_Angle(-5, dt);
				}
			}
		}
	}
}

void Joueur::Damage(int damage, sf::Vector2f position, float radius)
{
	sf::CircleShape tmp(radius);
	tmp.setOrigin(radius, radius);
	tmp.setPosition(position);

	for (auto& it : Team) {
		if (BetweenGlobalBoundsAndCircle(it.second.Get_Shape(), position, tmp)) {
			it.second.Damage(damage);
			it.second.Move(normalize(it.second.Get_Position() - position) * 500.f);
		}
	}
}

void Joueur::Update(const float& dt, sf::Image& image)
{
	TimerSend += dt;

	for (auto& it : Team) {
		it.second.Update(dt, image);

		if (TimerSend > 0.02f) {
			sf::Packet packet;
			packet << 1 << Update_Pos << it.first << it.second.Get_Position().x <<
				it.second.Get_Position().y << it.second.Get_Life();
			Socket->send(packet);
		}

		if (it.second.Movable)
			it.second.StopVeloX();

		it.second.Movable = false;
	}

	if (TimerSend > 0.02f) {
		TimerSend = 0;
	}

	for (auto it = std::begin(Team); it != std::end(Team); it++) {
		if (it->second.Get_Life() <= 0) {

			sf::Packet packet;
			packet << 1 << Delete_Worms << it->first;
			Socket->send(packet);

			Team.erase(it);
			break;
		}
	}
}

void Joueur::Display(sf::RenderWindow* window, sf::Font& font)
{
	for (auto& it : Team)
		it.second.Display(window, font);

	if (Team.size() > 0)
		Team[WormsTurn].DisplayDirAttack(window);
}

OtherPlayer::OtherPlayer(std::vector<sf::Vector2f> pos, int id,
	std::string name) : Name{ name }, Id{ id }
{
	sf::Color tmp;
	switch (id)
	{
	case 1:
		tmp = sf::Color::Blue;
		break;
	case 2:
		tmp = sf::Color::Red;
		break;
	case 3:
		tmp = sf::Color::Green;
		break;
	case 4:
		tmp = sf::Color::Magenta;
		break;
	default:
		break;
	}

	int i = 0;
	for (auto& it : pos) {
		Team.emplace(std::make_pair(i, Worms(it, tmp)));
		i++;
	}
}

void OtherPlayer::Damage(int damage, sf::Vector2f position, float radius)
{
	sf::CircleShape tmp(radius);
	tmp.setOrigin(radius, radius);
	tmp.setPosition(position);

	for (auto& it : Team) {
		if (BetweenGlobalBoundsAndCircle(it.second.Get_Shape(), position, tmp)) {
			it.second.Damage(damage);
			it.second.Move(normalize(it.second.Get_Position() - position) * 500.f);
		}
	}
}

void OtherPlayer::SetPos(sf::Vector2f pos, int id)
{
	Team.find(id)->second.Set_Position(pos);
}

void OtherPlayer::SetLife(int life, int id)
{
	Team.find(id)->second.Set_Life(life);
}

void OtherPlayer::Delete(int id)
{
	Team.erase(Team.find(id));
}

void OtherPlayer::Display(sf::RenderWindow* window, sf::Font& font)
{
	for (auto& it : Team)
		it.second.Display(window, font);
}