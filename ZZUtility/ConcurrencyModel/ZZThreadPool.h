#pragma once

#include <ZZUtility/DLLDefines.h>
#include <stdint.h>
#include <wtypes.h>

class CZZThreadPoolWork;
struct CZZThreadPoolPrivate;
class CWorkThread;
class _ZZUTILITY_EXTERN_ CZZThreadPool
{
public:
    CZZThreadPool(void);
    virtual ~CZZThreadPool(void);

public:  // static member;
    static CZZThreadPool *GlobalInstance();

public:
    /**
     * 添加一个工作对象到线程池中。
     * @param[in] pWork 一个要执行工作的对象。
     * @return 工作对象成功添加到线程池中或工作队列，返回TRUE,否则返回FALSE。
     */
    BOOL        PushWork(CZZThreadPoolWork *pWork);

    /**
     * 从工作队列中移除，未执行的工作。
     * @param[in] pWork 要移除的工作对象
     * @return 如果成功从工作队列中删除工作对象返回TRUE，否则返回FALSE。
     *         从工作队列中移除删除队列失败，有可能是工作已开始执行或已执行结束。
     */
    BOOL        RemoveWork(CZZThreadPoolWork *pWork);

    /**
     * 清除所有未执行的工作对象，并等待正在执行的工作对象执行完成并对退出所有进程。
     */
    void        ClearAndWait();

    /**
     * 获取允许线程池最大的工作线程数
     * @return 返回线程池最大允许的工作线程数
     */
    uint16_t    GetMaxThreadCount() const;

    /**
     * 设置允许的最大工作线程数
     * 1、当最大线程数小于当前的最小线程数时，最小线程数将释放超过的线程。
     * 2、当设置超过宏定义的ENTP_MAXNIMUM_THREAD_NUM值时，最大允许的工作线程数为 ENTP_MAXNIMUM_THREAD_NUM
     *    （当前值为：500）定义的值。
     *
     * @param[in]   uMaxThreadCount 允许线程池最大的工作线程数。
     * @return      返回实际允许的最大线程数。
     */
    uint16_t    SetMaxThreadCount(uint16_t uMaxThreadCount);

    /**
     * 获取允许的最少工作线程数
     * @return 返回允许的最少的工作线程数
     */
    uint16_t    GetMinThreadCount() const;

    /**
     * 设置允许的最少工作线程数
     * 当设置的最少线程数超过最大允许的线程数时，最少允许线程数为最大的工作线程数 GetMaxThreadCount()。
     * @param[in] uMinThreadCount 允许的最少工作线程数
     * @return 实际允许的最小线程数
     */
    uint16_t    SetMinThreadCount(uint16_t uMinThreadCount);

    /**
     * 获当前线程池中存在的工作线程数(包括正在工作和正在休眠的线程)。
     * return 当前的线程池中的线程数
     */
    uint16_t    GetActiveThreadCount() const;

    /**
     * 获取线程休眠退出时间(ms)值。
     * return 线程休眠退出时间(ms)。
     */
    uint32_t    GetExpiryTimeMs() const;

    /**
     * 设置线程完成工作并进入休眠等待工作或等待超时退出的时间(ms)值。
     * 当新设置的值比原来的值小时，所有正在休眠线程将结束休眠，并重新进入休眠计时。
     * @param[in] uExpiryTimeMs 休眠退出时间
     */
    void        SetExpiryTimeMs(uint32_t uExpiryTimeMs);


private:
    static CZZThreadPool *ms_globalInstance;

    CZZThreadPoolPrivate *m;

private:
    CZZThreadPoolWork *_TakeWork();
    void    _DoWork(CWorkThread *pWorkThread);

    friend DWORD WINAPI ThreadPoolThreadProc(void *);
};

