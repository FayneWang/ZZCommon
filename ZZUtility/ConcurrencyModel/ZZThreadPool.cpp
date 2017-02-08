
#include <assert.h>
#include <list>
#include <set>
#include <vector>
#include <Windows.h>
#include <ZZUtility/Synchronization/CriticalSectionSync.h>
#include "ZZThreadPoolWork.h"
#include "ZZThreadPool.h"

#define ZZTP_MINIMUM_THREAD_NUM     0
#define ZZTP_DEFAULT_THREAD_NUM     5U
#define ZZTP_DEFAULT_EXPIRY_TIME    30000U

class CWorkThread
{
public:
    CZZThreadPool       *m_pThreadPool;
    HANDLE              m_hWorkThread;
    HANDLE              m_hWaitForExpiry;  // 工作线程超时等待对象

    CZZThreadPoolWork   *m_pWork;
    
    CWorkThread(CZZThreadPool *pThreadPool,CZZThreadPoolWork *pWork) : 
        m_pThreadPool(pThreadPool),
        m_hWorkThread(NULL),
        m_hWaitForExpiry(NULL),
        m_pWork(pWork)
    {}

    ~CWorkThread()
    {
        CloseHandle(m_hWorkThread);
    }

    BOOL InitThread()
    {
        // 创建工作线程结象。
        m_hWaitForExpiry = CreateEvent(NULL,FALSE,FALSE,NULL);
        if (m_hWaitForExpiry == nullptr)
            return FALSE;

        m_hWorkThread = CreateThread(NULL,0,ThreadPoolThreadProc,this,0,NULL);
        if (m_hWorkThread == nullptr)
        {
            CloseHandle(m_hWaitForExpiry);
            m_hWaitForExpiry = nullptr;
            return FALSE;
        }

        return TRUE;
    }

    void Destroy()
    {
        m_pThreadPool = nullptr;
        HANDLE hTmp = m_hWaitForExpiry;
        m_hWaitForExpiry = nullptr;

        SetEvent(hTmp); // 激活可能正在等待工作的线程，使安全退出。
        CloseHandle(hTmp);
    }

    void StopWork()
    {
        if (m_pWork != NULL)
            m_pWork->StopWork();
    }

    BOOL IsDestroy()
    {
        return m_hWaitForExpiry == nullptr;
    }
};

typedef std::list<CWorkThread*>         ZZTP_WORK_THREAD_LIST;
typedef std::set<CWorkThread*>          ZZTP_WORK_THREAD_SET;
typedef std::list<CZZThreadPoolWork*>   ZZTP_UNWORKED_LIST;

struct CZZThreadPoolPrivate
{
    ZZTP_WORK_THREAD_LIST       lsRestingThreads;   // 使用链表是为了让当有新的工作进来的时候，最后进入休息的线程，优先进入工作处理。而最新进入休息的终其一生程，直接销毁
    ZZTP_WORK_THREAD_SET        setWorkingThreads;
    CCriticalSectionSync        csWorkThreadSync;
    ZZTP_UNWORKED_LIST          lsUnworked;
    CCriticalSectionSync        csUnworkedSync;
    uint16_t uMinThreadCount;
    uint16_t uMaxThreadCount;
    uint32_t uExpiryTimeMs;

    CZZThreadPoolPrivate() : uMinThreadCount(ZZTP_MINIMUM_THREAD_NUM),
                              uMaxThreadCount(ZZTP_DEFAULT_THREAD_NUM),
                              uExpiryTimeMs(ZZTP_DEFAULT_EXPIRY_TIME)
    {
    }
};

CZZThreadPool *CZZThreadPool::ms_globalInstance = NULL;

CZZThreadPool::CZZThreadPool(void) : m(new CZZThreadPoolPrivate())
{
}

CZZThreadPool::~CZZThreadPool(void)
{
    delete m;
}

CZZThreadPool * CZZThreadPool::GlobalInstance()
{
    if (ms_globalInstance == NULL)
        ms_globalInstance = new CZZThreadPool();

    return ms_globalInstance;
}

