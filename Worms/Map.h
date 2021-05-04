#pragma once
#include "SFML/Graphics.hpp"

class Map
{
private:
	sf::Image map;
	sf::Texture Texture;
	sf::Sprite Sprite;

public:
	Map();
	~Map() = default;

	sf::Image& Get_Collid() { return map; }
	void DestroyMap(float radius, sf::Vector2f pos);

	void Display(sf::RenderWindow* window);
};