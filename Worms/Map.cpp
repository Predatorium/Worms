#include "Map.h"

Map::Map()
{
	map.loadFromFile("../Ressources/Map.png");
	Texture.loadFromImage(map);
	Sprite.setTexture(Texture);
}

void Map::DestroyMap(float radius, sf::Vector2f pos)
{
	sf::RenderTexture texture;
	texture.create(1920, 1080);

	texture.clear(sf::Color::White);
	texture.draw(Sprite);

	sf::CircleShape tmp(radius);
	tmp.setOrigin(radius, radius);
	tmp.setPosition(pos);
	texture.draw(tmp);

	texture.display();

	Texture = texture.getTexture();
	map = Texture.copyToImage();
	Sprite.setTexture(Texture);
}

void Map::Display(sf::RenderWindow* window)
{
	window->draw(Sprite);
}
