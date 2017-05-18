
#include <assert.h>
#include <ZZUtility/Synchronization/EventSync.h>
#include <ZZUtility/Synchronization/MutexSync.h>
#include "IoCompletionHandlerAbstract.h"
#include "IoCompletionPortModel.h"
#include "IoCompletionPortPrivateTypes.h"



// 并发控制处理线程
DWORD WINAPI IocpConcurrentProc(LPVOID lpParameter);

CIoCompletionPortModel *CIoCompletionPortModel::s_pSinglton = NULL;

CIoCompletionPortModel::CIoCompletionPortModel(void) : m(new CIoCompletionPortModelPrivate())
{
	SYSTEM_INFO sysInfo;
	GetSystemInfo(&sysInfo);
	m->vtrThreadHandle.reserve((sysInfo.dwNumberOfProcessors << 1) + 2); // 完成端口的最大并发数： CPU数量*2+2；
}

CIoCompletionPortModel::CIoCompletionPortModel(const CIoCompletionPortModel&) :
m(new CIoCompletionPortModelPrivate())
{}

CIoCompletionPortModel::~CIoCompletionPortModel(void)
{
	CloseHandle(m->hIocp);

	m->attchedSetAndThreadSync.Enter();
	std::vector<HANDLE>  vtrThreadHandle = m->vtrThreadHandle;
	m->attchedSetAndThreadSync.Leave();
	WaitForMultipleObjects(vtrThreadHandle.size(), &vtrThreadHandle[0], TRUE, INFINITE);
	delete m;
}

BOOL CIoCompletionPortModel::AttachHandler(CIoCompletionHandlerAbstract *pHandler)
{
	if (pHandler == NULL ||
		pHandler->m_hHandle == NULL ||
		pHandler->m_hHandle == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}

	m->attchedSetAndThreadSync.Enter();
	if (m->setIocpAttchedObj.find(pHandler) != m->setIocpAttchedObj.end())
	{
		m->attchedSetAndThreadSync.Leave();
		return TRUE;
	}

	HANDLE hIocp = CreateIoCompletionPort(pHandler->m_hHandle, m->hIocp,
		reinterpret_cast<ULONG_PTR>(pHandler), m->vtrThreadHandle.capacity());
	if (hIocp == NULL)
	{
		m->attchedSetAndThreadSync.Leave();
		return FALSE;
	}

	pHandler->_AttachIocpModel();
	if (!pHandler->OverlapForIOCompletion())
	{
		m->attchedSetAndThreadSync.Leave();
		pHandler->_DetachIocpModel();
		return FALSE;
	}
	else
	{
		_InterlockedIncrement(&pHandler->m->lOverlappedCount);
	}

	m->hIocp = hIocp;
	if (m->vtrThreadHandle.capacity() > m->vtrThreadHandle.size()
		&& (m->setIocpAttchedObj.size() >> 1) >= m->vtrThreadHandle.size()) // 每个线程至少服务2个io对象。
	{
		ThreadData *pThreadData = new ThreadData;
		pThreadData->pIocpModel = this;
		pThreadData->hThread = CreateThread(NULL, 0, IocpConcurrentProc, pThreadData, 0, NULL);
		if (NULL != pThreadData->hThread)
		{
			m->vtrThreadHandle.push_back(pThreadData->hThread);
		}
		else
		{
			delete pThreadData;
		}
	}

	if (m->vtrThreadHandle.empty())
	{
		m->attchedSetAndThreadSync.Leave();
		pHandler->_DetachIocpModel();
		CloseHandle(m->hIocp);
		m->hIocp = NULL;
		return FALSE;
	}

	m->setIocpAttchedObj.insert(pHandler);
	m->attchedSetAndThreadSync.Leave();


	return TRUE;
}

void CIoCompletionPortModel::DetachHandler(CIoCompletionHandlerAbstract *pHandler)
{
	m->attchedSetAndThreadSync.Enter();
	if (m->setIocpAttchedObj.find(pHandler) == m->setIocpAttchedObj.end())
	{
		m->attchedSetAndThreadSync.Leave();
		return;
	}

	m->setIocpAttchedObj.erase(pHandler);
	m->attchedSetAndThreadSync.Leave();

	pHandler->Destroy(); // Destroy() 成员函数中需要将 与 m_hIocp句柄关联的对象销毁，才能确保关联系对象安全退出。
}

BOOL CIoCompletionPortModel::PostHandlerStatus(CIoCompletionHandlerAbstract *pHandler)
{
	_InterlockedIncrement(&pHandler->m->lOverlappedCount);
	return ::PostQueuedCompletionStatus(m->hIocp, 0, reinterpret_cast<ULONG_PTR>(pHandler), NULL);
}