BOOL CZZThreadPool::PushWork(CZZThreadPoolWork *pWork)
{
    assert(pWork != NULL);
    if (pWork == NULL)
        return FALSE;

    m->csWorkThreadSync.Enter();
    if (m->lsRestingThreads.size() > 0)
    {
        m->csWorkThreadSync.Leave();

        // 使用在休眠的工作线程，执行新的工作。
        m->csWorkThreadSync.Enter();
        CWorkThread *pWorkThread = m->lsRestingThreads.back();
        m->lsRestingThreads.pop_back();

		pWork->WaitForWork();
        pWorkThread->m_pWork = pWork;
        SetEvent(pWorkThread->m_hWaitForExpiry);
        m->setWorkingThreads.insert(pWorkThread);
        m->csWorkThreadSync.Leave();
    }
    else if(m->setWorkingThreads.size() < m->uMaxThreadCount)
    {
        m->csWorkThreadSync.Leave();

        // 创建新的工作线程，执行新的工作。

		pWork->WaitForWork();
        CWorkThread *pWorkThread = new CWorkThread(this,pWork);
        if (pWorkThread->InitThread())
        {
            m->setWorkingThreads.insert(pWorkThread);
        }
        else
        {
            delete pWorkThread;

            m->csWorkThreadSync.Enter();
            int iCurrentThreadCount = m->setWorkingThreads.size()+m->lsRestingThreads.size();
            m->csWorkThreadSync.Leave();
            if (iCurrentThreadCount>0)
            {
                // 线程创建失败的时间有存在工作线程则， PushWork() 也当作是成功的。
                m->csUnworkedSync.Enter();
				pWork->WaitForWork();
                m->lsUnworked.push_back(pWork);
                m->csUnworkedSync.Leave();
                return TRUE;
            }

            return FALSE;
        }
    }
    else
    {
        m->csWorkThreadSync.Leave();

		m->csUnworkedSync.Enter();
		pWork->WaitForWork();
        m->lsUnworked.push_back(pWork);
        m->csUnworkedSync.Leave();
    }

    return TRUE;
}

BOOL CZZThreadPool::RemoveWork(CZZThreadPoolWork *pWork)
{
    if (pWork == NULL)
        return FALSE;

	pWork->StopWork();

    m->csUnworkedSync.Enter();
    for (auto itrBegin=m->lsUnworked.begin(),itrEnd = m->lsUnworked.end();
        itrBegin != itrEnd;
        ++itrBegin)
    {
        if (*itrBegin == pWork)
        {
            m->lsUnworked.erase(itrBegin);
            m->csUnworkedSync.Leave();
            return TRUE;
        }
    }
    m->csUnworkedSync.Leave();

    return FALSE;
}

uint16_t CZZThreadPool::GetMaxThreadCount() const
{
    return m->uMaxThreadCount;
}

uint16_t CZZThreadPool::GetMinThreadCount() const
{
    return m->uMinThreadCount;
}

uint16_t CZZThreadPool::SetMaxThreadCount(uint16_t uMaxThreadCount)
{
    if (uMaxThreadCount == 0)
        uMaxThreadCount = 1;

    if (m->uMaxThreadCount < uMaxThreadCount)
    {
        m->csUnworkedSync.Enter();
        int iUnworkedCount = m->lsUnworked.size();
        m->csUnworkedSync.Leave();

        if (iUnworkedCount > 0)
        {
            CWorkThread *pWorkThread;
            int iDifferenceOfCount = uMaxThreadCount-m->uMaxThreadCount;
            iDifferenceOfCount = iUnworkedCount>iDifferenceOfCount ? iDifferenceOfCount : iUnworkedCount;

            m->csWorkThreadSync.Enter();
            m->csUnworkedSync.Enter();
            for (; iDifferenceOfCount>0; --iDifferenceOfCount)
            {
                pWorkThread = new CWorkThread(this,m->lsUnworked.front());
                m->lsUnworked.pop_front();
                if (pWorkThread->InitThread())
                {
                    m->setWorkingThreads.insert(pWorkThread);
                }
                else
                {
                    m->lsUnworked.push_front(pWorkThread->m_pWork);
                    break;
                }
            }
            m->csUnworkedSync.Leave();
            m->csWorkThreadSync.Leave();
        }

    }
    else if(m->uMaxThreadCount > uMaxThreadCount)
    {
        // 销毁多于最大线程数的线程。
        int iNumOfThread = int32_t(m->uMaxThreadCount) - uMaxThreadCount;        
        m->csWorkThreadSync.Enter();

        auto itrListEraseFirst = m->lsRestingThreads.begin();
        auto itrListEraseEnd = itrListEraseFirst;
        for (auto itrEnd = m->lsRestingThreads.end();
            (itrListEraseEnd != itrEnd) && (iNumOfThread>0);
            ++itrListEraseEnd)
        {
            (*itrListEraseEnd)->Destroy();
            --iNumOfThread;
        }
        m->lsRestingThreads.erase(itrListEraseFirst,itrListEraseEnd);

        auto itrSetEraseFirst = m->setWorkingThreads.begin();
        auto itrSetEraseEnd = itrSetEraseFirst;
        for (auto itrEnd = m->setWorkingThreads.end();
            (itrSetEraseEnd!=itrEnd) && (iNumOfThread>0);
            ++itrSetEraseEnd)
        {
            (*itrSetEraseEnd)->Destroy();
            --iNumOfThread;
        }
        m->setWorkingThreads.erase(itrSetEraseFirst,itrSetEraseEnd);
        m->csWorkThreadSync.Leave();

        // 先将线程数将低，再设置最小线程数
        if (m->uMinThreadCount>uMaxThreadCount)
        {
            // 最小线程数大于最大线程数时，将最小线程数设置为最大线程数。
            SetMinThreadCount(uMaxThreadCount);
        }
    }

    m->uMaxThreadCount = uMaxThreadCount;
    return m->uMaxThreadCount;
}


