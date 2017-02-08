#pragma once

#include <wtypes.h>
#include <ZZUtility/DLLDefines.h>


class _ZZUTILITY_EXTERN_ CCriticalSectionSync
{
public:
    CCriticalSectionSync(DWORD dwSpinCount = 4000);
    ~CCriticalSectionSync(void);

    void Enter();
    void Leave();
    DWORD SetCriticalSectionSpinCount(DWORD dwSpinCount);
private:
	CRITICAL_SECTION m_criticalSection;
};

