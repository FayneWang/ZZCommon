
#include <windows.h>
#include "SemaphoreSync.h"


CSemaphoreSync::CSemaphoreSync() :m_hSemaphore(nullptr)
{}

CSemaphoreSync::~CSemaphoreSync(void)
{
    CloseHandle(m_hSemaphore);
}

int CSemaphoreSync::Create(LONG lInitialCount,LONG lMaximumCount,LPCWSTR pSemaphoreName)
{
    m_hSemaphore = ::CreateSemaphore(NULL,lInitialCount,lMaximumCount,pSemaphoreName);
    return m_hSemaphore == NULL ? GetLastError() : 0;
}

bool CSemaphoreSync::WaitForSemaporeSignal()
{
    if (m_hSemaphore == NULL)
        return false;

    return ::WaitForSingleObject(m_hSemaphore,INFINITE) == WAIT_OBJECT_0;
}

bool CSemaphoreSync::IncreaseSemaphoreSignal(LONG lReleaseCount)
{
    if (m_hSemaphore == NULL)
        return false;

    return ::ReleaseSemaphore(m_hSemaphore,lReleaseCount,NULL) == TRUE;
}
