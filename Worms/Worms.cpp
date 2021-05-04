#include "Worms.h"

Worms::Worms(sf::Vector2f _pos, sf::Color _team)
{
	Shape.setSize(sf::Vector2f(50, 100));
	Shape.setOrigin(25, 50);
	Shape.setPosition(_pos);
	Shape.setFillColor(_team);
}

bool Worms::CheckCollid(sf::Vector2f _pos, sf::Image& image)
{
	if (sf::FloatRect(0, 0, image.getSize().x, image.getSize().y).contains(_pos)) {
		return image.getPixel(_pos.x, _pos.y) == sf::Color::Black;
	}
	return true;
}

void Worms::Get_NextPos(const float& dt, sf::Image& image)
{
	sf::Vector2f Next = Shape.getPosition();
	Next.y -= Shape.getGlobalBounds().height / 2;
	if (Velocity.x < 0) {
		Next.x -= Shape.getGlobalBounds().width / 2;

		for (int i = 5; i < (int)Shape.getGlobalBounds().height - 5; i++) {
			for (float y = 0; y < -Velocity.x * dt + 0.1; y += 0.1) {
				if (CheckCollid(sf::Vector2f(Next.x - y, Next.y + i), image)) {
					Shape.setPosition(sf::Vector2f((Next.x + y) - Shape.getGlobalBounds().width / 2, Shape.getPosition().y));
					Velocity.x = 0;
				}
			}
		}
	}
	else if (Velocity.x > 0) {
		Next.x += Shape.getGlobalBounds().width / 2;

		for (int i = 5; i < (int)Shape.getGlobalBounds().height - 5; i++) {
			for (float y = 0; y < Velocity.x * dt + 0.1; y += 0.1) {
				if (CheckCollid(sf::Vector2f(Next.x + y, Next.y + i), image)) {
					Shape.setPosition(sf::Vector2f((Next.x + y) - Shape.getGlobalBounds().width / 2, Shape.getPosition().y));
					Velocity.x = 0;
				}
			}
		}
	}

	Next = Shape.getPosition();
	Next.x -= Shape.getGlobalBounds().width / 2;

	if (Velocity.y < 0) {
		Next.y -= Shape.getGlobalBounds().height / 2;

		for (int i = 1; i < (int)Shape.getGlobalBounds().width; i++) {
			for (float y = 0; y < -Velocity.y * dt + 0.1; y += 0.1) {
				if (CheckCollid(sf::Vector2f(Next.x + i, Next.y - y), image)) {
					Shape.setPosition(sf::Vector2f(Shape.getPosition().x, (Next.y - y) + Shape.getGlobalBounds().height / 2));
					Velocity.y = 0;
				}
			}
		}
	}
	else if (Velocity.y > 0) {
		Next.y += Shape.getGlobalBounds().height / 2;

		for (int i = 1; i < (int)Shape.getGlobalBounds().width; i++) {
			for (float y = 0; y < Velocity.y * dt + 0.1; y += 0.1) {
				if (CheckCollid(sf::Vector2f(Next.x + i, Next.y + y), image)) {
					Shape.setPosition(sf::Vector2f(Shape.getPosition().x, (Next.y + y) - Shape.getGlobalBounds().height / 2));
					Velocity.y = 0;
				}
			}
		}
	}
}

void Worms::Move(sf::Vector2f _move)
{
	Velocity = _move;
}

void Worms::Update(const float& dt, sf::Image& image)
{
	Velocity.y += 100 * dt;
	Get_NextPos(dt, image);

	Shape.move(Velocity * dt);

	if (Shape.getPosition().x < 0 || Shape.getPosition().x > 1920 || Shape.getPosition().y > 1080) {
		Life = 0;
	}
}

void Worms::Display(sf::RenderWindow* window)
{
	window->draw(Shape);
}
