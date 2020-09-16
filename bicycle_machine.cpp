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

void CyclePoint::setColor(const sf::Color& acolor)
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

Bicycle::VectorCycle::VectorCycle() : data(::std::vector<CyclePoint>())
{

}

sf::Texture* Bicycle::addTx(const char* file)
{
    ER_IF(file == nullptr,, return nullptr; )
    ER_IF(file == "",, return nullptr; )

    sf::Image temp;
    if (temp.loadFromFile(file))
    {
        
        vTx.reserve(vTx.capacity() + 1);
        vTx.emplace_back(sf::Texture());
        vTx.back().loadFromImage(temp);
    }

    return &vTx.back();
}

bool Bicycle::initCycle2Back(float r, float x0, float y0, size_t K, sf::Color&& defColor, float txx0 = 0.f, float txy0 = 0.f)
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
    vvCycle.emplace_back(VectorCycle());
    vvCycle.back().data.reserve(KF_MULT * K);
    vvCycle.back().KF_Cycle = K;
    vvDrawAbleCycle.back().reserve(KF_MULT * K);
    
    coord x = startX + x0;
    coord y = -sqrt(SqrR - pow(x - x0, 2.)) + y0;
    vvDrawAbleCycle.back().emplace_back(sf::Vertex(sf::Vector2f(x, y), ::std::forward<decltype(defColor)>(defColor), sf::Vector2f(startX + txx0, y - y0 + txy0)));
    vvCycle.back().data.emplace_back(CyclePoint(&vvDrawAbleCycle.back().back(), 0x0, nullptr, nullptr));
    pPrev = &vvCycle.back().data.back();
    x -= incXY;
    for(size_t i = 1ull; i < K; ++i)
    {
        y = -sqrt(SqrR - pow(x - x0, 2.)) + y0;
        vvDrawAbleCycle.back().emplace_back(sf::Vertex(sf::Vector2f(x, y), ::std::forward<decltype(defColor)>(defColor), sf::Vector2f(x - x0 + txx0, y - y0 + txy0)));
        vvCycle.back().data.emplace_back(CyclePoint(&vvDrawAbleCycle.back().back(), 0x0, pPrev, nullptr));
        pPrev->next = &vvCycle.back().data.back();
        pPrev = &vvCycle.back().data.back();
        x -= incXY;
    }

    ER_IF(vvCycle.back().data.size() != K,
        ::std::cout << "vvCycle.back().data.size() - " << vvCycle.back().data.size() << ::std::endl;,
        return false; )
    ER_IF(vvDrawAbleCycle.back().size() != K,
        ::std::cout << "vvDrawAbleCycle.back().size() - " << vvDrawAbleCycle.back().size() << ::std::endl;,
        return false; )

    y = -startY + y0;
    for(size_t i = 0ull; i < K; ++i)
    {
        x = -sqrt(SqrR - pow(y - y0, 2.)) + x0;
        vvDrawAbleCycle.back().emplace_back(sf::Vertex(sf::Vector2f(x, y), ::std::forward<decltype(defColor)>(defColor), sf::Vector2f(x - x0 + txx0, y - y0 + txy0)));
        vvCycle.back().data.emplace_back(CyclePoint(&vvDrawAbleCycle.back().back(), 0x0, pPrev, nullptr));
        pPrev->next = &vvCycle.back().data.back();
        pPrev = &vvCycle.back().data.back();
        y += incXY;
    }

    ER_IF(vvCycle.back().data.size() != 2 * K,
        ::std::cout << "vvCycle.back().data.size() - " << vvCycle.back().data.size() << ::std::endl;,
        return false; )
    ER_IF(vvDrawAbleCycle.back().size() != 2 * K,
        ::std::cout << "vvDrawAbleCycle.back().size() - " << vvDrawAbleCycle.back().size() << ::std::endl;,
        return false; )

    x = -startX + x0;
    for(size_t i = 0ull; i < K; ++i)
    {
        y = sqrt(SqrR - pow(x - x0, 2.)) + y0;
        vvDrawAbleCycle.back().emplace_back(sf::Vertex(sf::Vector2f(x, y), ::std::forward<decltype(defColor)>(defColor), sf::Vector2f(x - x0 + txx0, y - y0 + txy0)));
        vvCycle.back().data.emplace_back(CyclePoint(&vvDrawAbleCycle.back().back(), 0x0, pPrev, nullptr));
        pPrev->next = &vvCycle.back().data.back();
        pPrev = &vvCycle.back().data.back();
        x += incXY;
    }

    ER_IF(vvCycle.back().data.size() != 3 * K,
        ::std::cout << "vvCycle.back().data.size() - " << vvCycle.back().data.size() << ::std::endl;,
        return false; )
    ER_IF(vvDrawAbleCycle.back().size() != 3 * K,
        ::std::cout << "vvDrawAbleCycle.back().size() - " << vvDrawAbleCycle.back().size() << ::std::endl;,
        return false; )

    y = startY + y0;
    for(size_t i = 0ull; i < K; ++i)
    {
        x = sqrt(SqrR - pow(y - y0, 2.)) + x0;
        vvDrawAbleCycle.back().emplace_back(sf::Vertex(sf::Vector2f(x, y), ::std::forward<decltype(defColor)>(defColor), sf::Vector2f(x - x0 + txx0, y - y0 + txy0)));
        vvCycle.back().data.emplace_back(CyclePoint(&vvDrawAbleCycle.back().back(), 0x0, pPrev, nullptr));
        pPrev->next = &vvCycle.back().data.back();
        pPrev = &vvCycle.back().data.back();
        y -= incXY;
    }
    pPrev->next = &vvCycle.back().data.front();
    vvCycle.back().data.front().prev = pPrev;

    ER_IF(vvCycle.back().data.size() != KF_MULT * K,
        ::std::cout << "vvCycle.back().data.size() - " << vvCycle.back().data.size() << ::std::endl;,
        return false; )
    ER_IF(vvDrawAbleCycle.back().size() != KF_MULT * K,
        ::std::cout << "vvDrawAbleCycle.back().size() - " << vvDrawAbleCycle.back().size() << ::std::endl;,
        return false; )

    is_Inited = true;
    return true;
}

