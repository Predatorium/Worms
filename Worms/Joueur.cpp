#include "Joueur.h"
#include "Tools.h"
#include <iostream>

Joueur::Joueur(std::vector<sf::Vector2f> pos, int id,
	std::shared_ptr<sf::TcpSocket> socket, sf::Font& font) : Id{ id }, Socket{socket}
{
	sf::Color tmp;
	switch (id)
	{
	case 1:
		tmp = sf::Color::Blue;
		break;
	case 2:
		tmp = sf::Color::Yellow;
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

	SelectWeapon.push_back(Button(CreateText("Bazooka", font), CreateRectangle(1.f, { 200,48 }),
		{ 150,150,150 }, { 1220, 1055 }, [] {}));

	SelectWeapon.push_back(Button(CreateText("Grenade", font), CreateRectangle(1.f, { 200,48 }),
		{ 150,150,150 }, { 1420, 1055 }, [] {}));

	SelectWeapon.push_back(Button(CreateText("Dynamite", font), CreateRectangle(1.f, { 200,48 }),
		{ 150,150,150 }, { 1620, 1055 }, [] {}));

	SelectWeapon.push_back(Button(CreateText("Uzi", font), CreateRectangle(1.f, { 200,48 }),
		{ 150,150,150 }, { 1820, 1055 }, [] {}));
}

void Joueur::NextWorms()
{
	ReShoot = true;

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

void Joueur::Controle(sf::Image& image, std::vector<Arme>& shoot, const float& dt, float& timer, sf::RenderWindow* window)
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
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::E) && Shoot && ReShoot) {
					if (Weapon == Arme::Type::Dynamite) {
						Team[WormsTurn].Shoot(shoot, Id, Weapon, 0);
						int arme = static_cast<int>(Weapon);

						sf::Packet packet;
						packet << 1 << Attack << 0 << 0 << Team[WormsTurn].Get_Position().x <<
							Team[WormsTurn].Get_Position().y << arme;
						Socket->send(packet);

						ReShoot = false;

						if (timer > 10)
							timer = 10;
					}
					else {
						Shoot = false;
						ReShoot = false;

						if (Weapon != Arme::Type::Uzi) {
							Charge = false;
						}
						else {
							Team[WormsTurn].Shoot(shoot, Id, Weapon, 10000);
							int arme = static_cast<int>(Weapon);

							sf::Packet packet;
							packet << 1 << Attack << Team[WormsTurn].Get_Angle() << 10000 <<
								Team[WormsTurn].Get_Position().x << Team[WormsTurn].Get_Position().y
								<< arme;
							Socket->send(packet);

							iteration++;
						}
					}
				}
				else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::E)) {
					Charge = true;
				}
					
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z)) {
					Team[WormsTurn].Move_Angle(3, dt);
				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
					Team[WormsTurn].Move_Angle(-3, dt);
				}

				if (!Charge) {
					if (Power < 1000)
						Power += dt * 500;
					else
						Charge = true;
				}

				if (!Shoot && Charge) {

					if (Weapon != Arme::Type::Uzi) {
						Team[WormsTurn].Shoot(shoot, Id, Weapon, Power);

						int arme = static_cast<int>(Weapon);

						sf::Packet packet;
						packet << 1 << Attack << Team[WormsTurn].Get_Angle() << Power <<
							Team[WormsTurn].Get_Position().x << Team[WormsTurn].Get_Position().y
							<< arme;
						Socket->send(packet);

						Power = 0;
						Shoot = true;
						if (timer > 10)
							timer = 10;
					}
					else {
						Timer += dt;

						if (Timer > 0.5f) {
							Team[WormsTurn].Shoot(shoot, Id, Weapon, 10000);

							int arme = static_cast<int>(Weapon);

							sf::Packet packet;
							packet << 1 << Attack << Team[WormsTurn].Get_Angle() << 10000 <<
								Team[WormsTurn].Get_Position().x << Team[WormsTurn].Get_Position().y
								<< arme;
							Socket->send(packet);

							iteration++;
							Timer = 0;
						}

						if (iteration >= 10) {
							iteration = 0;
							Shoot = true;
							if (timer > 10)
								timer = 10;
						}
					}
				}
			}
		}

		int i = 0;
		for (auto& it : SelectWeapon) {
			if (it.Update(window)) {
				Weapon = static_cast<Arme::Type>(i);
			}

			i++;
		}
	}


}

void Joueur::Damage(int damage, sf::CircleShape shape)
{
	for (auto& it : Team) {
		if (BetweenGlobalBoundsAndCircle(it.second.Get_Shape(), shape.getPosition(), shape)) {
			it.second.Damage(damage);
			it.second.Move(normalize(it.second.Get_Position() - shape.getPosition()) * 500.f);
		}
	}
}

bool Joueur::EraseWorms()
{
	for (auto it = std::begin(Team); it != std::end(Team); it++) {
		if (it->second.Get_Life() <= 0) {

			sf::Packet packet;
			packet << 1 << Delete_Worms << it->first;
			Socket->send(packet);

			Team.erase(it);
			return true;
		}
	}

	return false;
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

		if (it.second.Movable && it.second.Sol)
			it.second.StopVeloX();

		it.second.Movable = false;
	}

	if (TimerSend > 0.02f) {
		TimerSend = 0;
	}
}

void Joueur::Display(sf::RenderWindow* window, sf::Font& font)
{
	for (auto& it : Team) {
		it.second.Display(window, font);
	}

	sf::Text tmp;
	
	switch (Weapon)
	{
	case Arme::Type::Canon:
		tmp = CreateText("Bazooka", font);
		break;
	case Arme::Type::Grenade:
		tmp = CreateText("Grenade", font);
		break;
	case Arme::Type::Dynamite:
		tmp = CreateText("Dynamite", font);
		break;
	case Arme::Type::Uzi:
		tmp = CreateText("Uzi", font);
		break;
	}
	
	tmp.setFillColor(sf::Color::Black);
	tmp.setPosition(200, 50);
	window->draw(tmp);
}

void Joueur::DisplayTirAngle(sf::RenderWindow* window)
{
	if (Team.size() > 0) {
		if (ReShoot || !Shoot) {
			Team[WormsTurn].DisplayDirAttack(window);

			window->draw(line(sf::Color::Red, sf::Color::Red, 1,
				Team[WormsTurn].Get_Position(), Team[WormsTurn].Get_Position() +
				sf::Vector2f(cos(Team[WormsTurn].Get_Angle()) * (Power / 10),
					sin(Team[WormsTurn].Get_Angle()) * (Power / 10))));
		}

		for (auto& it : SelectWeapon) {
			it.Display(window);
		}
	}
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
		tmp = sf::Color::Yellow;
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
	for (auto& it : Team) {
		it.second.Display(window, font);
	}
}