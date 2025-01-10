#pragma once

#include <Windows.h>
#include <RakHook/rakhook.hpp>

#include "minhookwrapper.hpp"

class CPlugin
{
public:
    CPlugin(HMODULE module);
    ~CPlugin();

    static void GameLoop();
    static CHook<void(*)()> GameLoopHook;

private:
    HMODULE module_;
};
inline CHook<void(*)()> CPlugin::GameLoopHook = { 0x561B10 };