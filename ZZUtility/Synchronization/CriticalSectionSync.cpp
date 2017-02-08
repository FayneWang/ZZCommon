
#include <windows.h>
#include "CriticalSectionSync.h"

CCriticalSectionSync::CCriticalSectionSync(DWORD dwSpinCount)
{
	// 4000取自《Windows系统编程》的第9章锁、性能以及NT6增强中的CRITICAL_SECTION-互拆量之间的权衡
    InitializeCriticalSectionAndSpinCount(&m_criticalSection,dwSpinCount); 
}


CCriticalSectionSync::~CCriticalSectionSync(void)
{
    DeleteCriticalSection(&m_criticalSection);
}

void CCriticalSectionSync::Enter()
{
    EnterCriticalSection(&m_criticalSection);
}

void CCriticalSectionSync::Leave()
{
    LeaveCriticalSection(&m_criticalSection);
}

DWORD CCriticalSectionSync::SetCriticalSectionSpinCount(DWORD dwSpinCount)
{
    return ::SetCriticalSectionSpinCount(&m_criticalSection,dwSpinCount);
}
