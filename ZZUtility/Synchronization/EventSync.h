#pragma once

#include <wtypes.h>
#include <ZZUtility/DLLDefines.h>

class _ZZUTILITY_EXTERN_ CEventSync
{
public:
    CEventSync();
    ~CEventSync(void);

    BOOL Create(LPCTSTR lpName = NULL,BOOL bInitialState = TRUE,BOOL bManualReset = FALSE,LPSECURITY_ATTRIBUTES lpEventAttributes = NULL);
    void Destroy();
    BOOL WaitSignal(DWORD dwMilliseconds = INFINITE);
    BOOL Signaled();
    BOOL NonSignaled();

private:
    HANDLE m_hEvent;
};

