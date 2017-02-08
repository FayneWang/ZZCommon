
#include <assert.h>
#include <stdio.h>
#include <Windows.h>
#include "SingleProcessGuard.h"

struct CSingletonProcessGuardPrivate
{
    HANDLE  hEvent;
    UINT    uNotifyMsg;

    CSingletonProcessGuardPrivate() : hEvent(NULL),uNotifyMsg(0)
    {}
};

CSingleProcessGuard::CSingleProcessGuard(void) : m(new CSingletonProcessGuardPrivate())
{}


CSingleProcessGuard::~CSingleProcessGuard(void)
{
    CloseHandle(m->hEvent);
}

HANDLE CSingleProcessGuard::GuiadHandle(LPCWSTR pGuardName,BOOL bIsGlobal /*= FALSE*/)
{
    WCHAR szPath[MAX_PATH] = L"";
    wcscpy_s(szPath, bIsGlobal ?  L"Global\\" : L"Local\\");
    wcscat_s(szPath,pGuardName);
    assert(wcslen(pGuardName) < MAX_PATH);

    return OpenMutex(MUTEX_ALL_ACCESS,TRUE,szPath);
}

BOOL CSingleProcessGuard::HasGuarded(LPCWSTR pGuardName,BOOL bIsGlobal)
{
    WCHAR szPath[MAX_PATH] = L"";
    wcscpy_s(szPath, bIsGlobal ?  L"Global\\" : L"Local\\");
    wcscat_s(szPath,pGuardName);
    assert(wcslen(pGuardName) < MAX_PATH);

    HANDLE hEvent = OpenMutex(MUTEX_ALL_ACCESS,TRUE,szPath);
    if (hEvent == NULL)
    {
        return GetLastError() == ERROR_ACCESS_DENIED;
    }

    CloseHandle(hEvent);
    return TRUE;
}


BOOL CSingleProcessGuard::GuardSingleProcess(LPCWSTR pGuardName, BOOL bIsGlobal)
{
    if (m->hEvent != NULL)
        return FALSE;

    assert(NULL != pGuardName);

    WCHAR szPath[MAX_PATH] = L"";
    wcscpy_s(szPath, bIsGlobal ?  L"Global\\" : L"Local\\");
    wcscat_s(szPath,pGuardName);
    assert(wcslen(pGuardName) < MAX_PATH);

    m->hEvent = CreateMutex(NULL,TRUE,szPath);
    if (NULL == m->hEvent || 
        ERROR_ALREADY_EXISTS == GetLastError())
    {
        return FALSE;
    }

    return TRUE;
}


void CSingleProcessGuard::UnguardSingleProcess()
{
    if (m->hEvent == NULL)
        return;

    HANDLE hEvent = m->hEvent;
    m->hEvent = NULL;
    CloseHandle(hEvent);
}
