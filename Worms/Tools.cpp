#include "Tools.h"

int irandom(int Min, int Max)
{
	return rand() % (Max - Min + 1) + Min;
}

sf::Vector2f getMidle(sf::Sprite& _o)
{
	return sf::Vector2f(_o.getGlobalBounds().width / 2, _o.getGlobalBounds().height / 2);
}

sf::Vector2f getMidle(sf::RectangleShape& _o)
{
	return sf::Vector2f(_o.getGlobalBounds().width / 2, _o.getGlobalBounds().height / 2);
}

sf::Vector2f getMidle(sf::Text& _o)
{
	return sf::Vector2f(_o.getLocalBounds().left + _o.getLocalBounds().width / 2,
		_o.getLocalBounds().top + _o.getLocalBounds().height / 2);
}

sf::Text CreateText(std::string _text, sf::Font& _font, unsigned charactersize)
{
	sf::Text text(_text, _font, charactersize);
	text.setOrigin(getMidle(text));
	return text;
}

sf::RectangleShape CreateRectangle(float _outlinesize, sf::Vector2f _size)
{
	sf::RectangleShape shape(_size);
	shape.setOutlineThickness(_outlinesize);
	shape.setOrigin(getMidle(shape));
	return shape;
}

bool Circle_Collision(sf::Vector2f Pos1, sf::Vector2f Pos2, float Rayon)noexcept
{
	if (sqrt(pow(Pos1.x - Pos2.x, 2) + pow(Pos1.y - Pos2.y, 2)) < Rayon)
	{
		return true;
	}
	else
		return false;
}