uint16_t CZZThreadPool::SetMinThreadCount(uint16_t uMinThreadCount)
{
    if (m->uMinThreadCount>uMinThreadCount)
    {
        /* 新设置的最小线程数小于原来的最小线程数时，销毁正在休眠的线程数。 */
        m->csWorkThreadSync.Enter();

        auto itrListEraseFirst = m->lsRestingThreads.begin();
        auto itrListEraseEnd = itrListEraseFirst;
        for (auto itrEnd = m->lsRestingThreads.end();
            (itrListEraseEnd != itrEnd) && (m->uMinThreadCount>uMinThreadCount);
            ++itrListEraseEnd)
        {
            (*itrListEraseEnd)->Destroy();
            --m->uMinThreadCount;
        }
        m->lsRestingThreads.erase(itrListEraseFirst,itrListEraseEnd);

        m->csWorkThreadSync.Leave();
    }
    else if (m->uMaxThreadCount < uMinThreadCount)
    {
        SetMaxThreadCount(uMinThreadCount);
    }

    m->uMinThreadCount = uMinThreadCount;
    return m->uMinThreadCount;
}

uint16_t CZZThreadPool::GetActiveThreadCount() const
{
    m->csWorkThreadSync.Enter();
    uint32_t uActiveThreadSize = m->setWorkingThreads.size() + m->lsRestingThreads.size();
    m->csWorkThreadSync.Leave();

    return uActiveThreadSize;
}

uint32_t CZZThreadPool::GetExpiryTimeMs() const
{
    return m->uExpiryTimeMs;
}

void CZZThreadPool::SetExpiryTimeMs(uint32_t uExpiryTimeMs)
{
    if (m->uExpiryTimeMs > uExpiryTimeMs)
    {
        // 重置过期时间比原来小，且在休眠的线程的等待过期时间。
        m->csWorkThreadSync.Enter();
        CWorkThread *pWorkThread;
        for (auto itrBegin = m->lsRestingThreads.begin(), itrEnd = m->lsRestingThreads.end();
            itrBegin != itrEnd;
            ++itrBegin)
        {
            pWorkThread = *itrBegin;
            SetEvent(pWorkThread->m_hWaitForExpiry);
            --m->uMinThreadCount;
        }
        m->csWorkThreadSync.Leave();
    }

    m->uExpiryTimeMs = uExpiryTimeMs;
}

void CZZThreadPool::ClearAndWait()
{
    m->csUnworkedSync.Enter();
    m->lsUnworked = ZZTP_UNWORKED_LIST();
    m->csUnworkedSync.Leave();

    std::vector<HANDLE> vtrThreadHandle;
    CWorkThread *pWorkThread;
    m->csWorkThreadSync.Enter();
    for (auto itrBegin = m->lsRestingThreads.begin(),itrEnd = m->lsRestingThreads.end();
        itrBegin != itrEnd;
        ++itrBegin)
    {
        pWorkThread = (*itrBegin);
        pWorkThread->Destroy();

        vtrThreadHandle.push_back(pWorkThread->m_hWorkThread);
    }
    m->lsRestingThreads = ZZTP_WORK_THREAD_LIST();

    for (auto itrBegin = m->setWorkingThreads.begin(),itrEnd = m->setWorkingThreads.end();
        itrBegin != itrEnd;
        ++itrBegin)
    {
        pWorkThread = (*itrBegin);
        pWorkThread->Destroy();
        pWorkThread->StopWork();

        vtrThreadHandle.push_back(pWorkThread->m_hWorkThread);
    }
    m->setWorkingThreads = ZZTP_WORK_THREAD_SET();
    m->csWorkThreadSync.Leave();

    if (!vtrThreadHandle.empty())
    {
        WaitForMultipleObjects(vtrThreadHandle.size(),&vtrThreadHandle[0],TRUE,INFINITE);
    }
}

