#include "bicycle_machine.h"

namespace nndx
{

#if defined(_WIN32)
	dxFastInt32 randT(dxCRYPT& hProv)
	{
		BYTE Buf1 = 0b0;

		if (CryptGenRandom(hProv, DWORD(sizeof(BYTE)), &Buf1))
		{
			dxFastInt32 i = static_cast<dxFastInt32>(Buf1);
			if (CryptGenRandom(hProv, DWORD(sizeof(BYTE)), &Buf1))
			{
				i += static_cast<dxFastInt32>(Buf1);
				return i;
			}
			else
			{
				return 0;
			}
		}
		else
		{
			return 0;
		}
	}

	dxFastInt32 randB(dxCRYPT& hProv)
	{
		BYTE Buf1 = 0b0;

		if (CryptGenRandom(hProv, DWORD(sizeof(BYTE)), &Buf1))
		{
			dxFastInt32 i = static_cast<dxFastInt32>(Buf1);
			i <<= 0b1000;

			if (CryptGenRandom(hProv, DWORD(sizeof(BYTE)), &Buf1))
			{
				i += static_cast<dxFastInt32>(Buf1);
				i <<= 0b1000;

				if (CryptGenRandom(hProv, DWORD(sizeof(BYTE)), &Buf1))
				{
					i += static_cast<dxFastInt32>(Buf1);
					i <<= 0b1000; 

					if (CryptGenRandom(hProv, DWORD(sizeof(BYTE)), &Buf1))
					{
						i += static_cast<dxFastInt32>(Buf1);
						return i;
					}
					else
					{
						return 0;
					}
				}
				else
				{
					return 0;
				}
			}
			else
			{
				return 0;
			}
		}
		else
		{
			return 0;
		}
	}
#elif defined(__unix__)
    #if defined(__linux__)
		dxFastInt32 randT(dxCRYPT& hProv)
		{
			/*dxFastInt32 temp = hProv() % 0b1'1111'1111;
			::std::cout << temp << ::std::endl;*/
			return hProv() % 0b1'1111'1111;
		}

		dxFastInt32 randB(dxCRYPT& hProv)
		{
			/*dxFastInt32 temp = hProv();
			::std::cout << temp << ::std::endl;*/
			return hProv();
		}
    #else
        #error This UNIX operating system is not supported by dx::NN
    #endif
#else
    #error This operating system is not supported by dx::NN
#endif

}

