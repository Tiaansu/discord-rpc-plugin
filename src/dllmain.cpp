#include <Windows.h>
#include "plugin.hpp"

std::unique_ptr<CPlugin> plugin;

BOOL APIENTRY DllMain(HMODULE module, DWORD reason, LPVOID reserverd)
{
    switch (reason)
    {
        case DLL_PROCESS_ATTACH:
            DisableThreadLibraryCalls(module);
            plugin = std::make_unique<CPlugin>(module);
            break;
        case DLL_PROCESS_DETACH:
            plugin.reset();
            break;
    }

    return TRUE;
}