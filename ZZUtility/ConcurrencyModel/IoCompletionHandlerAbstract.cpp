
#include "IoCompletionPortPrivateTypes.h"
#include "IoCompletionHandlerAbstract.h"


CIoCompletionHandlerAbstract::CIoCompletionHandlerAbstract(void) : m(new CIoCompletionHandlerAbstractPrivate),
    m_hHandle(INVALID_HANDLE_VALUE),
    m_iIoCompletionStatus(ICS_OVERLAP_HANDLE)
{
    memset(&m->overlap,0x00,sizeof(m->overlap));
    m->esModelTie.Create(NULL,TRUE,TRUE);
}


CIoCompletionHandlerAbstract::~CIoCompletionHandlerAbstract(void)
{
     delete m;
}

void CIoCompletionHandlerAbstract::SetAutoDelete(BOOL bAutoDelete)
{
	m->bAutoDelete = bAutoDelete;
}

OVERLAPPED * CIoCompletionHandlerAbstract::IocpAsyncOverlap()
{
    return &m->overlap;
}

void CIoCompletionHandlerAbstract::Destroy()
{
	m->csMultiThread.Enter();
	if (m_hHandle != nullptr)
	{
		CloseHandle(m_hHandle);
		m_hHandle = NULL;
	}
	m->csMultiThread.Leave();
}

BOOL CIoCompletionHandlerAbstract::IsDestroyed()
{
	bool bDestroyed;
	m->csMultiThread.Enter();
	bDestroyed = m_hHandle == nullptr || m_hHandle == INVALID_HANDLE_VALUE;
	m->csMultiThread.Leave();

    return bDestroyed;
}

void CIoCompletionHandlerAbstract::WaitDetachIocpModel()
{
    m->esModelTie.WaitSignal();
}

BOOL CIoCompletionHandlerAbstract::_IsAutoDelete()
{
    return m->bAutoDelete;
}

void CIoCompletionHandlerAbstract::_AttachIocpModel()
{
    m->esModelTie.NonSignaled();
}

void CIoCompletionHandlerAbstract::_DetachIocpModel()
{
   m->esModelTie.Signaled();
}

void CIoCompletionHandlerAbstract::EnterThread()
{
	m->csMultiThread.Enter();
}

void CIoCompletionHandlerAbstract::LeaveThread()
{
	m->csMultiThread.Leave();
}
