#include "EventSync.h"


CEventSync::CEventSync() : m_hEvent(NULL)
{
}


CEventSync::~CEventSync(void)
{
    Destroy();
}

BOOL CEventSync::WaitSignal(DWORD dwMilliseconds)
{
    return WaitForSingleObject(m_hEvent,dwMilliseconds) == WAIT_OBJECT_0;
}

BOOL CEventSync::Signaled()
{
    return SetEvent(m_hEvent);
}

BOOL CEventSync::NonSignaled()
{
    return ResetEvent(m_hEvent);
}

BOOL CEventSync::Create(LPCTSTR lpName /*= NULL*/,
                        BOOL bInitialState /*= TRUE*/,
                        BOOL bManualReset /*= FALSE*/,
                        LPSECURITY_ATTRIBUTES lpEventAttributes /*= NULL*/)
{
    if (m_hEvent != NULL)
        return FALSE;

    if ((m_hEvent=CreateEvent(lpEventAttributes,bManualReset,bInitialState,lpName)) == NULL)
    {
        return FALSE;
    }

    return TRUE;
}

void CEventSync::Destroy()
{
    CloseHandle(m_hEvent);
    m_hEvent = NULL;
}
