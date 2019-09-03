// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
#include "bypass.h"

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
	HANDLE mainThread = NULL;
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
		mainThread = CreateThread(0, 0, Main, 0, 0, 0);
		break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
		TerminateThread(mainThread, NULL);
		CloseHandle(mainThread);
        break;
    }
    return TRUE;
}

