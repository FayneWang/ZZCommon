#pragma once

#include <wtypes.h>
#include <ZZUtility/DLLDefines.h>

class _ZZUTILITY_EXTERN_ CMutexSync
{
public:
    CMutexSync(BOOL bInitialOwner = FALSE,LPCWSTR pMutextName = NULL,LPSECURITY_ATTRIBUTES lpMutexAttributes = NULL);
    ~CMutexSync(void);

    BOOL Lock(DWORD dwMiliSeconds = INFINITE);
    BOOL Unlock();

private:
	HANDLE m_hMutext;
};

