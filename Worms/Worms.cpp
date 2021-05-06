#include "Worms.h"
#include <math.h>
#include "Tools.h"

Worms::Worms(sf::Vector2f _pos, sf::Color _team)
{
	Shape.setSize(sf::Vector2f(20, 40));
	Shape.setOrigin(Shape.getSize().x / 2, Shape.getSize().y / 2);
	Shape.setPosition(_pos);
	Shape.setFillColor(_team);
	if (_pos.x > 960)
		Angle = pi;

}

void Worms::Set_Life(int life)
{
	Received -= Life - life;
	Life = life;
}

void Worms::Move_Angle(float move, const float& dt)
{
	Angle += move * dt;
}

void Worms::Get_NextPos(const float& dt, sf::Image& image)
{
	bool find = false;
	sf::Vector2f Next = Shape.getPosition();
	Next.y -= Shape.getGlobalBounds().height / 2;
	if (Velocity.x < 0) {
		Next.x -= Shape.getGlobalBounds().width / 2;

		for (int i = 0; i < (int)Shape.getGlobalBounds().height; i++) {
			for (float y = 0; y < (double)-Velocity.x * dt + 0.1; y += (double)0.1f) {
				if (CheckCollid(sf::Vector2f(Next.x - y, Next.y + i), image)) {
					if (i >= (int)Shape.getGlobalBounds().height - 3 && Sol) {
						Velocity.y -= 2;
					}
					else {
						Shape.setPosition(sf::Vector2f((Next.x - y) + Shape.getGlobalBounds().width / 2, Shape.getPosition().y));
						Velocity.x = 0;
					}

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
					if (i >= (int)Shape.getGlobalBounds().height - 3 && Sol) {
						Velocity.y -= 2;
					}
					else {
						Shape.setPosition(sf::Vector2f((Next.x + y) - Shape.getGlobalBounds().width / 2, Shape.getPosition().y));
						Velocity.x = 0;
					}
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
					Velocity.y = 0;
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
					Velocity.y = 0;

					if ((int)DamageFall - 10 > 0) {
						Damage(-((int)DamageFall - 10));
					}

					DamageFall = 0;
					find = true;
					break;
				}
			}
			if (find)
				break;
		}
	}
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

void Worms::Shoot(std::vector<Arme>& shoot, int id, Arme::Type arme, float power)
{
	shoot.push_back(Arme(Shape.getPosition(),
		Angle, id, power, arme));
}

void Worms::Damage(int damage)
{
	Received += damage;
	Life += damage;
}

void Worms::Update(const float& dt, sf::Image& image)
{
	if (Received < 0 && DamageText.size() == 0) {
		timer += dt;

		if (timer > 1.f) {
			DamageText = std::to_string(Received);
			timer = 0;
		}
	}

	if (DamageText.size() > 0) {
		timer += dt;

		if (timer > 2.f) {
			timer = 0;
			Received = 0;
			DamageText.clear();
		}
	}

	if (!Sol) {
		DamageFall += 20 * dt;
	}

	Velocity.y += 1000 * dt;
	Get_NextPos(dt, image);

	Shape.move(Velocity * dt);

	sf::Vector2f Next = Shape.getPosition();
	Next.x -= Shape.getGlobalBounds().width / 2;
	Next.y += Shape.getGlobalBounds().height / 2 + 1;
	for (int i = 2; i < (int)Shape.getGlobalBounds().width - 2; i++)
		if (CheckCollid(sf::Vector2f(Next.x + i, Next.y), image)) {
			Sol = true;
			break;
		}

	if (Shape.getPosition().x < 0 || Shape.getPosition().x > 1920 || Shape.getPosition().y > 1080) {
		Life = 0;
	}
}

void Worms::Display(sf::RenderWindow* window, sf::Font& font)
{
	sf::Text tmp = CreateText(std::to_string(Life), font, 15);
	tmp.setFillColor(sf::Color::Green);
	tmp.setPosition(Shape.getPosition().x, Shape.getPosition().y - Shape.getGlobalBounds().height);

	if (DamageText.size() > 0) {

		sf::Text tmp2 = CreateText(DamageText, font, 15);
		tmp2.setFillColor(sf::Color::Red);
		tmp2.setPosition(tmp.getPosition().x, tmp.getPosition().y - tmp.getGlobalBounds().height * 2);

		window->draw(tmp2);
	}

	window->draw(tmp);
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
