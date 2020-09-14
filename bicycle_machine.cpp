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

sf::Texture* Bicycle::addTx(const char* file)
{
    ER_IF(file == nullptr,, return nullptr; )
    ER_IF(file == "",, return nullptr; )

    sf::Image temp;
    temp.loadFromFile(file);
    vTx.reserve(vTx.capacity() + 1);
    vTx.emplace_back(sf::Texture());
    vTx.back().loadFromImage(temp);

    return &vTx.back();
}

bool Bicycle::initCycle2Back(float r, float x0, float y0, size_t K, sf::Color defColor, float txx0 = 0.f, float txy0 = 0.f)
{
    ER_IF(r <= 0,, return false; )
    ER_IF(K <= 0,, return false; )

    coord startX = r * sqrt(2) / 2;
    coord startY = r * sqrt(2) / 2;
    coord incXY = r * sqrt(2) / K;
    double SqrR = pow(r, 2.);
    CyclePoint* pPrev = nullptr;
    vvCycle.reserve(vvCycle.capacity() + 1);
    vvDrawAbleCycle.reserve(vvDrawAbleCycle.capacity() + 1);
    vvDrawAbleCycle.emplace_back(::std::vector<sf::Vertex>());
    vvCycle.emplace_back(::std::vector<CyclePoint>());
    vvCycle.back().reserve(4 * K);
    vvDrawAbleCycle.back().reserve(4 * K);
    
    coord x = startX + x0;
    coord y = -sqrt(SqrR - pow(x - x0, 2.)) + y0;
    vvDrawAbleCycle.back().emplace_back(sf::Vertex(sf::Vector2f(x, y), defColor, sf::Vector2f(startX + txx0, y - y0 + txy0)));
    vvCycle.back().emplace_back(CyclePoint(&vvDrawAbleCycle.back().back(), 0x0, nullptr, nullptr));
    pPrev = &vvCycle.back().back();
    x -= incXY;
    for(size_t i = 1ull; i < K; ++i)
    {
        y = -sqrt(SqrR - pow(x - x0, 2.)) + y0;
        vvDrawAbleCycle.back().emplace_back(sf::Vertex(sf::Vector2f(x, y), defColor, sf::Vector2f(x - x0 + txx0, y - y0 + txy0)));
        vvCycle.back().emplace_back(CyclePoint(&vvDrawAbleCycle.back().back(), 0x0, pPrev, nullptr));
        pPrev->next = &vvCycle.back().back();
        pPrev = &vvCycle.back().back();
        x -= incXY;
    }

    ER_IF(vvCycle.back().size() != K,
        ::std::cout << "vvCycle.back().size() - " << vvCycle.back().size() << ::std::endl;,
        return false; )
    ER_IF(vvDrawAbleCycle.back().size() != K,
        ::std::cout << "vvDrawAbleCycle.back().size() - " << vvDrawAbleCycle.back().size() << ::std::endl;,
        return false; )

    y = -startY + y0;
    for(size_t i = 0ull; i < K; ++i)
    {
        x = -sqrt(SqrR - pow(y - y0, 2.)) + x0;
        vvDrawAbleCycle.back().emplace_back(sf::Vertex(sf::Vector2f(x, y), defColor, sf::Vector2f(x - x0 + txx0, y - y0 + txy0)));
        vvCycle.back().emplace_back(CyclePoint(&vvDrawAbleCycle.back().back(), 0x0, pPrev, nullptr));
        pPrev->next = &vvCycle.back().back();
        pPrev = &vvCycle.back().back();
        y += incXY;
    }

    ER_IF(vvCycle.back().size() != 2 * K,
        ::std::cout << "vvCycle.back().size() - " << vvCycle.back().size() << ::std::endl;,
        return false; )
    ER_IF(vvDrawAbleCycle.back().size() != 2 * K,
        ::std::cout << "vvDrawAbleCycle.back().size() - " << vvDrawAbleCycle.back().size() << ::std::endl;,
        return false; )

    x = -startX + x0;
    for(size_t i = 0ull; i < K; ++i)
    {
        y = sqrt(SqrR - pow(x - x0, 2.)) + y0;
        vvDrawAbleCycle.back().emplace_back(sf::Vertex(sf::Vector2f(x, y), defColor, sf::Vector2f(x - x0 + txx0, y - y0 + txy0)));
        vvCycle.back().emplace_back(CyclePoint(&vvDrawAbleCycle.back().back(), 0x0, pPrev, nullptr));
        pPrev->next = &vvCycle.back().back();
        pPrev = &vvCycle.back().back();
        x += incXY;
    }

    ER_IF(vvCycle.back().size() != 3 * K,
        ::std::cout << "vvCycle.back().size() - " << vvCycle.back().size() << ::std::endl;,
        return false; )
    ER_IF(vvDrawAbleCycle.back().size() != 3 * K,
        ::std::cout << "vvDrawAbleCycle.back().size() - " << vvDrawAbleCycle.back().size() << ::std::endl;,
        return false; )

    y = startY + y0;
    for(size_t i = 0ull; i < K; ++i)
    {
        x = sqrt(SqrR - pow(y - y0, 2.)) + x0;
        vvDrawAbleCycle.back().emplace_back(sf::Vertex(sf::Vector2f(x, y), defColor, sf::Vector2f(x - x0 + txx0, y - y0 + txy0)));
        vvCycle.back().emplace_back(CyclePoint(&vvDrawAbleCycle.back().back(), 0x0, pPrev, nullptr));
        pPrev->next = &vvCycle.back().back();
        pPrev = &vvCycle.back().back();
        y -= incXY;
    }
    pPrev->next = &vvCycle.back().front();
    vvCycle.back().front().prev = pPrev;

    ER_IF(vvCycle.back().size() != 4 * K,
        ::std::cout << "vvCycle.back().size() - " << vvCycle.back().size() << ::std::endl;,
        return false; )
    ER_IF(vvDrawAbleCycle.back().size() != 4 * K,
        ::std::cout << "vvDrawAbleCycle.back().size() - " << vvDrawAbleCycle.back().size() << ::std::endl;,
        return false; )

    is_Inited = true;
    return true;
}

