#pragma once

#include <WTypes.h>
#include <ZZUtility/DLLDefines.h>

class _ZZUTILITY_EXTERN_ CSemaphoreSync
{
public:
    CSemaphoreSync();
    ~CSemaphoreSync(void);

    /**
     * �������һ���ź�������
     *
     * @param[in] lInitialCount ��ʼ���ź��������ñ�����ڻ����0���Ҳ���С��lMaximumCount��
     * @param[in] lMaxinumCount �ź���������ź����ֵ��
     * @param[in] pSemaphoreName �ź�����������ƣ���������MAX_PATH����������������ĸ��Сд��
     *
     * @return �źŴ����ɹ�ʱ������0��ʧ��ʱ���أ�GetLastError() ȡ�õ�ֵ��
     */
    int Create(LONG lInitialCount,LONG lMaximumCount,LPCWSTR pSemaphoreName);

    /**
     * �ȴ��źţ�������û���ź�ʱ��������״̬���������ź�ʱ��ֱ�����������ҽ������Ѵ��ڵ��źż�1��
     *
     * @return ���������óɹ�ʱ������true�����򷵻�false��
     */
    bool WaitForSemaporeSignal();

    /**
     * �����ź����ź���
     *
     * @param[in] lReleaseCount ���ӵ��ź���
     *
     * @return ���������óɹ�ʱ���� ture�����򷵻�false��
     */
    bool IncreaseSemaphoreSignal(LONG lReleaseCount);

private:
	HANDLE m_hSemaphore;
};

