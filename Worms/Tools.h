#pragma once
#include "SFML/Graphics.hpp"

int irandom(int Min, int Max);
sf::Vector2f getMidle(sf::Sprite& _o);
sf::Vector2f getMidle(sf::RectangleShape& _o);
sf::Vector2f getMidle(sf::Text& _o);

sf::Text CreateText(std::string _text, sf::Font& _font, unsigned charactersize = 30U);
sf::RectangleShape CreateRectangle(float _outlinesize = 0, sf::Vector2f _size = { 0,0 });
bool Circle_Collision(sf::Vector2f Pos1, sf::Vector2f Pos2, float Rayon)noexcept;

template<typename T1, typename T3>
[[nodiscard]] inline bool BetweenGlobalBoundsAndCircle(T1 type1, sf::Vector2f pos, T3 type3)
{
	float X = pos.x;
	float Y = pos.y;

	if (pos.x < type1.getGlobalBounds().left)
		X = type1.getGlobalBounds().left;
	else if (pos.x > type1.getGlobalBounds().left + type1.getGlobalBounds().width)
		X = type1.getGlobalBounds().left + type1.getGlobalBounds().width;
	if (pos.y < type1.getGlobalBounds().top)
		Y = type1.getGlobalBounds().top;
	else if (pos.y > type1.getGlobalBounds().top + type1.getGlobalBounds().height)
		Y = type1.getGlobalBounds().top + type1.getGlobalBounds().height;

	float distX = pos.x - X;
	float distY = pos.y - Y;
	float distance = sqrtf((distX * distX) + (distY * distY));

	if (distance <= type3.getRadius())
		return true;

	return false;
}

[[nodiscard]] inline float length(sf::Vector2f vector)noexcept
{
	return sqrtf(vector.x * vector.x + vector.y * vector.y);
}

[[nodiscard]] inline sf::Vector2f perpendicularVector(sf::Vector2f vector)noexcept
{
	return sf::Vector2f(-vector.x, vector.y);
}

[[nodiscard]] inline sf::Vector2f normalize(sf::Vector2f v)noexcept {
	return v / length(v);
}

[[nodiscard]] inline float distance(sf::Vector2f x, sf::Vector2f y)noexcept {
	return length(x - y);
}

[[nodiscard]] inline sf::ConvexShape line(sf::Color col, sf::Color ouC, float ouT, sf::Vector2f dirT, sf::Vector2f dirR)noexcept
{
	sf::ConvexShape convex;
	convex.setPointCount(4);
	convex.setFillColor(col);
	convex.setOutlineColor(ouC);
	convex.setOutlineThickness(ouT);
	convex.setPoint(0, dirT);
	convex.setPoint(1, dirR);
	convex.setPoint(2, dirR);
	convex.setPoint(3, dirT);

	return convex;
}