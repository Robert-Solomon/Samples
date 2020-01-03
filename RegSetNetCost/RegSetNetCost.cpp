// RegSetNetCost.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <Windows.h>
#include <winnt.h>
#include <stdio.h>
#include <strsafe.h>

#define MYDBG 1

#if MYDBG
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
	for (unsigned i = 0; i < ptp->PrivilegeCount; i++)
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
#endif


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

BOOL AddRequiredPrivileges()
{
	HANDLE procTok = NULL;

#if MYDBG
	printf("[V] ---------- Original Token Info -----------\n");
	DebugTokenInfo();
#endif

	if (!OpenProcessToken(
		GetCurrentProcess(),
		TOKEN_ALL_ACCESS,
		&procTok))
	{
		printf("OpenProcessToken error: %u\n", GetLastError());
		return FALSE;
	}

	SetPrivilege(procTok, SE_BACKUP_NAME, TRUE);
	SetPrivilege(procTok, SE_RESTORE_NAME, TRUE);
	SetPrivilege(procTok, SE_TAKE_OWNERSHIP_NAME, TRUE);

#if MYDBG
	printf("[V] ---------- Modified Token Info -----------\n");
	DebugTokenInfo();
#endif

	if (procTok)
		CloseHandle(procTok);

	return TRUE;
}

BOOL GrabKeyOwnership(HKEY hKey)
{
	BOOL bRet = TRUE;

	// allocate and initialize a security descriptor
	PISECURITY_DESCRIPTOR pSD = (PISECURITY_DESCRIPTOR)LocalAlloc(0, sizeof(SECURITY_DESCRIPTOR));
	SID sid;
	DWORD SizeT;
	LSTATUS lStatus;

	if (pSD == NULL)
	{
		printf("LocalAlloc failed\n");
		return FALSE;
	}

	if (!InitializeSecurityDescriptor(pSD, SECURITY_DESCRIPTOR_REVISION))
	{
		printf("InitializeSecurityDescriptor error: %u\n", GetLastError());
		bRet = FALSE;
		goto Cleanup;
	}

	SizeT = sizeof(sid);
	if (!CreateWellKnownSid(WinLocalSystemSid, NULL, &sid, &SizeT))
	{
		printf("CreateWellKnownSid error: %u\n", GetLastError());
		bRet = FALSE;
		goto Cleanup;
	}

	if (!SetSecurityDescriptorOwner(pSD, &sid, FALSE))
	{
		printf("SetSecurityDescriptorOwner error: %u\n", GetLastError());
		bRet = FALSE;
		goto Cleanup;
	}

	lStatus = RegSetKeySecurity(
		hKey,
		OWNER_SECURITY_INFORMATION,
		pSD);

	if (lStatus != ERROR_SUCCESS)
	{
		printf("RegSetKeySecurity error: %u\n", lStatus);
		bRet = FALSE;
		goto Cleanup;
	}

Cleanup:
	if (pSD != NULL)
		LocalFree(pSD);

	return bRet;
}

BOOL FixCostRegKeys()
{
	BOOL bRet = TRUE;
	LSTATUS lStatus;
	HKEY hRegKey = NULL;
	LPCWSTR sRegKey = TEXT("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\NetworkList\\DefaultMediaCost");

	if (!AddRequiredPrivileges())
		return FALSE;

	// open reg key

	lStatus = RegOpenKeyEx(
		HKEY_LOCAL_MACHINE,
		sRegKey,
		0,
		WRITE_OWNER | READ_CONTROL | WRITE_DAC,
		&hRegKey);

	if (lStatus != ERROR_SUCCESS)
	{
		printf("RegOpenKeyEx error: %u\n", lStatus);
		bRet = FALSE;
		goto Cleanup;
	}

	if (!GrabKeyOwnership(hRegKey))
	{
		bRet = FALSE;
		goto Cleanup;
	}


Cleanup:
	if (hRegKey != NULL)
		RegCloseKey(hRegKey);

	return TRUE;
}

int main()
{
	FixCostRegKeys();

	return 0;
}