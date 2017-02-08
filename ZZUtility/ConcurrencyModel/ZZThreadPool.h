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
     * ���һ�����������̳߳��С�
     * @param[in] pWork һ��Ҫִ�й����Ķ���
     * @return ��������ɹ���ӵ��̳߳��л������У�����TRUE,���򷵻�FALSE��
     */
    BOOL        PushWork(CZZThreadPoolWork *pWork);

    /**
     * �ӹ����������Ƴ���δִ�еĹ�����
     * @param[in] pWork Ҫ�Ƴ��Ĺ�������
     * @return ����ɹ��ӹ���������ɾ���������󷵻�TRUE�����򷵻�FALSE��
     *         �ӹ����������Ƴ�ɾ������ʧ�ܣ��п����ǹ����ѿ�ʼִ�л���ִ�н�����
     */
    BOOL        RemoveWork(CZZThreadPoolWork *pWork);

    /**
     * �������δִ�еĹ������󣬲��ȴ�����ִ�еĹ�������ִ����ɲ����˳����н��̡�
     */
    void        ClearAndWait();

    /**
     * ��ȡ�����̳߳����Ĺ����߳���
     * @return �����̳߳��������Ĺ����߳���
     */
    uint16_t    GetMaxThreadCount() const;

    /**
     * ���������������߳���
     * 1��������߳���С�ڵ�ǰ����С�߳���ʱ����С�߳������ͷų������̡߳�
     * 2�������ó����궨���ENTP_MAXNIMUM_THREAD_NUMֵʱ���������Ĺ����߳���Ϊ ENTP_MAXNIMUM_THREAD_NUM
     *    ����ǰֵΪ��500�������ֵ��
     *
     * @param[in]   uMaxThreadCount �����̳߳����Ĺ����߳�����
     * @return      ����ʵ�����������߳�����
     */
    uint16_t    SetMaxThreadCount(uint16_t uMaxThreadCount);

    /**
     * ��ȡ��������ٹ����߳���
     * @return ������������ٵĹ����߳���
     */
    uint16_t    GetMinThreadCount() const;

    /**
     * ������������ٹ����߳���
     * �����õ������߳����������������߳���ʱ�����������߳���Ϊ���Ĺ����߳��� GetMaxThreadCount()��
     * @param[in] uMinThreadCount ��������ٹ����߳���
     * @return ʵ���������С�߳���
     */
    uint16_t    SetMinThreadCount(uint16_t uMinThreadCount);

    /**
     * ��ǰ�̳߳��д��ڵĹ����߳���(�������ڹ������������ߵ��߳�)��
     * return ��ǰ���̳߳��е��߳���
     */
    uint16_t    GetActiveThreadCount() const;

    /**
     * ��ȡ�߳������˳�ʱ��(ms)ֵ��
     * return �߳������˳�ʱ��(ms)��
     */
    uint32_t    GetExpiryTimeMs() const;

    /**
     * �����߳���ɹ������������ߵȴ�������ȴ���ʱ�˳���ʱ��(ms)ֵ��
     * �������õ�ֵ��ԭ����ֵСʱ���������������߳̽��������ߣ������½������߼�ʱ��
     * @param[in] uExpiryTimeMs �����˳�ʱ��
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