void CZZThreadPool::_DoWork(CWorkThread *pWorkThread)
{
    BOOL bInfiniteThread = FALSE;
    do 
    {
        do 
        {
            if (pWorkThread->m_pWork != NULL)
            {
                pWorkThread->m_pWork->Run();
                if (pWorkThread->IsDestroy())
                {
                    delete pWorkThread;
                    return;
                }

                // 工作线程执行完后，检查线程是否已被销毁。
                // 销毁的线程直接退出线程。
                m->csWorkThreadSync.Enter();
                pWorkThread->m_pWork = nullptr;
                m->csUnworkedSync.Enter();
                if (m->lsUnworked.empty())
                {
                    m->csUnworkedSync.Leave();

                    m->setWorkingThreads.erase(pWorkThread);
                    m->lsRestingThreads.push_back(pWorkThread);
                    m->csWorkThreadSync.Leave();
                    break;
                }
                else
                {
                    // 这里使用m->csWorkThreadSync线程临界区锁住是为了保证在调用ClearAndWait()时,
                    // 调用pWorkThread->StopWork()能进行同步。

                    pWorkThread->m_pWork = m->lsUnworked.front();
                    m->lsUnworked.pop_front();
                    m->csUnworkedSync.Leave();

                    m->csWorkThreadSync.Leave();
                }

            }
            else
            {
                m->csWorkThreadSync.Enter();
                m->setWorkingThreads.erase(pWorkThread);
                m->lsRestingThreads.push_back(pWorkThread);
                m->csWorkThreadSync.Leave();
                break;
            }

        } while (true);


        switch (WaitForSingleObject(pWorkThread->m_hWaitForExpiry,bInfiniteThread ? INFINITE : m->uExpiryTimeMs))
        {
        case WAIT_OBJECT_0:
            {
                // 只要是被Destroy的工作线程对象，都已从 lsRestingThreads 或 setWorkingThreads中移除，所以不需要再从这两个集合中进行移除操作。
                if (pWorkThread->IsDestroy())            
                {
                    delete pWorkThread;
                    return;
                }
                else
                {
                    // 调用 PushWork() 之后触发的流程。
                    continue;
                }
            }

        case WAIT_FAILED :
            
            if (pWorkThread->IsDestroy())
            {
                delete pWorkThread;
                return;  // exit thread;
            }

            break;
        case WAIT_TIMEOUT:
            {
                if (pWorkThread->IsDestroy())
                {
                    delete pWorkThread;
                    return;  // exit thread;
                }
                else
                {
                    m->csWorkThreadSync.Enter();
                    if (pWorkThread->IsDestroy())
                    {
                        // 安全起见，再次检查是否已被销毁
                        m->csWorkThreadSync.Leave();
                        delete pWorkThread;
                        return;  // exit thread;
                    }
                    else
                    {
                        // 如果最小的工作线程数大于0时，必然存在不销毁的工作线程；
                        // 当目前的工作线程数和等待工作的线程数与最小工作线程数相等时退入永远等待状态，
                        bInfiniteThread = m->uMinThreadCount == (m->lsRestingThreads.size() + m->setWorkingThreads.size());
                        if (bInfiniteThread)
                        {
                            m->lsRestingThreads.remove(pWorkThread);
                            m->setWorkingThreads.insert(pWorkThread);
                            m->csWorkThreadSync.Leave();
                        }
                        else
                        {
                            // 工作线程等待超时，自然毁销工作线程。
                            m->lsRestingThreads.remove(pWorkThread);
                            CloseHandle(pWorkThread->m_hWaitForExpiry);
                            m->csWorkThreadSync.Leave();
                            delete pWorkThread;
                            return;  // exit thread;
                        }
                    }
                }

                break;
            }
        default:
            break;
        }

    } while (true);
}

DWORD WINAPI ThreadPoolThreadProc(void *pVoid)
{
    CWorkThread *&pWorkThread = reinterpret_cast<CWorkThread *&>(pVoid);

    if (pWorkThread->m_pThreadPool != nullptr)
        pWorkThread->m_pThreadPool->_DoWork(pWorkThread);

    return 0;
}