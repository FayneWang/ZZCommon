
#include <assert.h>
#include <Windows.h>
#include "SingleProcessGuard.h"


CSingleProcessGuard::CSingleProcessGuard(void) : m_hGurad(INVALID_HANDLE_VALUE)
{}


CSingleProcessGuard::~CSingleProcessGuard(void)
{
	Unguard();
}

BOOL CSingleProcessGuard::Guard(LPCWSTR pGuardName,BOOL bIsGlobal /*= FALSE*/)
{
	if (pGuardName == NULL)
		return FALSE;

    WCHAR szPath[MAX_PATH] = L"";
    wcscpy_s(szPath, bIsGlobal ?  L"Global\\" : L"Local\\");
    wcscat_s(szPath,pGuardName);
    assert(wcslen(pGuardName) < MAX_PATH);

	m_hGurad = ::CreateMutex(NULL, FALSE, szPath);

	return m_hGurad != INVALID_HANDLE_VALUE; 
}

void CSingleProcessGuard::Unguard()
{
	CloseHandle(m_hGurad);
	m_hGurad = INVALID_HANDLE_VALUE;
}


BOOL CSingleProcessGuard::IsGuarded(LPCWSTR pGuardName, BOOL bIsGlobal)
{
	if (pGuardName == NULL)
		return FALSE;

	WCHAR szPath[MAX_PATH] = L"";
	wcscpy_s(szPath, bIsGlobal ? L"Global\\" : L"Local\\");
	wcscat_s(szPath, pGuardName);
	assert(wcslen(pGuardName) < MAX_PATH);

	HANDLE hEvent = OpenMutex(MUTEX_ALL_ACCESS, TRUE, szPath);
	CloseHandle(hEvent);

	return hEvent != NULL;
}