bool Bicycle::initCycle2Back(float r, float x0, float y0, size_t K, const sf::Color& defColor, float txx0 = 0.f, float txy0 = 0.f)
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
    vvCycle.emplace_back(VectorCycle());
    vvCycle.back().data.reserve(KF_MULT * K);
    vvCycle.back().KF_Cycle = K;
    vvDrawAbleCycle.back().reserve(KF_MULT * K);
    
    coord x = startX + x0;
    coord y = -sqrt(SqrR - pow(x - x0, 2.)) + y0;
    vvDrawAbleCycle.back().emplace_back(sf::Vertex(sf::Vector2f(x, y), defColor, sf::Vector2f(startX + txx0, y - y0 + txy0)));
    vvCycle.back().data.emplace_back(CyclePoint(&vvDrawAbleCycle.back().back(), 0x0, nullptr, nullptr));
    pPrev = &vvCycle.back().data.back();
    x -= incXY;
    for(size_t i = 1ull; i < K; ++i)
    {
        y = -sqrt(SqrR - pow(x - x0, 2.)) + y0;
        vvDrawAbleCycle.back().emplace_back(sf::Vertex(sf::Vector2f(x, y), defColor, sf::Vector2f(x - x0 + txx0, y - y0 + txy0)));
        vvCycle.back().data.emplace_back(CyclePoint(&vvDrawAbleCycle.back().back(), 0x0, pPrev, nullptr));
        pPrev->next = &vvCycle.back().data.back();
        pPrev = &vvCycle.back().data.back();
        x -= incXY;
    }

    ER_IF(vvCycle.back().data.size() != K,
        ::std::cout << "vvCycle.back().data.size() - " << vvCycle.back().data.size() << ::std::endl;,
        return false; )
    ER_IF(vvDrawAbleCycle.back().size() != K,
        ::std::cout << "vvDrawAbleCycle.back().size() - " << vvDrawAbleCycle.back().size() << ::std::endl;,
        return false; )

    y = -startY + y0;
    for(size_t i = 0ull; i < K; ++i)
    {
        x = -sqrt(SqrR - pow(y - y0, 2.)) + x0;
        vvDrawAbleCycle.back().emplace_back(sf::Vertex(sf::Vector2f(x, y), defColor, sf::Vector2f(x - x0 + txx0, y - y0 + txy0)));
        vvCycle.back().data.emplace_back(CyclePoint(&vvDrawAbleCycle.back().back(), 0x0, pPrev, nullptr));
        pPrev->next = &vvCycle.back().data.back();
        pPrev = &vvCycle.back().data.back();
        y += incXY;
    }

    ER_IF(vvCycle.back().data.size() != 2 * K,
        ::std::cout << "vvCycle.back().data.size() - " << vvCycle.back().data.size() << ::std::endl;,
        return false; )
    ER_IF(vvDrawAbleCycle.back().size() != 2 * K,
        ::std::cout << "vvDrawAbleCycle.back().size() - " << vvDrawAbleCycle.back().size() << ::std::endl;,
        return false; )

    x = -startX + x0;
    for(size_t i = 0ull; i < K; ++i)
    {
        y = sqrt(SqrR - pow(x - x0, 2.)) + y0;
        vvDrawAbleCycle.back().emplace_back(sf::Vertex(sf::Vector2f(x, y), defColor, sf::Vector2f(x - x0 + txx0, y - y0 + txy0)));
        vvCycle.back().data.emplace_back(CyclePoint(&vvDrawAbleCycle.back().back(), 0x0, pPrev, nullptr));
        pPrev->next = &vvCycle.back().data.back();
        pPrev = &vvCycle.back().data.back();
        x += incXY;
    }

    ER_IF(vvCycle.back().data.size() != 3 * K,
        ::std::cout << "vvCycle.back().data.size() - " << vvCycle.back().data.size() << ::std::endl;,
        return false; )
    ER_IF(vvDrawAbleCycle.back().size() != 3 * K,
        ::std::cout << "vvDrawAbleCycle.back().size() - " << vvDrawAbleCycle.back().size() << ::std::endl;,
        return false; )

    y = startY + y0;
    for(size_t i = 0ull; i < K; ++i)
    {
        x = sqrt(SqrR - pow(y - y0, 2.)) + x0;
        vvDrawAbleCycle.back().emplace_back(sf::Vertex(sf::Vector2f(x, y), defColor, sf::Vector2f(x - x0 + txx0, y - y0 + txy0)));
        vvCycle.back().data.emplace_back(CyclePoint(&vvDrawAbleCycle.back().back(), 0x0, pPrev, nullptr));
        pPrev->next = &vvCycle.back().data.back();
        pPrev = &vvCycle.back().data.back();
        y -= incXY;
    }
    pPrev->next = &vvCycle.back().data.front();
    vvCycle.back().data.front().prev = pPrev;

    ER_IF(vvCycle.back().data.size() != KF_MULT * K,
        ::std::cout << "vvCycle.back().data.size() - " << vvCycle.back().data.size() << ::std::endl;,
        return false; )
    ER_IF(vvDrawAbleCycle.back().size() != KF_MULT * K,
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
        for(size_t j = 0ull; j < vvCycle[i].data.size(); ++j)
        {
            auto x = vvCycle[i].data[j];
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
        for(size_t j = 0ull; j < vvCycle[i].data.size(); ++j)
        {
            auto x = vvCycle[i].data[j];
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
        auto IterCount = U % (vvCycle[j].KF_Cycle * KF_MULT);
        //::std::cout << IterCount << ::std::endl;
        for(decltype(Bicycle::U) i = 0; i < IterCount; ++i)
        {
            auto pFirst = &vvCycle[j].data.front();
            auto pCur = vvCycle[j].data.front().next;
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
    //::std::cout << "Bicycle::U - " << this->U << ::std::endl;
}

decltype(Bicycle::U) Bicycle::getSpeed()
{
    return U;
}

bool Bicycle::getState()
{
    return is_Inited;
}

CycleMachine::CycleMachine(size_t countCycle, float r, float deltr, float x0, float y0, size_t kf, sf::Color&& clr, float tx0 = 0.f, float ty0 = 0.f)
{
    ER_IF(countCycle <= 0, ::std::cout << "countCycle <= 0" << ::std::endl;, )

	this->mainBicycle.vvCycle.reserve(countCycle);
	for(size_t i = 0ul; i < countCycle; ++i)
	{
		ER_IFN(mainBicycle.initCycle2Back(r + deltr * i,
			x0, y0, kf, ::std::forward<decltype(clr)>(clr), tx0, ty0),, )
	}

    vpTx.reserve(vpTx.capacity() + 1);
    vpTx.emplace_back(nullptr);
    ER_IF(vpTx.capacity() != 1, ::std::cout << "vpTx.capacity() != 1" << ::std::endl;, )
    ER_IF(vpTx.size() != 1, ::std::cout << "vpTx.size() != 1" << ::std::endl;, )
    ER_IF(vpTx.back() != nullptr, ::std::cout << "vpTx.size(vpTx.back() != nullptr) != 1" << ::std::endl;, )
}

CycleMachine::CycleMachine(size_t countCycle, float r, float deltr, float x0, float y0, size_t kf, const sf::Color& clr, float tx0 = 0.f, float ty0 = 0.f)
{
    ER_IF(countCycle <= 0, ::std::cout << "countCycle <= 0" << ::std::endl;, )

	this->mainBicycle.vvCycle.reserve(countCycle);
	for(size_t i = 0ul; i < countCycle; ++i)
	{
		ER_IFN(mainBicycle.initCycle2Back(r + deltr * i,
			x0, y0, kf, clr, tx0, ty0),, )
	}
    
    vpTx.reserve(vpTx.capacity() + 1);
    vpTx.emplace_back(nullptr);
    ER_IF(vpTx.capacity() != 1, ::std::cout << "vpTx.capacity() != 1" << ::std::endl;, )
    ER_IF(vpTx.size() != 1, ::std::cout << "vpTx.size() != 1" << ::std::endl;, )
    ER_IF(vpTx.back() != nullptr, ::std::cout << "vpTx.size(vpTx.back() != nullptr) != 1" << ::std::endl;, )
}

CycleMachine::~CycleMachine()
{
    vpTx.clear();
}

bool CycleMachine::addTexture(const char* Filename)
{
    sf::Texture* temp = mainBicycle.addTx(Filename);
    if (temp != nullptr)
    {
        vpTx.reserve(vpTx.capacity() + 1);
        vpTx.emplace_back(temp);
    }
    else
    {
        return false;
    }
    return true;
}

bool CycleMachine::AddColor2CyclePart(size_t idx1, size_t idx2, size_t kfCycle, size_t clrCount, const sf::Color& clr, GenerateStates states = GenerateStates::SOLO,
    dxCRYPT* hProv = nullptr)
{
    ER_IFN(mainBicycle.getState(), ::std::cout << "mainBicycle is not initialized!" << ::std::endl;, return false; )
    ER_IF(idx1 < 0 || idx1 >= mainBicycle.vvCycle.size(), ::std::cout << "idx1 < 0 || idx1 >= mainBicycle.vvCycle.size()" << ::std::endl;, return false; )
    ER_IF(idx2 < 0 || idx2 >= mainBicycle.vvCycle.size(), ::std::cout << "idx2 < 0 || idx2 >= mainBicycle.vvCycle.size()" << ::std::endl;, return false; )
    ER_IF(idx1 >= idx2, ::std::cout << "idx1 >= idx2" << ::std::endl;, return false; )
    ER_IF(kfCycle <= 0, ::std::cout << "kfCycle <= 0" << ::std::endl;, return false; )
    ER_IF(clrCount <= 0, ::std::cout << "clrCount <= 0" << ::std::endl;, return false; )
    ER_IF(kfCycle * KF_MULT < clrCount, ::std::cout << "kfCycle < clrCount" << ::std::endl;, return false; )

	for(size_t idx = idx1; idx < idx2; ++idx)
	{
		if (idx == 0ul)
		{
            if (states == GenerateStates::SOLO)
            {
                auto pCur = &mainBicycle.vvCycle[idx].data.front();
                ER_IF(pCur == nullptr,, )
                for(size_t i = 0ul; i < clrCount; ++i)
                {
                    ER_IF(pCur == nullptr,, )
                    pCur->setColor(clr);
                    pCur->setOptions(SETBITS(SETBITS(0x0, 0x1, bmdx::CyclePoint::OPTIONS::DRAW), 0x1, bmdx::CyclePoint::OPTIONS::DRAWVAL));
                    pCur = pCur->next;
                }
            }
            /*else if (states == GenerateStates::RANDOM)
            {
                ER_IF(hProv == nullptr, ::std::cout << "hProv == nullptr" << ::std::endl;, return false; )

                auto pCur = &mainBicycle.vvCycle[idx].data.front();
                ER_IF(pCur == nullptr,, )
                auto pFirst = pCur;

                size_t countFilled = 0ul;
                auto Xcondition = kfCycle * KF_MULT / clrCount;
                while(pCur != pFirst)
                {
                    ER_IF(pCur == nullptr,, )
                    if (countFilled < clrCount)
                    {
                        if (nndx::randB(*hProv) % Xcondition)
                        {
                            pCur->setColor(clr);
                            pCur->setOptions(SETBITS(SETBITS(0x0, 0x1, bmdx::CyclePoint::OPTIONS::DRAW), 0x1, bmdx::CyclePoint::OPTIONS::DRAWVAL));
                        }
                        pCur = pCur->next;
                        ++countFilled;
                    }
                    else
                    {
                        break;
                    }
                    
                }
            }*/
            else
            {
                auto pCur = &mainBicycle.vvCycle[idx].data.front();
                ER_IF(pCur == nullptr,, )
                auto pFirst = pCur;

                int fFill = static_cast<unsigned int>(kfCycle * KF_MULT);
                auto Xcondition = fFill / states;
                do
                {
                    ER_IF(pCur == nullptr,, )
                    if (fFill > 0)
                    {
                        if ((fFill % Xcondition) > (Xcondition - clrCount))
                        {
                            pCur->setColor(clr);
                            pCur->setOptions(SETBITS(SETBITS(0x0, 0x1, bmdx::CyclePoint::OPTIONS::DRAW), 0x1, bmdx::CyclePoint::OPTIONS::DRAWVAL));
                        }
                        pCur = pCur->next;
                        --fFill;
                    }
                    else
                    {
                        break;
                    }
                    
                } while(pCur != pFirst);
            }
        }
    }

    return true;
}

bool CycleMachine::DefaultCopyBetweenCycles(size_t rdx, size_t tdx1, size_t tdx2)
{
    ER_IF(rdx < 0 || rdx >= mainBicycle.vvCycle.size(), ::std::cout << "rdx < 0 || rdx >= mainBicycle.vvCycle.size()" << ::std::endl;, return false; )
    ER_IF(tdx1 < 0 || tdx1 >= mainBicycle.vvCycle.size(), ::std::cout << "tdx1 < 0 || tdx1 >= mainBicycle.vvCycle.size()" << ::std::endl;, return false; )
    ER_IF(tdx2 < 0 || tdx2 >= mainBicycle.vvCycle.size(), ::std::cout << "tdx2 < 0 || tdx2 >= mainBicycle.vvCycle.size()" << ::std::endl;, return false; )
    ER_IF(tdx1 >= tdx2, ::std::cout << "tdx1 >= tdx2" << ::std::endl;, return false; )

   for(size_t i = tdx1; i < tdx2; ++i)
    {
        if (i != rdx)
        {
            for(size_t j = 0ul; j < mainBicycle.vvCycle[i].data.size(); ++j)
            {
                mainBicycle.vvCycle[i].data[j].setColor(mainBicycle.vvCycle[rdx].data[j].getColor());
                mainBicycle.vvCycle[i].data[j].setOptions(mainBicycle.vvCycle[rdx].data[j].getOptions());
            }
        }
    }

    return true;
}

bool CycleMachine::mA(unsigned short speed, unsigned short dependedSpeedOfCycles, bool nativeSpeed)
{
    ER_IFN(mainBicycle.is_Inited, ::std::cout << "mainBicycle.is_Inited - " << mainBicycle.is_Inited << ::std::endl;, return false; )

    if (nativeSpeed)    mainBicycle.setSpeed(speed);
    else    mainBicycle.setSpeed(dependedSpeedOfCycles);

    for(size_t j = 0ull; j < mainBicycle.vvCycle.size(); ++j)
    {
        auto IterCount = mainBicycle.U % (mainBicycle.vvCycle[j].KF_Cycle * KF_MULT);
        //::std::cout << IterCount << ::std::endl;
        for(decltype(Bicycle::U) i = 0; i < IterCount; ++i)
        {
            auto pFirst = &mainBicycle.vvCycle[j].data.front();
            auto pCur = mainBicycle.vvCycle[j].data.front().next;
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

bool CycleMachine::drawCycles(sf::RenderWindow& win, bool func(bmdx::CyclePoint*), size_t textureIdx)
{
    ER_IFN(mainBicycle.drawBicycle(win, func, vpTx[textureIdx]),, return false; )

    return true;
}

size_t CycleMachine::getCycleSize()
{
    return mainBicycle.vvCycle.size();
}

}