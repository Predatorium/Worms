#pragma once
#include "SFML/Graphics.hpp"
#include <string>
#include <functional>

class Button
{
public:
	Button() = default;
	Button(sf::Text&& _text, sf::RectangleShape&& _shape,
		sf::Vector2f _position, std::function<void()> _f);
	Button(sf::Text&& _text, sf::RectangleShape&& _shape, sf::Color _color,
		sf::Vector2f _position, std::function<void()> _f);
	~Button() = default;

	inline sf::Text& Get_Texte() { return Texte; };
	inline sf::RectangleShape& Get_Shape() { return Shape; };

	inline void Set_ColorText(sf::Color _color) { Texte.setFillColor(_color); };
	inline void Set_ColorShape(sf::Color _color) { Shape.setOutlineColor(_color); };
	void Set_Color(sf::Color _colortext, sf::Color _outlinecolor);
	void Set_CallBack(std::function<int()> _f);
	void Set_Position(sf::Vector2f _pos);
	void Set_Text(std::string _texte);

	bool Update();
	void Display(sf::RenderWindow* _window);

protected:
	sf::RectangleShape Shape;
	sf::Text Texte;

	sf::Color Out;
	sf::Color In;
	bool Click{ false };

	std::function<void()> f;
};