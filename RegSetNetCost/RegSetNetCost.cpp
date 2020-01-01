// RegSetNetCost.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <Windows.h>
#include <winnt.h>
#include <stdio.h>
#include <strsafe.h>

// this function borrowed from MSDN
BOOL SetPrivilege(
	HANDLE hToken,          // access token handle
	LPCTSTR lpszPrivilege,  // name of privilege to enable/disable
	BOOL bEnablePrivilege   // to enable or disable privilege
)
{
	TOKEN_PRIVILEGES tp;
	LUID luid;

	if (!LookupPrivilegeValue(
		NULL,            // lookup privilege on local system
		lpszPrivilege,   // privilege to lookup 
		&luid))        // receives LUID of privilege
	{
		printf("LookupPrivilegeValue error: %u\n", GetLastError());
		return FALSE;
	}

	tp.PrivilegeCount = 1;
	tp.Privileges[0].Luid = luid;
	if (bEnablePrivilege)
		tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	else
		tp.Privileges[0].Attributes = 0;

	// Enable the privilege or disable all privileges.

	if (!AdjustTokenPrivileges(
		hToken,
		FALSE,
		&tp,
		sizeof(TOKEN_PRIVILEGES),
		(PTOKEN_PRIVILEGES)NULL,
		(PDWORD)NULL))
	{
		printf("AdjustTokenPrivileges error: %u\n", GetLastError());
		return FALSE;
	}

	if (GetLastError() == ERROR_NOT_ALL_ASSIGNED)
	{
		printf("The token does not have the specified privilege. \n");
		return FALSE;
	}

	return TRUE;
}

BOOL DebugTokenInfo()
{
	static BYTE DebugInfoBuf[4096];
	const int StrBufLen = 128;
	WCHAR StrBuf[StrBufLen];
	DWORD RetLen;

	HANDLE procTok = NULL;

	if (!OpenProcessToken(
		GetCurrentProcess(),
		TOKEN_ALL_ACCESS,
		&procTok))
	{
		printf("OpenProcessToken error: %u\n", GetLastError());
		return FALSE;
	}

	if (!GetTokenInformation(
		procTok,
		TokenPrivileges,
		&DebugInfoBuf,
		sizeof(DebugInfoBuf),
		&RetLen))
	{
		printf("GetTokenInformation(Privileges) error: %u\n", GetLastError());
			return FALSE;
	}
	PTOKEN_PRIVILEGES ptp = (PTOKEN_PRIVILEGES)&DebugInfoBuf;
	printf("[V] Token Info: Privilegess. size: %d, count: %d\n", RetLen, ptp->PrivilegeCount);
	for (int i = 0; i < ptp->PrivilegeCount; i++)
	{
		RetLen = StrBufLen;
		if (!LookupPrivilegeName(NULL, &(ptp->Privileges[i].Luid), StrBuf, &RetLen))
			StringCchCopy(StrBuf, StrBufLen, L"PrivilegeName not found");

		printf("[V]     %ws: 0x%08X\n", StrBuf, ptp->Privileges[i].Attributes);
	}



	if (procTok)
		CloseHandle(procTok);

	return TRUE;
}
BOOL FixCostRegKeys()
{
	HANDLE procTok = NULL;

	if (!OpenProcessToken(
		GetCurrentProcess(),
		TOKEN_ALL_ACCESS,
		&procTok))
	{
		printf("OpenProcessToken error: %u\n", GetLastError());
		return FALSE;
	}

	// SetPrivilege(procTok, SE_IMPERSONATE_NAME, TRUE);
	SetPrivilege(procTok, SE_BACKUP_NAME, TRUE);
	SetPrivilege(procTok, SE_RESTORE_NAME, TRUE);
	SetPrivilege(procTok, SE_TAKE_OWNERSHIP_NAME, TRUE);

	if (procTok)
		CloseHandle(procTok);

	return TRUE;
}

int main()
{
	DebugTokenInfo();
	FixCostRegKeys();
	return 0;
}