#include "Button.h"
#include "Tools.h"

Button::Button(sf::Text&& _text, sf::RectangleShape&& _shape, sf::Vector2f _position,
	std::function<void()> _f)
	: Texte(_text), Shape{ _shape }
{
	Out = sf::Color::White;
	In = sf::Color::Red;

	Shape.setSize(sf::Vector2f(Texte.getLocalBounds().width,
		Texte.getLocalBounds().height));
	Shape.setFillColor(sf::Color::Transparent);
	Texte.setOrigin(getMidle(Texte));
	Shape.setOrigin(getMidle(Shape));
	Texte.setPosition(sf::Vector2f(_position));
	Shape.setPosition(_position);

	f = _f;
}

Button::Button(sf::Text&& _text, sf::RectangleShape&& _shape, sf::Color _color,
	sf::Vector2f _position, std::function<void()> _f)
	: Texte(_text), Shape{ _shape }
{
	Out = sf::Color::White;
	In = sf::Color::Red;

	Shape.setFillColor(_color);
	Texte.setOrigin(getMidle(Texte));
	Shape.setOrigin(getMidle(Shape));
	Texte.setPosition(sf::Vector2f(_position));
	Shape.setPosition(_position);

	f = _f;
}

void Button::Set_Color(sf::Color _colortext, sf::Color _outlinecolor)
{
	Texte.setFillColor(_colortext);
	Shape.setOutlineColor(_outlinecolor);
}

void Button::Set_CallBack(std::function<int()> _f)
{
	f = _f;
}

void Button::Set_Text(std::string _texte)
{
	Texte.setString(_texte);
	Shape.setSize(sf::Vector2f(Texte.getLocalBounds().width,
		Texte.getLocalBounds().height));
}

void Button::Set_Position(sf::Vector2f _pos)
{
	Texte.setPosition(_pos);
	Shape.setPosition(_pos);
}

bool Button::Update()
{
	Set_ColorText(Out);
	if (Shape.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition()))) {
		Set_ColorText(In);
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
			f();
			Click = true;
			return true;
		}
	}

	return false;
}

void Button::Display(sf::RenderWindow* _window)
{
	_window->draw(Shape);
	_window->draw(Texte);
}