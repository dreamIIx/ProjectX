#pragma once

#include <iostream>
#include <vector>
#include <map>
#include <cmath>

#include "SFML/Graphics.hpp"

#define defDX_S(x)		#x
#define defDX_S_(x)		defDX_S(x)
#define defDX_S__LINE__	defDX_S_(__LINE__)
#define defDX__FILELINE		(__FILE__  " line " defDX_S__LINE__)
/*
#define ERROR_				::std::cout << "Error - " << defDX__FILELINE << ::std::endl; \
							system("pause");
*/
#ifndef ERROR_

#if defined(_WIN32)
#define ERROR_				throw ::std::exception((const char*)defDX_S_(__LINE__));
#elif defined(__unix__)
#if defined(__linux__)
#define ERROR_				::std::cout << (const char*)defDX__FILELINE << ::std::endl; \
                            throw ::std::exception(); // ((const char*)defDX_S_(__LINE__))
#else
#error This UNIX operating system is not supported by dx::NN
#endif
#else
#error This operating system is not supported by dx::NN
#endif

#endif
#ifndef ER_IF
//#define ER_IF(x) if ( (x) ) { ERROR_ }
#define ER_IF(x, beforeExc, AfterExc) if ( (x) ) { beforeExc ERROR_ AfterExc }
#endif
#ifndef ER_IFN
//#define ER_IFN(x) if ( !(x) ) { ERROR_ }
#define ER_IFN(x, beforeExc, AfterExc) if ( !(x) ) { beforeExc ERROR_ AfterExc }
#endif

#if defined(_WIN32)
	#include <Windows.h>
	typedef HCRYPTPROV dxCRYPT;
	typedef unsigned long int dxFastInt32;
#elif defined(__unix__)
    #if defined(__linux__)
		#include <random>
		#include <chrono>
		typedef ::std::mt19937 dxCRYPT;
		typedef uint_fast32_t dxFastInt32;
		#define	def_FILEROOT "/run/media/dream11x/dreamIIx/programming/C++/Project2/x64/Debug/"
    #else
        #error This UNIX operating system is not supported by dx::NN
    #endif
#else
    #error This operating system is not supported by dx::NN
#endif

#define ISBIT(x,pos)            ( ( (x) & ( 0x1 << (pos) ) ) != 0 )
#define GETBIT(x,pos)           ( (x) & ( 0x1 << (pos) ) )
#define GETBITS(x,y,pos)        ( (x) & ( y << (pos) ) )
#define SETBIT(x,pos)           ( (x) |= ( 0x1 << (pos) ) )
#define UNSETBIT(x,pos)         ( (x) &= (~( 0x1 << (pos) ) ) )
#define SETBITS(x,y,pos)        ( (x) |= ( y << (pos) ) )
#define UNSETBITS(x,y,pos)      ( (x) &= (~( y << (pos) ) ) )

namespace nndx
{
	dxFastInt32 randT(dxCRYPT&);
	dxFastInt32 randB(dxCRYPT&);
}

namespace bmdx
{

typedef float coord;

class CyclePoint
{
    friend class Bicycle;
private:
    sf::Vertex* pPoint;
    ///
    // ...|     2    |   1  |  5  ||
    // ...| reserved | flag | idx ||
    ///
    int options;

public:
    CyclePoint* prev;
    CyclePoint* next;

    CyclePoint(sf::Vertex* rfPoint, int opt, CyclePoint* prev, CyclePoint* next);
    ~CyclePoint();

    sf::Color getColor();
    void setColor(sf::Color Color);
    void setOptions(decltype(options) options);
    decltype(options) getOptions();
};

class Bicycle
{
private:
    std::vector<sf::Vertex> DrawAbleCycle1;
    bool is_Inited;

public:
    ::std::vector<CyclePoint> vCycle;

    Bicycle();    
    ~Bicycle();

    bool initCycle(float CycleRadius, float x0, float y0, size_t halfCount, sf::Color defaultColor/*, ::std::map<size_t, CyclePoint> specDefs = {}*/);
    bool drawBicycle(sf::RenderWindow& RenderWindow);

    bool getState();
};

}