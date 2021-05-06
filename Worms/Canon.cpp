#include "Canon.h"
#include "Tools.h"
#include "Joueur.h"

Arme::Arme(sf::Vector2f pos, float angle, int id, float power, Type arme)
	:ID{ id }, Start{ pos }
{
	switch (arme)
	{
	case Type::Canon:
		Shape.setRadius(5);
		Radius = 50;
		Damage = 45;
		break;
	case Type::Grenade:
		Shape.setRadius(5);
		Radius = 75;
		Damage = 55;
		timer = 4.f;
		break;
	case Type::Dynamite:
		Shape.setRadius(10);
		Radius = 100;
		Damage = 70;
		timer = 3.f;
		break;
	case Type::Uzi:
		Shape.setRadius(2);
		Radius = 5;
		Damage = 5;
		break;
	default:
		break;
	}

	type = arme;
	
	Shape.setOrigin(Shape.getRadius(), Shape.getRadius());
	Shape.setPosition(pos);
	Shape.setFillColor(sf::Color::Cyan);

	if (arme != Type::Dynamite)
		Velocity = sf::Vector2f(cos(angle) * power, sin(angle) * power);
}

void Arme::Get_NextPos(const float& dt, sf::Image& image)
{
	bool find = false;
	sf::Vector2f Next = Shape.getPosition();
	Next.y -= Shape.getGlobalBounds().height / 2;
	if (Velocity.x < 0) {
		Next.x -= Shape.getGlobalBounds().width / 2;

		for (int i = 0; i < (int)Shape.getGlobalBounds().height; i++) {
			for (float y = 0; y < (double)-Velocity.x * dt + 0.1; y += (double)0.1f) {
				if (CheckCollid(sf::Vector2f(Next.x - y, Next.y + i), image)) {
					sf::Vector2f Previous = Velocity;
					Velocity.x = 0;
					Velocity.x += 1.f * length(Previous) * 0.25f;
					Life = false;
					find = true;
					break;
				}
			}
			if (find)
				break;
		}
	}
	else if (Velocity.x > 0) {
		Next.x += Shape.getGlobalBounds().width / 2;

		for (int i = 0; i < (int)Shape.getGlobalBounds().height; i++) {
			for (float y = 0; y < (double)Velocity.x * dt + 0.1; y += (double)0.1f) {
				if (CheckCollid(sf::Vector2f(Next.x + y, Next.y + i), image)) {
					sf::Vector2f Previous = Velocity;
					Velocity.x = 0;
					Velocity.x += -1.f * length(Previous) * 0.25f;
					Life = false;
					find = true;
					break;
				}
			}
			if (find)
				break;
		}
	}

	find = false;
	Next = Shape.getPosition();
	Next.x -= Shape.getGlobalBounds().width / 2;

	if (Velocity.y < 0) {
		Next.y -= Shape.getGlobalBounds().height / 2;

		for (int i = 1; i < (int)Shape.getGlobalBounds().width; i++) {
			for (float y = 0; y < (double)-Velocity.y * dt + 0.1; y += (double)0.1f) {
				if (CheckCollid(sf::Vector2f(Next.x + i, Next.y - y), image)) {
					Shape.setPosition(sf::Vector2f(Shape.getPosition().x, (Next.y - y) + Shape.getGlobalBounds().height / 2));
					sf::Vector2f Previous = Velocity;
					Velocity.y = 0;
					Velocity.y += 1.f * length(Previous) * 0.25f;
					Life = false;
					find = true;
					break;
				}
			}
			if (find)
				break;
		}
	}
	else if (Velocity.y > 0) {
		Next.y += Shape.getGlobalBounds().height / 2;

		for (int i = 1; i < (int)Shape.getGlobalBounds().width; i++) {
			for (float y = 0; y < (double)Velocity.y * dt + 0.1; y += (double)0.1f) {
				if (CheckCollid(sf::Vector2f(Next.x + i, Next.y + y), image)) {
					Shape.setPosition(sf::Vector2f(Shape.getPosition().x, (Next.y + y) - Shape.getGlobalBounds().height / 2));
					sf::Vector2f Previous = Velocity;
					Velocity.y = 0;
					Velocity.y += -1.f * length(Previous) * 0.25f;
					Life = false;
					find = true;
					break;
				}
			}
			if (find)
				break;
		}
	}
}

void Arme::Collide(sf::Image& image)
{
	sf::Vector2i pos = static_cast<sf::Vector2i>(Shape.getPosition());
	pos.x -= (int)Shape.getGlobalBounds().width / 2 + 1;
	pos.y -= (int)Shape.getGlobalBounds().height / 2 + 1;

	for (int i = 0; i < (int)Shape.getGlobalBounds().width; i++)
		if (CheckCollid(sf::Vector2f(pos.x + i, pos.y - 1), image)) {
			Life = false;
			return;
		}

	for (int i = 0; i < (int)Shape.getGlobalBounds().width; i++)
		if (CheckCollid(sf::Vector2f(pos.x + i, pos.y + (int)Shape.getGlobalBounds().height + 2), image)) {
			Life = false;
			return;
		}

	for (int i = 0; i < (int)Shape.getGlobalBounds().height; i++)
		if (CheckCollid(sf::Vector2f(pos.x - 1, pos.y + i), image)) {
			Life = false;
			return;
		}

	for (int i = 0; i < (int)Shape.getGlobalBounds().height; i++)
		if (CheckCollid(sf::Vector2f(pos.x + (int)Shape.getGlobalBounds().width + 2, pos.y + i), image)) {
			Life = false;
			return;
		}
}

void Arme::CollidWorms(Joueur& joueur, std::vector<OtherPlayer>& player)
{
	for (auto& it : joueur.Get_Team()) {
		if (BetweenGlobalBoundsAndCircle(it.second.Get_Shape(), Shape.getPosition(), Shape) &&
			!Circle_Collision(it.second.Get_Position(), Start, it.second.Get_Shape().getGlobalBounds().height / 2)) {
			Life = false;
			return;
		}
	}

	for (auto& itp : player) {
		for (auto& it : itp.Get_Team()) {
			if (BetweenGlobalBoundsAndCircle(it.second.Get_Shape(), Shape.getPosition(), Shape) &&
				!Circle_Collision(it.second.Get_Position(), Start, it.second.Get_Shape().getGlobalBounds().height / 2)) {
				Life = false;
				return;
			}
		}
	}
}

void Arme::Update(const float& dt, int vent, sf::Image& image)
{
	timer -= dt;
	Velocity.y += 1000 * dt;

	if (type == Type::Canon)
		Velocity.x += vent * 25.f * dt;
	if (type == Type::Grenade)
		Velocity.x += vent * 12.5f * dt;
	

	Get_NextPos(dt, image);

	Shape.move(Velocity * dt);

	Collide(image);

	if (Shape.getPosition().x < 0 || Shape.getPosition().x > 1920 || Shape.getPosition().y > 1080) {
		Life = false;
	}
}

void Arme::Display(sf::RenderWindow* window, sf::Font& font)
{
	if (timer > 0) {
		sf::Text tmp = CreateText(std::to_string(static_cast<int>(timer)), font);
		tmp.setFillColor(sf::Color::Red);
		tmp.setPosition(Shape.getPosition().x, Shape.getPosition().y - Shape.getRadius() * 2);

		window->draw(tmp);
	}

	window->draw(Shape);
}