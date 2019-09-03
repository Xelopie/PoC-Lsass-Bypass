#include "stdafx.h"
#include "bypass.h"

#pragma comment(lib, "Wtsapi32.lib")

using namespace std;

DWORD WINAPI SimpleMessageBox(LPCSTR msg, LPCSTR title) {
	DWORD response;

	WTSSendMessageA(WTS_CURRENT_SERVER_HANDLE,       // hServer
		WTSGetActiveConsoleSessionId(),  // ID for the console seesion (1)
		const_cast<LPSTR>(title),        // MessageBox Caption
		strlen(title),                   // 
		const_cast<LPSTR>(msg),         // MessageBox Text
		strlen(msg),                    // 
		MB_OK,                           // Buttons, etc
		0,                              // Timeout period in seconds
		&response,                       // What button was clicked (if bWait == TRUE)
		TRUE);                          // bWait - Blocks until user click

	return response;
}

HANDLE GetProcessHandle(DWORD pId) {
	auto NtQuerySystemInformation = reinterpret_cast<NtQuerySystemInformationFn>(GetProcAddress(GetModuleHandleA("ntdll.dll"), "NtQuerySystemInformation"));
	NTSTATUS status;
	ULONG infoSize = 0x10000;

	auto info = reinterpret_cast<PSYSTEM_HANDLE_INFORMATION>(malloc(infoSize));

	while ((status = NtQuerySystemInformation(SystemHandleInformation, info, infoSize, nullptr)) == STATUS_INFO_LENGTH_MISMATCH) {
		info = reinterpret_cast<PSYSTEM_HANDLE_INFORMATION>(realloc(info, infoSize *= 2));
	}
	
	if (!NT_SUCCESS(status)) {
		// MessageBox here
		SimpleMessageBox("Error: NtQuerySystemInformation failed", "GetProcessHandle");
	}

	for (UINT32 i = 0; i < info->NumberOfHandles; i++) {
		auto infoTable = info->Handles[i];
		HANDLE procHandle = reinterpret_cast<HANDLE>(infoTable.HandleValue);

		if (infoTable.UniqueProcessId == GetCurrentProcessId() && GetProcessId(procHandle) == pId) {
			return procHandle;
		}
	}

	free(info);
	return nullptr;
}

DWORD GetProcessIdByName(LPCSTR proc) {
	auto snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	PROCESSENTRY32 pe32;
	pe32.dwSize = sizeof(pe32);

	if (Process32First(snapshot, &pe32)) {
		do {
			if (!strcmp(pe32.szExeFile, proc)) {
				CloseHandle(snapshot);
				return pe32.th32ProcessID;
			}
		} while (Process32Next(snapshot, &pe32));
	}
	CloseHandle(snapshot);
	return NULL;
}

DWORD Main(LPVOID) {
	stringstream ss;

	DWORD pId = GetProcessIdByName(processName);
	ss << "Target Process ID: " << pId << endl;
	SimpleMessageBox(ss.str().c_str(), "Main");
	ss.str("");

	HANDLE handle = GetProcessHandle(pId);
	ss << "Process Handle: " << (uintptr_t)handle << endl;
	SimpleMessageBox(ss.str().c_str(), "Main");
	ss.str("");

	//int value = 456456;
	//WriteProcessMemory(handle, (LPVOID)0xD7714FF774, &value, sizeof(int), NULL);
	return 0;
}