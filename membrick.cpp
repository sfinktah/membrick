#include "membrick.hpp"

MODULEINFO GetMainModuleInfo()
{
    MODULEINFO mi;
    GetModuleInformation(GetCurrentProcess(), GetModuleHandle(NULL), &mi, sizeof(mi));
    return mi;
}

MODULEINFO g_MainModuleInfo = GetMainModuleInfo();

memBrick Scan(const char* pattern)
{
    return memBrickSafe::scan(g_MainModuleInfo, pattern);
}

template <typename T>
T* Scan(const char* pattern)
{
    return Scan(pattern).as<T*>();
}