bool Bicycle::drawBicycle(sf::RenderWindow& win, bool func(bmdx::CyclePoint*), sf::Texture* tx = nullptr)
{
    ER_IFN(win.isOpen(),, return false; )
    ER_IFN(is_Inited,, return false; )

/*
    for(size_t i = 0ull; i < vvDrawAbleCycle.size(); ++i)
    {
        win.draw(&vvDrawAbleCycle[i][0], vvDrawAbleCycle[i].size(), sf::Points, tx); // without func() checking
    }
*/
    for(size_t i = 0ull; i < vvCycle.size(); ++i)
    {
        for(size_t j = 0ull; j < vvCycle[i].size(); ++j)
        {
            auto x = vvCycle[i][j];
            if (func(&x)) win.draw(x.pPoint, 1, sf::Points, tx);
        }
    }

    return true;
}

bool Bicycle::drawBicycle(sf::RenderWindow& win, size_t idx1, size_t idx2, bool func(bmdx::CyclePoint*), sf::Texture* tx = nullptr)
{
    ER_IFN(win.isOpen(),, return false; )
    ER_IFN(is_Inited,, return false; )
    ER_IF(idx1 >= vvCycle.size() || idx1 < 0, ::std::cout << "idx1 >= vvCycle().size() || idx1 < 0" << ::std::endl;, return false; )
    ER_IF(idx2 >= vvCycle.size() || idx2 < 0, ::std::cout << "idx2 >= vvCycle().size() || idx2 < 0" << ::std::endl;, return false; )
    ER_IF(idx1 >= idx2, ::std::cout << "idx1 >= idx2," << ::std::endl;, return false; )

/*
    for(size_t i = 0ull; i < vvDrawAbleCycle.size(); ++i)
    {
        win.draw(&vvDrawAbleCycle[i][0], vvDrawAbleCycle[i].size(), sf::Points, tx); // without func() checking
    }
*/
    for(size_t i = idx1; i < idx2; ++i)
    {
        for(size_t j = 0ull; j < vvCycle[i].size(); ++j)
        {
            auto x = vvCycle[i][j];
            if (func(&x)) win.draw(x.pPoint, 1, sf::Points, tx);
        }
    }
    
    return true;
}

bool Bicycle::mA()
{
    ER_IFN(this->is_Inited, ::std::cout << "is_Inited - " << this->is_Inited << ::std::endl;, return false;)

    for(size_t j = 0ull; j < vvCycle.size(); ++j)
    {
        for(decltype(Bicycle::U) i = 0; i < U; ++i)
        {
            auto pFirst = &vvCycle[j].front();
            auto pCur = vvCycle[j].front().next;
            auto FirstColor = pFirst->getColor();
            auto FirstOpt = pFirst->getOptions();
            pFirst->setColor(pCur->getColor()); // <<<<<-------  THIS
            pFirst->setOptions(pCur->getOptions());
            while(pCur != pFirst->prev)
            {
                pCur = pCur->next;
                pCur->prev->setColor(pCur->getColor());
                pCur->prev->setOptions(pCur->getOptions());
            }
            pCur->setColor(FirstColor);
            pCur->setOptions(FirstOpt);

        }
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