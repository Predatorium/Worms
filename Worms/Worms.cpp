#include "Worms.h"
#include <math.h>

Worms::Worms(sf::Vector2f _pos, sf::Color _team)
{
	Shape.setSize(sf::Vector2f(40, 80));
	Shape.setOrigin(Shape.getSize().x / 2, Shape.getSize().y / 2);
	Shape.setPosition(_pos);
	Shape.setFillColor(_team);
	if (_pos.x > 960)
		Angle = pi;

}

void Worms::Move_Angle(float move, const float& dt)
{
	Angle += move * dt;
}

void Worms::Jump(float jump, sf::Image& image)
{
	sf::Vector2f Next = Shape.getPosition();
	Next.x -= Shape.getGlobalBounds().width / 2;
	Next.y += Shape.getGlobalBounds().height / 2;

	for (int i = 1; i < (int)Shape.getGlobalBounds().width; i++) {
		if (CheckCollid(sf::Vector2f(Next.x + i, Next.y + 1), image)) {
			Velocity.y = -jump;
			break;
		}
	}
}

void Worms::Shoot(std::vector<Arme>& shoot)
{
	shoot.push_back(Arme(Shape.getPosition() + sf::Vector2f(cos(Angle) * 100, sin(Angle) * 100), Angle));
}

void Worms::Update(const float& dt, sf::Image& image)
{
	Velocity.y += 1000 * dt;
	Get_NextPos(dt, image);

	Shape.move(Velocity * dt);

	if (Shape.getPosition().x < 0 || Shape.getPosition().x > 1920 || Shape.getPosition().y > 1080) {
		Life = 0;
	}
}

void Worms::Display(sf::RenderWindow* window)
{
	sf::RectangleShape Barre(sf::Vector2f(Shape.getGlobalBounds().width - 2, 20));
	Barre.setOrigin((Shape.getGlobalBounds().width - 2) / 2, 20 / 2);
	Barre.setPosition(Shape.getPosition().x, Shape.getPosition().y - 65);
	Barre.setFillColor(sf::Color::Transparent);
	Barre.setOutlineThickness(1);
	Barre.setOutlineColor(sf::Color(150,150,150));

	sf::RectangleShape LifeBarre(sf::Vector2f(Shape.getGlobalBounds().width - 2 * (100 / Life), 20));
	LifeBarre.setPosition(Barre.getPosition().x - Barre.getGlobalBounds().width / 2,
		Shape.getPosition().y - 65 - LifeBarre.getGlobalBounds().height / 2);
	LifeBarre.setFillColor(sf::Color::Red);

	window->draw(LifeBarre);
	window->draw(Barre);
	window->draw(Shape);
}

void Worms::DisplayDirAttack(sf::RenderWindow* window)
{
	float Radiant = Angle * (pi / 180);

	sf::CircleShape tmp(20);
	tmp.setFillColor(sf::Color::Transparent);
	tmp.setOutlineThickness(-1);
	tmp.setOutlineColor(sf::Color::Blue);
	tmp.setPosition(Shape.getPosition() + sf::Vector2f(cos(Angle) * 100, sin(Angle) * 100));
	tmp.setOrigin(tmp.getRadius(), tmp.getRadius());

	window->draw(tmp);
}
