#include "bicycle_machine.h"

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

void CyclePoint::setOptions(decltype(CyclePoint::options) aOpt)
{
    this->options = aOpt;
}

decltype(CyclePoint::options) CyclePoint::getOptions()
{
    return options;
}

Bicycle::Bicycle() : is_Inited(false)
{

}

Bicycle::~Bicycle()
{
    
}

bool Bicycle::initCycle(float r, float x0, float y0, size_t K, sf::Color defColor/*, ::std::map<size_t, CyclePoint> specDefs = {}*/)
{
    ER_IF(r <= 0,, return false; )
    ER_IF(K <= 0,, return false; )

    coord y = y0;
    coord x = -r + x0;
    coord incX = (2 * r) / K;
    double SqrR = pow(r, 2.);
    CyclePoint* pPrev = nullptr;
    vCycle.reserve(K);
    DrawAbleCycle1.reserve(K);
    
    DrawAbleCycle1.emplace_back(sf::Vertex(sf::Vector2f(x, y), defColor));
    vCycle.emplace_back(CyclePoint(&DrawAbleCycle1.back(), 0x0, nullptr, nullptr));
    pPrev = &vCycle.back();
    x += incX;
    for(size_t i = 1ull; i < K; ++i)
    {
        y = sqrt(SqrR - pow(x - x0, 2.)) + y0;
        DrawAbleCycle1.emplace_back(sf::Vertex(sf::Vector2f(x, y), defColor));
        vCycle.emplace_back(CyclePoint(&DrawAbleCycle1.back(), 0x0, pPrev, nullptr));
        pPrev->next = &vCycle.back();
        pPrev = &vCycle.back();
        x += incX;
    }
    ER_IF(vCycle.size() != K,
        ::std::cout << "vCycle.size() - " << vCycle.size() << ::std::endl;,
        return false; )
    ER_IF(DrawAbleCycle1.size() != K,
        ::std::cout << "DrawAbleCycle1.size() - " << DrawAbleCycle1.size() << ::std::endl;,
        return false; )
    x = r + x0;
    y = y0;
    DrawAbleCycle1.emplace_back(sf::Vertex(sf::Vector2f(x, y), defColor));
    vCycle.emplace_back(CyclePoint(&DrawAbleCycle1.back(), 0x0, pPrev, nullptr));
    pPrev->next = &vCycle.back();
    pPrev = &vCycle.back();

    for(size_t i = 1ull; i < K; ++i)
    {
        y = -sqrt(SqrR - pow(x - x0, 2.)) + y0;
        DrawAbleCycle1.emplace_back(sf::Vertex(sf::Vector2f(x, y), defColor));
        vCycle.emplace_back(CyclePoint(&DrawAbleCycle1.back(), 0x0, pPrev, nullptr));
        pPrev->next = &vCycle.back();
        pPrev = &vCycle.back();
        x -= incX;
    }
    vCycle.front().prev = pPrev;

    ER_IF(vCycle.capacity() < vCycle.size(),
        ::std::cout << "vCycle.capacity() - " << vCycle.capacity() << " vCycle.size() - " << vCycle.size() << ::std::endl;,
        return false; )
    ER_IF(DrawAbleCycle1.capacity() < DrawAbleCycle1.size(),
        ::std::cout << "DrawAbleCycle1.capacity() - " << DrawAbleCycle1.capacity() << " DrawAbleCycle1.size() - " << DrawAbleCycle1.size() << ::std::endl;,
        return false; )
    ER_IF(vCycle.size() != 2 * K,
        ::std::cout << "vCycle.size() - " << vCycle.size() << " K - " << K << ::std::endl;,
        return false; )
    ER_IF(DrawAbleCycle1.size() != 2 * K,
        ::std::cout << "DrawAbleCycle1.size() - " << DrawAbleCycle1.size() << " K - " << K << ::std::endl;,
        return false; )

    is_Inited = true;
    return true;
}

bool Bicycle::drawBicycle(sf::RenderWindow& win)
{
    ER_IFN(win.isOpen(),, return false; )
    ER_IFN(is_Inited,, return false; )

    win.draw(&DrawAbleCycle1[0], DrawAbleCycle1.size(), sf::Points);
    /*
    for(size_t i = 0ull; i < DrawAbleCycle1.size(); ++i)
    {
        win.draw(&DrawAbleCycle1[i], 1, sf::Points);
    }
    */
    return true;
}

bool Bicycle::getState()
{
    return is_Inited;
}

}