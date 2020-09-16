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

#define COUNTCYCLES 30
#define DEFRADIUSBICYCLE 150.f
#define DELT_RADIUS -1.f
#define X0BICYCLES 450.f
#define Y0BICYCLES 200.f
#define K_BICYCLE 45

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
					auto x = speedmA.load(::std::memory_order_acquire);
					if (x > 1)
					{
						speedmA.store(x - 1);
						::std::cout << "ITERCOUNT - " << x - 1 << ::std::endl;
						//::std::cout << "U - " << ((x - 1) * 2 * M_PI * DEFRADIUSBICYCLE) / 4 * K_BICYCLE << ::std::endl;
					}
				}
				else if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
				{
					auto x = speedmA.load(::std::memory_order_acquire);
					speedmA.store(x + 1);
					::std::cout << "ITERCOUNT - " << x + 1 << ::std::endl;
					//::std::cout << "U - " << ((x + 1) * 2 * M_PI * DEFRADIUSBICYCLE) / 4 * K_BICYCLE << ::std::endl;
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

	bmdx::CycleMachine testMachine(COUNTCYCLES, DEFRADIUSBICYCLE, DELT_RADIUS, X0BICYCLES, Y0BICYCLES, K_BICYCLE,
		sf::Color(0, 0, 0, 255), X0BICYCLES, Y0BICYCLES);
	ER_IFN(testMachine.AddColor2CyclePart(0ul, 1ul, K_BICYCLE, 15, sf::Color(255, 255, 255, 255), bmdx::CycleMachine::GenerateStates::TRIANGLE),, )
	ER_IFN(testMachine.addTexture(file_image_1),, ) // нужно как-то с ассоциировать с каким-то ключом (имя_файла, номер), сделать что-то вроде хеш-таблицы
	
	//sf::Vector2i mouse_pos;
	
	//typedef ::std::chrono::steady_clock __CLOCK_T;
	typedef ::std::chrono::system_clock __CLOCK_T;
    ::std::chrono::time_point<__CLOCK_T> startTime;
	//mA
	while (__is_Open.load())
	{
		startTime = __CLOCK_T::now();

		ER_IFN(testMachine.DefaultCopyBetweenCycles(0, 1, testMachine.getCycleSize() - 1),, )

		{
			using namespace ::std::chrono;
			typedef microseconds TIME_T;
			unsigned short SpeedX = static_cast<unsigned short>(SpeedmA.load() * duration_cast<TIME_T>(__CLOCK_T::now() - startTime).count() / 225);
			//::std::cout << SpeedX << ::std::endl;
			ER_IFN(testMachine.mA(SpeedmA.load(), SpeedX, __NativeSpeed.load()),, )
		}

		view.setCenter(static_cast<float>(win.getSize().x / 2), static_cast<float>(win.getSize().y / 2));

		win.clear(/*sf::Color::White*/);
		win.setView(view);
		ER_IFN(testMachine.drawCycles(win,
			[](bmdx::CyclePoint* x) -> bool {	if (GETBIT(x->getOptions(), bmdx::CyclePoint::OPTIONS::DRAW)) return true; else return false; }, 1),, )
		//win.draw();
		win.display();
	}

	win.setActive(false);
}
