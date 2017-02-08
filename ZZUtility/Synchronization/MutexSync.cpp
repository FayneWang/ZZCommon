
#include <Windows.h>
#include "MutexSync.h"

CMutexSync::CMutexSync(BOOL bInitialOwner /*= FALSE*/,LPCWSTR pMutextName /*= NULL*/,
	LPSECURITY_ATTRIBUTES lpMutexAttributes /*= NULL*/) : m_hMutext(CreateMutex(lpMutexAttributes,bInitialOwner,pMutextName))
{
}

CMutexSync::~CMutexSync(void)
{
    CloseHandle(m_hMutext);
}

BOOL CMutexSync::Lock(DWORD dwMiliSeconds /*= INFINITE*/)
{
    return WaitForSingleObject(m_hMutext,dwMiliSeconds) == WAIT_OBJECT_0;
}

BOOL CMutexSync::Unlock()
{
    return ReleaseMutex(m_hMutext);
}
