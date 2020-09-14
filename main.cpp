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

#define COUNTCYCLES 150
#define DEFRADIUSBICYCLE 150.f
#define DELT_MINUSRADIUS -1.f
#define X0BICYCLES 450.f
#define Y0BICYCLES 200.f
#define K_BICYCLE 90

constexpr unsigned int def_WIN_X = 900;
constexpr unsigned int def_WIN_Y = 400;
constexpr unsigned long def_FPS = 60ul;

#if defined(_WIN32)
constexpr const char* file_image_1 = "files/image_1_750.jpg";
#elif defined(__unix__)
    #if defined(__linux__)
constexpr const char* file_image_1 = "./files/image_1_750.jpg";
    #else
        #error This UNIX operating system is not supported by dx::NN
    #endif
#else
    #error This operating system is not supported by dx::NN
#endif

dxCRYPT hProv;

void mainA(sf::RenderWindow&, sf::View&, volatile ::std::atomic_bool&,
	volatile ::std::atomic<unsigned short>&, volatile ::std::atomic_bool&);

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
	win.setFramerateLimit(def_FPS);

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
	volatile ::std::atomic_bool nativeSpeed;
	volatile ::std::atomic<unsigned short> speedmA;
	isOpen.store(true);
	nativeSpeed.store(true);
	speedmA.store(1);

	::std::thread mainThread(mainA, ::std::ref(win), ::std::ref(view), ::std::ref(isOpen), ::std::ref(speedmA), ::std::ref(nativeSpeed));

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
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
				{
					auto x = speedmA.load();
					if (x > 5)	speedmA.store(x - 5);
					else if (x > 2)	speedmA.store(x - 1);
				}
				else if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
				{
					auto x = speedmA.load();
					speedmA.store(x + 15);
				}
				else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
				{
					auto x = nativeSpeed.load();
					nativeSpeed.store(!x);
					::std::cout << "nativeSpeed - " << (!x ? "true" : "false") << ::std::endl;
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

void mainA(
	sf::RenderWindow& win,
	sf::View& view,
	volatile ::std::atomic_bool& __is_Open,
	volatile ::std::atomic<unsigned short>& SpeedmA,
	volatile ::std::atomic_bool& __NativeSpeed)
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

	bmdx::Bicycle Bicycle1;
	Bicycle1.vvCycle.reserve(COUNTCYCLES);
	for(size_t i = 0ul; i < COUNTCYCLES; ++i)
	{
		ER_IFN(Bicycle1.initCycle2Back(DEFRADIUSBICYCLE + DELT_MINUSRADIUS * i,
			X0BICYCLES, Y0BICYCLES, K_BICYCLE, sf::Color(0, 0, 0, 0), X0BICYCLES, Y0BICYCLES),, )
	}
	sf::Texture* pTx = Bicycle1.addTx(file_image_1);
	
	for(size_t i = 0ul; i < Bicycle1.vvCycle.size(); ++i)
	{
		if (i == 0ul)
		{
			auto pFirst = &Bicycle1.vvCycle[i].front();
			ER_IF(pFirst == nullptr,, )
			auto pCur = Bicycle1.vvCycle[i].front().next;
			ER_IF(pCur == nullptr,, )
			int fFill = static_cast<unsigned int>(38ul/*K_BICYCLE * 0.5 - 2*/);
			pFirst->setColor(sf::Color(255, 255, 255, 255));
			pFirst->setOptions(SETBITS(SETBITS(0x0, 0x1, bmdx::CyclePoint::OPTIONS::DRAW), 0x1, 0x0));
			pCur->setColor(sf::Color(255, 255, 255, 255));
			pCur->setOptions(SETBITS(SETBITS(0x0, 0x1, bmdx::CyclePoint::OPTIONS::DRAW), 0x1, 0x0));
			while(pCur != pFirst)
			{
				pCur = pCur->next;
				ER_IF(pCur == nullptr,, )
				//fFill = nndx::randB(hProv) % 100;
				if ((fFill >= 0) && (fFill % 40 >= 34))
				{
					pCur->setColor(sf::Color(255, 255, 255, 255));
					pCur->setOptions(SETBITS(SETBITS(0x0, 0x1, bmdx::CyclePoint::OPTIONS::DRAW), 0x1, 0x0));
				}
				--fFill;
			}
		}
	}
	
	//sf::Vector2i mouse_pos;
	
	//typedef ::std::chrono::steady_clock __CLOCK_T;
	typedef ::std::chrono::system_clock __CLOCK_T;
    ::std::chrono::time_point<__CLOCK_T> startTime;
	//mA
	while (__is_Open.load())
	{
		startTime = __CLOCK_T::now();

		for(size_t i = 1ull; i < Bicycle1.vvCycle.size(); ++i)
		{
			for(size_t j = 0ul; j < Bicycle1.vvCycle[i].size(); ++j)
			{
				Bicycle1.vvCycle[i][j].setColor(Bicycle1.vvCycle[0][j].getColor());
				Bicycle1.vvCycle[i][j].setOptions(Bicycle1.vvCycle[0][j].getOptions());
			}
		}

		{
			using namespace ::std::chrono;
			typedef microseconds TIME_T;
			auto SpeedX = static_cast<unsigned short>(SpeedmA.load() * duration_cast<TIME_T>(__CLOCK_T::now() - startTime).count() / 10600);
			//::std::cout << SpeedX << ::std::endl;
			if (__NativeSpeed.load())	Bicycle1.setSpeed(SpeedmA.load());
			else	Bicycle1.setSpeed(SpeedX);
			Bicycle1.mA();
		}

		view.setCenter(static_cast<float>(win.getSize().x / 2), static_cast<float>(win.getSize().y / 2));

		win.clear(/*sf::Color::White*/);
		win.setView(view);
		Bicycle1.drawBicycle(win, [](bmdx::CyclePoint* x) -> bool {	if (GETBIT(x->getOptions(), bmdx::CyclePoint::OPTIONS::DRAW)) return true; else return false; }, pTx);
		//win.draw();
		win.display();
	}

	win.setActive(false);
}