namespace bmdx
{

CyclePoint::CyclePoint(sf::Vertex* rfPoint, int aOpt, CyclePoint* aprev, CyclePoint* anext)
{
    this->pPoint = rfPoint;
    this->options = aOpt;
    this->prev = aprev;
    this->next = anext;
}

CyclePoint::~CyclePoint()
{
    this->prev = nullptr;
    this->next = nullptr;
}

sf::Color CyclePoint::getColor()
{
    return pPoint->color;
}

void CyclePoint::setColor(sf::Color acolor)
{
    this->pPoint->color = acolor;
}

void CyclePoint::setOptions(decltype(CyclePoint::options) aOpt)
{
    this->options = aOpt;
}

decltype(CyclePoint::options) CyclePoint::getOptions()
{
    return options;
}

Bicycle::Bicycle() : is_Inited(false), U(1u)
{

}

Bicycle::~Bicycle()
{
    
}

bool Bicycle::initCycle(float r, float x0, float y0, size_t K, sf::Color defColor/*, ::std::map<size_t, CyclePoint> specDefs = {}*/)
{
    ER_IF(r <= 0,, return false; )
    ER_IF(K <= 0,, return false; )

    coord startX = r * sqrt(2) / 2;
    coord startY = r * sqrt(2) / 2;
    coord incXY = r * sqrt(2) / K;
    double SqrR = pow(r, 2.);
    CyclePoint* pPrev = nullptr;
    vCycle.reserve(4 * K);
    DrawAbleCycle1.reserve(4 * K);
    
    coord x = startX + x0;
    coord y = -sqrt(SqrR - pow(x - x0, 2.)) + y0;
    DrawAbleCycle1.emplace_back(sf::Vertex(sf::Vector2f(x, y), defColor));
    vCycle.emplace_back(CyclePoint(&DrawAbleCycle1.back(), 0x0, nullptr, nullptr));
    pPrev = &vCycle.back();
    x -= incXY;
    for(size_t i = 1ull; i < K; ++i)
    {
        y = -sqrt(SqrR - pow(x - x0, 2.)) + y0;
        DrawAbleCycle1.emplace_back(sf::Vertex(sf::Vector2f(x, y), defColor));
        vCycle.emplace_back(CyclePoint(&DrawAbleCycle1.back(), 0x0, pPrev, nullptr));
        pPrev->next = &vCycle.back();
        pPrev = &vCycle.back();
        x -= incXY;
    }

    ER_IF(vCycle.size() != K,
        ::std::cout << "vCycle.size() - " << vCycle.size() << ::std::endl;,
        return false; )
    ER_IF(DrawAbleCycle1.size() != K,
        ::std::cout << "DrawAbleCycle1.size() - " << DrawAbleCycle1.size() << ::std::endl;,
        return false; )

    y = -startY + y0;
    for(size_t i = 0ull; i < K; ++i)
    {
        x = -sqrt(SqrR - pow(y - y0, 2.)) + x0;
        DrawAbleCycle1.emplace_back(sf::Vertex(sf::Vector2f(x, y), defColor));
        vCycle.emplace_back(CyclePoint(&DrawAbleCycle1.back(), 0x0, pPrev, nullptr));
        pPrev->next = &vCycle.back();
        pPrev = &vCycle.back();
        y += incXY;
    }

    ER_IF(vCycle.size() != 2 * K,
        ::std::cout << "vCycle.size() - " << vCycle.size() << ::std::endl;,
        return false; )
    ER_IF(DrawAbleCycle1.size() != 2 * K,
        ::std::cout << "DrawAbleCycle1.size() - " << DrawAbleCycle1.size() << ::std::endl;,
        return false; )

    x = -startX + x0;
    for(size_t i = 0ull; i < K; ++i)
    {
        y = sqrt(SqrR - pow(x - x0, 2.)) + y0;
        DrawAbleCycle1.emplace_back(sf::Vertex(sf::Vector2f(x, y), defColor));
        vCycle.emplace_back(CyclePoint(&DrawAbleCycle1.back(), 0x0, pPrev, nullptr));
        pPrev->next = &vCycle.back();
        pPrev = &vCycle.back();
        x += incXY;
    }

    ER_IF(vCycle.size() != 3 * K,
        ::std::cout << "vCycle.size() - " << vCycle.size() << ::std::endl;,
        return false; )
    ER_IF(DrawAbleCycle1.size() != 3 * K,
        ::std::cout << "DrawAbleCycle1.size() - " << DrawAbleCycle1.size() << ::std::endl;,
        return false; )

    y = startY + y0;
    for(size_t i = 0ull; i < K; ++i)
    {
        x = sqrt(SqrR - pow(y - y0, 2.)) + x0;
        DrawAbleCycle1.emplace_back(sf::Vertex(sf::Vector2f(x, y), defColor));
        vCycle.emplace_back(CyclePoint(&DrawAbleCycle1.back(), 0x0, pPrev, nullptr));
        pPrev->next = &vCycle.back();
        pPrev = &vCycle.back();
        y -= incXY;
    }
    pPrev->next = &vCycle.front();
    vCycle.front().prev = pPrev;

    ER_IF(vCycle.size() != 4 * K,
        ::std::cout << "vCycle.size() - " << vCycle.size() << ::std::endl;,
        return false; )
    ER_IF(DrawAbleCycle1.size() != 4 * K,
        ::std::cout << "DrawAbleCycle1.size() - " << DrawAbleCycle1.size() << ::std::endl;,
        return false; )

    is_Inited = true;
    return true;
}

bool Bicycle::drawBicycle(sf::RenderWindow& win)
{
    ER_IFN(win.isOpen(),, return false; )
    ER_IFN(is_Inited,, return false; )

    //win.draw(&DrawAbleCycle1[0], DrawAbleCycle1.size(), sf::Points);
    
    for(size_t i = 0ull; i < vCycle.size(); ++i)
    {
        win.draw(vCycle[i].pPoint, 1, sf::Points);
    }
    
    return true;
}

bool Bicycle::mA()
{
    ER_IFN(this->is_Inited, ::std::cout << "is_Inited - " << this->is_Inited << ::std::endl;, return false;)

    for(decltype(Bicycle::U) i = 0; i < U; ++i)
    {
        auto First = vCycle[0].pPoint;
        for(size_t i = 0ull; i < this->vCycle.size() - 1; ++i)
        {
			::std::cout << i << ::std::endl;
            vCycle[i].pPoint->color = vCycle[i + 1].pPoint->color;
            vCycle[i].pPoint = vCycle[i + 1].pPoint;
        }
        vCycle.back().pPoint = First;
    }

    return true;
}

void Bicycle::setSpeed(decltype(Bicycle::U) aSpeed)
{
    this->U = aSpeed;
}

decltype(Bicycle::U) Bicycle::getSpeed()
{
    return U;
}

bool Bicycle::getState()
{
    return is_Inited;
}

}