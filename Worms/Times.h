#pragma once
#include "SFML/Graphics.hpp"

class Times
{
public:
	Times() = default;
	~Times() = default;

	inline float Get_TimeDeltaF() { return DeltaF; };
	inline sf::Time GetTimeDeltaT() const { return DeltaTime; };

	void RestartClock() { DeltaTime = Clock.restart(); DeltaF = DeltaTime.asSeconds(); };

private:
	sf::Time DeltaTime;
	sf::Clock Clock;
	float DeltaF{ 0 };
};