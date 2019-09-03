#pragma once
#include "stdafx.h"

using namespace std;

#define SystemHandleInformation 16
#define STATUS_INFO_LENGTH_MISMATCH 0xc0000004

const char processName[] = "Amber.exe";

typedef struct _SYSTEM_HANDLE_TABLE_ENTRY_INFO
{
     USHORT UniqueProcessId;
     USHORT CreatorBackTraceIndex;
     UCHAR ObjectTypeIndex;
     UCHAR HandleAttributes;
     USHORT HandleValue;
     PVOID Object;
     ULONG GrantedAccess;
} SYSTEM_HANDLE_TABLE_ENTRY_INFO, *PSYSTEM_HANDLE_TABLE_ENTRY_INFO;

typedef struct _SYSTEM_HANDLE_INFORMATION
{
	ULONG NumberOfHandles;
	SYSTEM_HANDLE_TABLE_ENTRY_INFO Handles[1];
} SYSTEM_HANDLE_INFORMATION, *PSYSTEM_HANDLE_INFORMATION;

typedef NTSTATUS(NTAPI *_NtQuerySystemInformation)(
	ULONG SystemInformationClass,
	PVOID SystemInformation,
	ULONG SystemInformationLength,
	PULONG ReturnLength
	);

typedef _NtQuerySystemInformation NtQuerySystemInformationFn;

HANDLE GetProcessHandle(DWORD pID);
DWORD GetProcessIdByName(LPCSTR proc);
DWORD WINAPI Main(LPVOID);
DWORD SimpleMessageBox(LPCSTR, LPCSTR);