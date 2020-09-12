#include <iostream>
#include <thread>
#include <atomic>
#include <bitset>

#include "SFML/Graphics.hpp"

#include "bicycle_machine.h"

constexpr unsigned int def_WIN_X = 900;
constexpr unsigned int def_WIN_Y = 400;

void mainA(sf::RenderWindow&, sf::View&, volatile ::std::atomic_bool&);

int main(int argc, char** argv)
{
	sf::RenderWindow win({ def_WIN_X, def_WIN_Y }, "ProjectX");
	win.setVerticalSyncEnabled(true);
	win.setActive(false);
	win.setFramerateLimit(60);

	sf::View view;
	view.reset(sf::FloatRect(0.0f, 0.0f, static_cast<float>(def_WIN_X), static_cast<float>(def_WIN_Y)));

	volatile ::std::atomic_bool isOpen;
	isOpen.store(true);

	::std::thread mainThread(mainA, ::std::ref(win), ::std::ref(view), ::std::ref(isOpen));

	while (isOpen.load())
	{
		sf::Event event;
		while (win.waitEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				isOpen.store(false);
				break;
			}
			else if (event.type == sf::Event::KeyPressed)
			{
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::O))
				{

				}
				else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
				{

				}
				else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
				{

				}
				else if ((sf::Keyboard::isKeyPressed(sf::Keyboard::RShift)) || (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)))
				{

				}
			}
			else if (event.type == sf::Event::Resized)
			{
				view.reset(sf::FloatRect(0.f, 0.f, static_cast<float>(win.getSize().x), static_cast<float>(win.getSize().y)));
			}
		}
	}

	mainThread.join();
	win.setActive(true);
	win.close();

	return 0;
}

void mainA(sf::RenderWindow& win, sf::View& view, volatile ::std::atomic_bool& is_Open)
{
	win.setActive(true);

	bmdx::Bicycle test;
	ER_IFN(test.initCycle(100.f, 200.f, 200.f, 3600, sf::Color::Red),, )
	ER_IFN(test.getState(),, )
	
	//sf::Vector2i mouse_pos;

	//mA
	while (is_Open.load())
	{
		//mouse_pos = static_cast<sf::Vector2i>(win.mapPixelToCoords(sf::Mouse::getPosition(win)));

		view.setCenter(static_cast<float>(win.getSize().x / 2), static_cast<float>(win.getSize().y / 2));

		win.clear();
		win.setView(view);
		test.drawBicycle(win);
		//win.draw();
		win.display();
	}

	win.setActive(false);
}
