#include "Canon.h"

Arme::Arme(sf::Vector2f pos, float angle, Type arme)
{
	switch (arme)
	{
	case Type::Canon:
		Shape.setSize(sf::Vector2f(20, 20));
		Radius = 50;
		Damage = 45;
		break;
	case Type::Grenade:
		Shape.setSize(sf::Vector2f(20, 20));
		Radius = 75;
		Damage = 55;
		break;
	case Type::Dynamite:
		Shape.setSize(sf::Vector2f(20, 20));
		Radius = 100;
		Damage = 70;
		break;
	case Type::Uzi:
		Shape.setSize(sf::Vector2f(20, 20));
		Radius = 20;
		Damage = 10;
		break;
	default:
		break;
	}
	
	Shape.setOrigin(Shape.getSize().x / 2, Shape.getSize().y / 2);
	Shape.setPosition(pos);
	Shape.setFillColor(sf::Color::Cyan);

	if (arme != Type::Dynamite)
		Velocity = sf::Vector2f(cos(angle) * 1000, sin(angle) * 1000);
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

void Arme::Update(const float& dt, sf::Image& image)
{
	Velocity.y += 1000 * dt;
	Get_NextPos(dt, image);

	Shape.move(Velocity * dt);

	Collide(image);

	if (Shape.getPosition().x < 0 || Shape.getPosition().x > 1920 || Shape.getPosition().y > 1080) {
		Life = false;
	}
}

void Arme::Display(sf::RenderWindow* window)
{
	window->draw(Shape);
}