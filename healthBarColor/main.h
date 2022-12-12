#ifndef _MAIN_H_
#define _MAIN_H_

#include <windows.h>
#include <memory>
#include "memwrapper/memwrapper.h"

typedef struct { float x, y, z; } CVector;

using CSprite2d__DrawBarChart = void(__cdecl*)(float, float, unsigned short, unsigned char, float, signed char, unsigned char, unsigned char, unsigned long, unsigned long);
std::unique_ptr<memwrapper::memhook<CSprite2d__DrawBarChart>> DrawBarHook;

using GameloopPrototype = void(__cdecl*)();
std::unique_ptr<memwrapper::memhook<GameloopPrototype>> GameloopHook;

using CPlayerTags__Prototype = void(__thiscall*)(void*, CVector*, float, float, float);
std::unique_ptr<memwrapper::memhook<CPlayerTags__Prototype>> PlayerTags__HealthHook;

class AsiPlugin
{
    
public:
    explicit AsiPlugin();
    virtual ~AsiPlugin();
} AsiPlugin;

#endif