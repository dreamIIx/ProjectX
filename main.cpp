#include <iostream>
#include <thread>
#include <atomic>
#include <bitset>

#include "SFML/Graphics.hpp"

#include "bicycle_machine.h"

#if defined(__unix__)
#if defined(__linux__)
#include <X11/Xlib.h>
#else
#error This UNIX operating system is not supported by dx::NN
#endif
#endif

#define COUNTBICYCLES 5
#define DEFRADIUSBICYCLE 150.f
#define X0BICYCLES 450.f
#define Y0BICYCLES 200.f
#define K_BICYCLE 180

constexpr unsigned int def_WIN_X = 900;
constexpr unsigned int def_WIN_Y = 400;

dxCRYPT hProv;

void mainA(sf::RenderWindow&, sf::View&, volatile ::std::atomic_bool&);

int main(int argc, char** argv)
{
#if defined(_WIN32)
#elif defined(__unix__)
    #if defined(__linux__)
    	XInitThreads();
    #else
        #error This UNIX operating system is not supported by dx::NN
    #endif
#else
    #error This operating system is not supported by dx::NN
#endif

	sf::RenderWindow win({ def_WIN_X, def_WIN_Y }, "ProjectX");
	win.setVerticalSyncEnabled(true);
	win.setActive(false);
	win.setFramerateLimit(60);

#if defined(_WIN32)
	if (!CryptAcquireContext(&hProv, 0, NULL, PROV_RSA_FULL, 0))
		if (GetLastError() == NTE_BAD_KEYSET)
			if (!CryptAcquireContext(&hProv, 0, NULL, PROV_RSA_FULL, CRYPT_NEWKEYSET))
			{
				ERROR_
					system("pause");
				return 0;
			}
#elif defined(__unix__)
    #if defined(__linux__)
		hProv.seed(::std::chrono::system_clock::to_time_t(::std::chrono::system_clock::now()));
    #else
        #error This UNIX operating system is not supported by dx::NN
    #endif
#else
    #error This operating system is not supported by dx::NN
#endif

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
#if defined(_WIN32)
#elif defined(__unix__)
    #if defined(__linux__)
    	XInitThreads();
    #else
        #error This UNIX operating system is not supported by dx::NN
    #endif
#else
    #error This operating system is not supported by dx::NN
#endif
	
	win.setActive(true);

	::std::vector<bmdx::Bicycle> vBicycle;
	vBicycle.reserve(COUNTBICYCLES);
	for(size_t i = 0ull; i < COUNTBICYCLES; ++i)
	{
		vBicycle.emplace_back(bmdx::Bicycle());
		ER_IFN(vBicycle.back().initCycle(DEFRADIUSBICYCLE - 10.f * i, X0BICYCLES, Y0BICYCLES, K_BICYCLE, sf::Color::Red),, )
	}
	
	for(size_t i = 0ull; i < vBicycle.size(); ++i)
	{
		auto pFirst = &vBicycle[i].vCycle.front();
		ER_IF(pFirst == nullptr,, )
		auto pCur = vBicycle[i].vCycle.front().next;
		ER_IF(pCur == nullptr,, )
		auto fFill = nndx::randB(hProv) % 100;
		if (fFill < 20)
		{
			pCur->setColor(sf::Color::Cyan);
		}
		while(pCur != pFirst)
		{
			pCur = pCur->next;
			ER_IF(pCur == nullptr,, )
			fFill = nndx::randB(hProv) % 100;
			if (fFill < 20)
			{
				pCur->setColor(sf::Color::Cyan);
			}
		}
	}
	
	//sf::Vector2i mouse_pos;

	//mA
	while (is_Open.load())
	{
		//mouse_pos = static_cast<sf::Vector2i>(win.mapPixelToCoords(sf::Mouse::getPosition(win)));

		view.setCenter(static_cast<float>(win.getSize().x / 2), static_cast<float>(win.getSize().y / 2));

		for(size_t i = 0ull; i < vBicycle.size(); ++i)
		{
			auto pFirst = &vBicycle[i].vCycle.front();
			ER_IF(pFirst == nullptr,, )
			auto pCur = vBicycle[i].vCycle.front().next;
			ER_IF(pCur == nullptr,, )
			auto FirstColor = pFirst->getColor();
			auto FirstOpt = pFirst->getOptions();
			pFirst->setColor(pCur->getColor());
			pFirst->setOptions(pCur->getOptions());
			while(pCur != pFirst->prev)
			{
				pCur = pCur->next;
				ER_IF(pCur == nullptr,, )
				pCur->prev->setColor(pCur->getColor());
				pCur->prev->setOptions(pCur->getOptions());
			}
			pCur->setColor(FirstColor);
			pCur->setOptions(FirstOpt);
		}

		win.clear();
		win.setView(view);
		for(auto& x : vBicycle)
		{
			x.drawBicycle(win);
		}
		//win.draw();
		win.display();
	}

	win.setActive(false);
}
