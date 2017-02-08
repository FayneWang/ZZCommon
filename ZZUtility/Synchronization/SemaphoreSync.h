#pragma once

#include <WTypes.h>
#include <ZZUtility/DLLDefines.h>

class _ZZUTILITY_EXTERN_ CSemaphoreSync
{
public:
    CSemaphoreSync();
    ~CSemaphoreSync(void);

    /**
     * 创建或打开一个信号量对象
     *
     * @param[in] lInitialCount 初始化信号总数，该必须大于或等于0，且不能小于lMaximumCount。
     * @param[in] lMaxinumCount 信号量对象的信号最大值。
     * @param[in] pSemaphoreName 信号量对象的名称，名字限制MAX_PATH长，该名称区分字母大小写。
     *
     * @return 信号创建成功时，返回0；失败时返回，GetLastError() 取得的值。
     */
    int Create(LONG lInitialCount,LONG lMaximumCount,LPCWSTR pSemaphoreName);

    /**
     * 等待信号，当对象没有信号时对象处理阻状态。对象有信号时，直接跳过，并且将对象已存在的信号减1。
     *
     * @return 当函数调用成功时，返回true；否则返回false。
     */
    bool WaitForSemaporeSignal();

    /**
     * 增加信号量信号数
     *
     * @param[in] lReleaseCount 增加的信号数
     *
     * @return 当函数调用成功时返回 ture，否则返回false。
     */
    bool IncreaseSemaphoreSignal(LONG lReleaseCount);

private:
	HANDLE m_hSemaphore;
};

