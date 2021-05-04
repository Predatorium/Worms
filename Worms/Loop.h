#pragma once
#include "SFML/Graphics.hpp"
#include "State_Manager.h"
#include "Times.h"

class Loop
{
private:
	sf::RenderWindow window;
	State_Manager state_manager;
	Times Time;

public:
	Loop() = default;
	Loop(sf::VideoMode _videomode, std::string _name, sf::Uint32 _style);
	~Loop() = default;

	void GameLoop();
};