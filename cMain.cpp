#include "Headers.h"

extern cAddress Address;

void Init()
{
    cESP::Initialize();
    DirectXHook::Initialize();
}

DWORD WINAPI DllMain(HINSTANCE hModule, DWORD _Reason, LPVOID _Reserved)
{
    if (_Reason == DLL_PROCESS_ATTACH)
    {
        DisableThreadLibraryCalls(hModule);

        CreateThread(0, 0, (LPTHREAD_START_ROUTINE)Init, 0, 0, 0);
        return true;
    }
    if (_Reason == DLL_PROCESS_DETACH)
    {
    }

    return false;
}
