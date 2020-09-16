#pragma once

#include <iostream>
#include <vector>
#include <atomic>
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
#define EQ_SETBIT(x,pos)        ( (x) |= ( 0x1 << (pos) ) )
#define EQ_UNSETBIT(x,pos)      ( (x) &= (~( 0x1 << (pos) ) ) )
#define EQ_SETBITS(x,y,pos)     ( (x) |= ( y << (pos) ) )
#define EQ_UNSETBITS(x,y,pos)   ( (x) &= (~( y << (pos) ) ) )
#define SETBIT(x,pos)           ( (x) | ( 0x1 << (pos) ) )
#define UNSETBIT(x,pos)         ( (x) & (~( 0x1 << (pos) ) ) )
#define SETBITS(x,y,pos)        ( (x) | ( y << (pos) ) )
#define UNSETBITS(x,y,pos)      ( (x) & (~( y << (pos) ) ) )

#define KF_MULT 4

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
    // ...|  2[7..6] |  1[5]  |  5[4..0] ||
    // ...| reserved |  flag  |    idx   ||
    ///
    int options;

public:
    CyclePoint* prev;
    CyclePoint* next;

    enum OPTIONS : int
    {
        DRAWVAL = 0,
        DRAW = 5,
        KEYVAL = 8,
        KEY = 13
    };

    CyclePoint(sf::Vertex* rfPoint, int opt, CyclePoint* prev, CyclePoint* next);
    ~CyclePoint();

    sf::Color getColor();
    void setColor(const sf::Color& Color);
    void setOptions(decltype(options) options);
    decltype(options) getOptions();
};

class Bicycle
{
    friend class CycleMachine;
private:
    ::std::vector<::std::vector<sf::Vertex>> vvDrawAbleCycle;
    ::std::vector<sf::Texture> vTx;
    bool is_Inited;
    unsigned short U;

public:
    struct VectorCycle
    {
        ::std::vector<CyclePoint> data;
        size_t KF_Cycle;

        VectorCycle();
    };
    ::std::vector<VectorCycle> vvCycle;

    Bicycle();    
    ~Bicycle();

    sf::Texture* addTx(const char* Filename);
    bool initCycle2Back(float CycleRadius, float x0, float y0, size_t halfCount, sf::Color&& defaultColor,
        float TextureX0 = 0.f, float TextureY0 = 0.f);
    bool initCycle2Back(float CycleRadius, float x0, float y0, size_t halfCount, const sf::Color& defaultColor,
        float TextureX0 = 0.f, float TextureY0 = 0.f);
    bool drawBicycle(sf::RenderWindow& RenderWindow, bool func(bmdx::CyclePoint*), sf::Texture* DrawTexture = nullptr);
    bool drawBicycle(sf::RenderWindow& RenderWindow, size_t StartIdx, size_t EndIdx, bool func(bmdx::CyclePoint*), sf::Texture* DrawTexture = nullptr);
    bool mA();

    void setSpeed(decltype(U) Speed);
    decltype(U) getSpeed();
    bool getState();
};

class CycleMachine
{
private:
	bmdx::Bicycle mainBicycle;
    ::std::vector<sf::Texture*> vpTx;

public:
    enum GenerateStates : int
    {
        SOLO = 0b0000,
        //SOLO_DRAWNUM = 0b0001, // reserved
        HALFS = 0b0010,
        //HALFS_DRAWNUM = 0b0011, // reserved
        TRIANGLE = 0b0011,
        //TRIANGLE_DRAWNUM = 0b0101, // reserved
        QUADS = 0b0100,
        //QUADS_DRAWNUM = 0b1001 // reserved
        //RANDOM = 0001'0000, // don't work
        //RANDOM_DRAWNUM = 0001'0001 // reserved
    };

    CycleMachine(size_t countCycle, float Radius, float DeltRadius, float BicycleX0, float BicycleY0, size_t kf, sf::Color&& Color,
        float TextureX0 = 0.f, float TextureY0 = 0.f);
    CycleMachine(size_t countCycle, float Radius, float DeltRadius, float BicycleX0, float BicycleY0, size_t kf, const sf::Color& Color,
        float TextureX0 = 0.f, float TextureY0 = 0.f);
    ~CycleMachine();

    bool addTexture(const char* Filename);
    ///
    // for GenerateStates::RANDOM the ColorPointCount meaning the high bound of points count to fill color for,
    // but actually it is a random count ((ColorPointCount / KF_AllCyclePoints)% of KF_AllCyclePoints)
    ///
    bool AddColor2CyclePart(size_t idx1, size_t idx2, size_t KF_AllCyclePoints, size_t ColorPointCount, const sf::Color& Color, GenerateStates States = GenerateStates::SOLO,
        dxCRYPT* hProv = nullptr);
    bool AddColor2CyclePart(size_t idx1, size_t idx2, size_t KF_AllCyclePoints, size_t ColorPointCount, const sf::Color& ColorStart, const sf::Color& ColorEnd,
        GenerateStates States = GenerateStates::SOLO, dxCRYPT* hProv = nullptr);
    bool SetColor2CyclePart(size_t idx1, size_t idx2, size_t KF_AllCyclePoints, size_t ColorPointCount, const sf::Color& Color, const sf::Color& ClearColor,
        GenerateStates States = GenerateStates::SOLO, dxCRYPT* hProv = nullptr);
    bool SetColor2CyclePart(size_t idx1, size_t idx2, size_t KF_AllCyclePoints, size_t ColorPointCount, const sf::Color& ColorStart, const sf::Color& ColorEnd,
        const sf::Color& ClearColor, GenerateStates States = GenerateStates::SOLO, dxCRYPT* hProv = nullptr);
    bool SetBitCyclePoint();
    bool DefaultCopyBetweenCycles(size_t ResourceIdx, size_t TargetIdx1, size_t TargetIdx2);
    bool mA(unsigned short nativeSpeedOfCycles, unsigned short dependedSpeedOfCycles, bool SpeedState);
    bool drawCycles(sf::RenderWindow& RenderWindow, bool SelectionFunction(bmdx::CyclePoint*), size_t textureIdx = 0ul);

    size_t getCycleSize();
};

}