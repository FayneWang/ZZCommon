
#include <windows.h>
#include "CriticalSectionSync.h"

CCriticalSectionSync::CCriticalSectionSync(DWORD dwSpinCount)
{
	// 4000ȡ�ԡ�Windowsϵͳ��̡��ĵ�9�����������Լ�NT6��ǿ�е�CRITICAL_SECTION-������֮���Ȩ��
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
