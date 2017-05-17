
#include "IoCompletionPortPrivateTypes.h"
#include "IoCompletionHandlerAbstract.h"


CIoCompletionHandlerAbstract::CIoCompletionHandlerAbstract(void) : m(new CIoCompletionHandlerAbstractPrivate),
    m_hHandle(INVALID_HANDLE_VALUE)
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

void CIoCompletionHandlerAbstract::WaitDetachIocpModel()
{
    m->esModelTie.WaitSignal();
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
	m->csCriticalLock.Enter();
}

void CIoCompletionHandlerAbstract::LeaveThread()
{
	m->csCriticalLock.Leave();
}
