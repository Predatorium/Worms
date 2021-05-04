#pragma once
#include <stack>
#include "SFML/Graphics.hpp"
#include <thread>
#include <mutex>

class State;

class State_Manager
{
public:
	State_Manager() = default;
	State_Manager(sf::RenderWindow* _window);
	~State_Manager();

	template<typename T, class ...Args>
	void ChangeState(Args ...args);

	template<typename T, class ...Args>
	void PushState(Args ...args);

	void PopState();

	void HandleEvents(sf::Event e);
	void Update(const float& dt);
	void Display();

private:
	sf::RenderWindow* window;
	std::stack<State*> states;
};

template<typename T, class ...Args>
inline void State_Manager::ChangeState(Args ...args)
{
	if (!states.empty())
		states.pop();

	states.push(new T(args...));
}

template<typename T, class ...Args>
inline void State_Manager::PushState(Args ...args)
{
	if (!states.empty())
		states.top()->Pause();

	states.push(new T(args...));
}