BOOL CIoCompletionPortModel::_DetachHanderl(CIoCompletionHandlerAbstract *pCompletionHandler)
{
	if (pCompletionHandler->m->lOverlappedCount == 0)
	{
		m->attchedSetAndThreadSync.Enter();
		BOOL bExit = (m->setIocpAttchedObj.size() >> 1) < m->vtrThreadHandle.size(); // io对象数量除以2小于线程数时退出线程
		m->setIocpAttchedObj.erase(pCompletionHandler);
		m->attchedSetAndThreadSync.Leave();

		if (pCompletionHandler->m->bAutoDelete)
		{
			pCompletionHandler->_DetachIocpModel(); //执行分离处理后， IOCP Handler对象可以完全脱离 IOCP模型对象。
			delete this;
		}
		else
		{
			// 事件有信号标记必须放在其他成员函数的调用后面，避免 有信号 标记之后，执行了 delete 对象之后，再调用到对象的其他成员。
			pCompletionHandler->_DetachIocpModel(); //执行分离处理后， IOCP Handler对象可以完全脱离 IOCP模型对象。
		}

		return bExit;
	}

	return FALSE;
}

void CIoCompletionPortModel::_IocpProc()
{
	DWORD           dwNumOfTransferredBytes;
	ULONG_PTR       lCompletionKey;
	LPOVERLAPPED    lpOverlapped;

	do
	{
		if (GetQueuedCompletionStatus(m->hIocp, &dwNumOfTransferredBytes, &lCompletionKey, &lpOverlapped, INFINITE))
		{
			if (lCompletionKey == NULL)
			{
				break;  // exit loop of do{...}while(lCompletionKey != NULL).
			}

			CIoCompletionHandlerAbstract *pIocpHandler = reinterpret_cast<CIoCompletionHandlerAbstract *>(lCompletionKey);
			if (pIocpHandler->DataTransferTrigger(dwNumOfTransferredBytes))
			{
				if (!pIocpHandler->OverlapForIOCompletion())
				{
					_InterlockedDecrement(&pIocpHandler->m->lOverlappedCount);
				}
			}
			else
			{
				_InterlockedDecrement(&pIocpHandler->m->lOverlappedCount);
			}

			if (_DetachHanderl(pIocpHandler))
			{
				break;  // exit loop of do{...}while(lCompletionKey != NULL).
			}
		}
		else
		{
			if (lCompletionKey != NULL)
			{
				// 已重叠的句柄在直接关闭或者通讯另一端关闭退出时，会到达这一步骤。
				CIoCompletionHandlerAbstract *pIocpHandler = reinterpret_cast<CIoCompletionHandlerAbstract*>(lCompletionKey);
				_InterlockedDecrement(&pIocpHandler->m->lOverlappedCount);
				pIocpHandler->HandleRaiseError(GetLastError());
				if (_DetachHanderl(pIocpHandler))
				{
					break;  // exit loop of do{...}while(lCompletionKey != NULL).
				}
			}
			else
			{
				// 完成端口句柄出现异常时。
				int iErrorCode = GetLastError();

				// TODO: prompt error info.
				break; // exit : while (lCompletionKey != NULL) loop!
			}
		}

	} while (lCompletionKey != NULL);
}

CIoCompletionPortModel * CIoCompletionPortModel::GetInstance()
{
	if (NULL == s_pSinglton)
	{
		// 单例模式支持线程安全的处理。
		CMutexSync mutex(TRUE, L"Local\\CIoCompletionPortModel");
		if (NULL == s_pSinglton)
		{
			CIoCompletionPortModel *pSinglton = new CIoCompletionPortModel();
			s_pSinglton = pSinglton;
		}
	}

	return s_pSinglton;
}

void CIoCompletionPortModel::Uninstance()
{
	// TODO: 用于以后未来在 这个类的完成端口线程中，如果没有对象关联便在完成响应线程中自动自我销毁人线程安全保证。
	if (nullptr != s_pSinglton)
	{
		CMutexSync syncLock(TRUE, L"Local\\CPlayerTcpClientIpc");
		if (nullptr != s_pSinglton)
		{
			delete s_pSinglton;
			s_pSinglton = nullptr;
		}
	}
}

HANDLE CIoCompletionPortModel::GetHandle()
{
	return m->hIocp;
}

void CIoCompletionPortModel::_ThreadHanleProc(HANDLE hThread)
{
	_IocpProc();

	m->attchedSetAndThreadSync.Enter();
	for (std::vector<HANDLE>::iterator itrBegin = m->vtrThreadHandle.begin(), itrEnd = m->vtrThreadHandle.end();
		itrBegin != itrEnd; ++itrBegin)
	{
		if (hThread == *itrBegin)
		{
			HANDLE hThread = GetCurrentThread();
			m->vtrThreadHandle.erase(itrBegin);
			CloseHandle(hThread);
			break; // exit for;
		}
	}
	m->attchedSetAndThreadSync.Leave();

	// TODO: 所有的线程都退出，则销毁IOCP Model中所有的资源。
}

DWORD WINAPI IocpConcurrentProc(LPVOID lpParameter)
{
	CIoCompletionPortModel::ThreadData *pThreadData =
		static_cast<CIoCompletionPortModel::ThreadData*>(lpParameter);

	pThreadData->pIocpModel->_ThreadHanleProc(pThreadData->hThread);

	delete pThreadData;

	return 0;
}