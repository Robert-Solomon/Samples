// RegSetNetCost.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <Windows.h>
#include <winnt.h>
#include <stdio.h>
#include <strsafe.h>
#include <AclAPI.h>

// this function borrowed from MSDN
BOOL SetPrivilege(
	HANDLE hToken,          // access token handle
	LPCTSTR lpszPrivilege,  // name of privilege to enable/disable
	BOOL bEnablePrivilege   // to enable or disable privilege
)
{
	TOKEN_PRIVILEGES tp;
	LUID luid;

	if (ERROR_SUCCESS != LookupPrivilegeValue(
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

	if (ERROR_SUCCESS != AdjustTokenPrivileges(
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

BOOL FixCostRegKeys()
{
//	LPCWSTR sRegKey = L"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\NetworkList\\DefaultMediaCost";
	LPCWSTR sRegKey = L"MACHINE<i>SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\NetworkList\\MyTestKey";

	// start by adding required privileges
	HANDLE procTok = NULL;
	if (ERROR_SUCCESS != OpenProcessToken(
		GetCurrentProcess(),
		TOKEN_ADJUST_PRIVILEGES,
		&procTok))
	{
		printf("OpenProcessToken error: %u\n", GetLastError());
		return FALSE;
	}

	SetPrivilege(procTok, SE_BACKUP_NAME, TRUE);
	SetPrivilege(procTok, SE_RESTORE_NAME, TRUE);
	SetPrivilege(procTok, SE_TAKE_OWNERSHIP_NAME, TRUE);
	SetPrivilege(procTok, SE_SECURITY_NAME, TRUE);

	BOOL bRet = TRUE;

	PSID pSidOwner = NULL;
	PSID pSidEveryone = NULL;
	SID_IDENTIFIER_AUTHORITY authority;
	PACL pDACL = NULL;

	authority = SECURITY_NT_AUTHORITY;
	if (ERROR_SUCCESS != AllocateAndInitializeSid(
		&authority,
		2, // nSubAuthorityCount
		SECURITY_BUILTIN_DOMAIN_RID, DOMAIN_ALIAS_RID_ADMINS, 0, 0, 0, 0, 0, 0,
		&pSidOwner))
	{
		printf("AllocateAndInitializeSid(Owner) error: %u\n", GetLastError());
		bRet = FALSE;
		goto Cleanup;
	}

	authority = SECURITY_WORLD_SID_AUTHORITY;
	if (ERROR_SUCCESS != AllocateAndInitializeSid(
		&authority,
		1, // nSubAuthorityCount
		SECURITY_WORLD_RID, 0, 0, 0, 0, 0, 0, 0,
		&pSidOwner))
	{
		printf("AllocateAndInitializeSid(Everyone) error: %u\n", GetLastError());
		bRet = FALSE;
		goto Cleanup;
	}

	// Crate the DACL
	EXPLICIT_ACCESS ea[2];
	ZeroMemory(&ea, sizeof(ea));

	// Set read access for Everyone.
	ea[0].grfAccessPermissions = GENERIC_READ;
	ea[0].grfAccessMode = SET_ACCESS;
	ea[0].grfInheritance = NO_INHERITANCE;
	ea[0].Trustee.TrusteeForm = TRUSTEE_IS_SID;
	ea[0].Trustee.TrusteeType = TRUSTEE_IS_WELL_KNOWN_GROUP;
	ea[0].Trustee.ptstrName = (LPTSTR)pSidEveryone;

	// Set full control for Administrators.
	ea[1].grfAccessPermissions = GENERIC_ALL;
	ea[1].grfAccessMode = SET_ACCESS;
	ea[1].grfInheritance = NO_INHERITANCE;
	ea[1].Trustee.TrusteeForm = TRUSTEE_IS_SID;
	ea[1].Trustee.TrusteeType = TRUSTEE_IS_GROUP;
	ea[1].Trustee.ptstrName = (LPTSTR)pSidOwner;

	if (ERROR_SUCCESS != SetEntriesInAcl(2, ea, NULL, &pDACL))
	{
		printf("SetEntriesInAcl error: %u\n", GetLastError());
		bRet = FALSE;
		goto Cleanup;
	}

	SetNamedSecurityInfo((LPWSTR)sRegKey, SE_REGISTRY_KEY, OWNER_SECURITY_INFORMATION, pSidOwner, NULL, NULL, NULL);

	SetPrivilege(procTok, SE_TAKE_OWNERSHIP_NAME, FALSE);

	SetNamedSecurityInfo((LPWSTR)sRegKey, SE_REGISTRY_KEY, DACL_SECURITY_INFORMATION, NULL, NULL, pDACL, NULL);

Cleanup:
	if (pDACL != NULL)
		LocalFree(pDACL);

	if (pSidOwner != NULL)
		FreeSid(pSidOwner);

	if (pSidEveryone != NULL)
		FreeSid(pSidEveryone);

	if (procTok)
		CloseHandle(procTok);

	return bRet;
}

int main()
{
	FixCostRegKeys();

	return 0;
}