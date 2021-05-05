#include "Entity.h"

bool Entity::CheckCollid(sf::Vector2f _pos, sf::Image& image)
{
	if (sf::FloatRect(0, 0, image.getSize().x, image.getSize().y).contains(_pos)) {
		return image.getPixel(_pos.x, _pos.y) == sf::Color::Black;
	}

	if (_pos.y < 0)
		return false;

	return true;
}

void Entity::Get_NextPos(const float& dt, sf::Image& image)
{
	bool find = false;
	sf::Vector2f Next = Shape.getPosition();
	Next.y -= Shape.getGlobalBounds().height / 2;
	if (Velocity.x < 0) {
		Next.x -= Shape.getGlobalBounds().width / 2;

		for (int i = 0; i < (int)Shape.getGlobalBounds().height; i++) {
			for (float y = 0; y < -Velocity.x * dt + 0.1; y += 0.1) {
				if (CheckCollid(sf::Vector2f(Next.x - y, Next.y + i), image)) {
					if (i >= (int)Shape.getGlobalBounds().height - 2) {
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
			for (float y = 0; y < Velocity.x * dt + 0.1; y += 0.1) {
				if (CheckCollid(sf::Vector2f(Next.x + y, Next.y + i), image)) {
					if (i >= (int)Shape.getGlobalBounds().height - 2) {
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
			for (float y = 0; y < -Velocity.y * dt + 0.1; y += 0.1) {
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
			for (float y = 0; y < Velocity.y * dt + 0.1; y += 0.1) {
				if (CheckCollid(sf::Vector2f(Next.x + i, Next.y + y), image)) {
					Shape.setPosition(sf::Vector2f(Shape.getPosition().x, (Next.y + y) - Shape.getGlobalBounds().height / 2));
					Velocity.y = 0;
					Sol = true;
					Movable = true;
					find = true;
					break;
				}
			}
			if (find)
				break;
		}
	}
}

void Entity::Move(sf::Vector2f move)
{
	Velocity = move;
	Sol = false;
}