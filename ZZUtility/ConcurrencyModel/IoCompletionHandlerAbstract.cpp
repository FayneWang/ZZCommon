
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
	this->Destroy();
	if (m->bAutoDelete)
	{
		m->esModelTie.Signaled();
		delete this;
	}
	else
	{
		// 事件有信号标记必须放在其他成员函数的调用后面，避免 有信号 标记之后，执行了 delete 对象之后，再调用到对象的其他成员。
		m->esModelTie.Signaled(); 
	}
}

void CIoCompletionHandlerAbstract::EnterThread()
{
	m->csCriticalLock.Enter();
}

void CIoCompletionHandlerAbstract::LeaveThread()
{
	m->csCriticalLock.Leave();
}
