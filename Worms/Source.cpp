#include "Loop.h"

int main()
{
	Loop Game(sf::VideoMode(1920,1080), "Worms", sf::Style::Fullscreen);
	Game.GameLoop();

	return 0;
}