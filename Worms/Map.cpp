#include "Map.h"

Map::Map()
{
	map.loadFromFile("../Ressources/Map.png");
	Texture.loadFromImage(map);
	Sprite.setTexture(Texture);
}

void Map::DestroyMap(float radius, sf::Vector2f pos)
{
	sf::RenderTexture Rtexture;
	Rtexture.create(1920, 1080);

	Rtexture.clear(sf::Color::White);
	Rtexture.draw(Sprite);

	sf::CircleShape tmp(radius);
	tmp.setOrigin(radius, radius);
	tmp.setPosition(pos);
	Rtexture.draw(tmp);

	Rtexture.display();

	Texture = Rtexture.getTexture();
	map = Texture.copyToImage();
	Sprite.setTexture(Texture);
}

void Map::Display(sf::RenderWindow* window)
{
	window->draw(Sprite);
}