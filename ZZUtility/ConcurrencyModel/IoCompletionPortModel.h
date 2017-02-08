#pragma once

/**
 * @file
 * @author wangzezhou <wangzezhou@163.com>
 * @date 2015-05-19
 *
 * @brief Windows��ɶ˿�ģ�͵ķ�װ�࣬��@see ��CIoCompletionHandlerAbstract����
 * һ��ʹ�á�
 *
 */

#include <set>
#include <wtypes.h>
#include <ZZUtility/DLLDefines.h>

/**
 * @class CIoCompletionPortModel
 *
 * @breif ��ɶ˿ڲ���ģ�ͷ�װ�࣬ʹ�ø�����Ա�����ɶ˿�ģ�͵ķ�������
 *
 * �����ʵ����Windows��ɶ�ģ�͵ķ�װ����@see ��CIoCompletionHandlerAbstract������
 * ��һ��ʹ�á�����һ�������࣬�ṩ�˴��ɶ˿�ʱ������Ҫ�����ۣ��ɿ�����ɶ˿�ʱ
 * ��Ҫʹ�õĽӿڷ�װ����@see ��CIoCompletionHandlerAbstract�����ж������ȶ�����׳
 * �Ĵ����߼��ӿڡ�
 */
class CIoCompletionHandlerAbstract;
class CIoCompletionPortModelPrivate;
class _ZZUTILITY_EXTERN_ CIoCompletionPortModel
{
    friend DWORD WINAPI IocpConcurrentProc(LPVOID lpParameter);

public:
	HANDLE	GetHandle();
    
    /**
     * ����һ����overlaped����Դ����Ӧ������ɶ˿ڲ���ģ���С�
	 *
	 * @param[in] pHandler �����@see {CIoCompletionHandlerAbstract}
	 * 
	 * @return �����ɹ�ʱ������TRUE.���򷵻�FALSE��
     */
    BOOL AttachHandler(CIoCompletionHandlerAbstract *pHandler);
    
    /**
     * ����һ����overlapped����Դ����Ӧ������ɶ˿ڲ���ģ���С�
	 *
	 * @param[in] pHandler �����@see {CIoCompletionHandlerAbstract}
	 * 
	 * @return �����ɹ�ʱ������TRUE.���򷵻�FALSE��
     */
    void DetachHandler(CIoCompletionHandlerAbstract *pHandler);
    
    /**
     * ����һ����ɶ˿��źţ�ʹ@see {CIoCompletionHandlerAbstract}��������ɶ˿�ģ��
	 * �߳��д��� CIoCompletionHandlerAbstract ����ĵ�ǰ״̬����Ҫ���еĹ�����
	 * 
	 * @warning δȷ��@see {CIoCompletionHandlerAbstract}�����Ƿ���Ҫ����ɶ˿ڽ���
	 * ����������ʹ�øú������á�������û���ж�ҪPostHandler�Ķ����Ƿ񱻹���״̬��
     */
    BOOL PostHandlerStatus(CIoCompletionHandlerAbstract *pHandler);
    
    /**
     * ���������Ļ�ȡ�ӿڡ�������Ӧ�ó����У���ʹ���ڶ಻ͬ�Ķ˿���Ҫ�����ģ�Ҳ��
	 * ��Ҫһ����ɶ˿�ģ�Ͷ���������
	 * 
	 * @return ��ɶ˿�ģ�͵ĵ�������
     */
    static CIoCompletionPortModel *Instance();
    
    /**
     * ʹ�øýӿ����ֶ����ٵ�������
     */
    static void Uninstance();

private:
    struct THREAD_DATA
    {
        CIoCompletionPortModel *pIocpModel;
        HANDLE                  hThread;
    };

private:
    CIoCompletionPortModel(const CIoCompletionPortModel &);
    CIoCompletionPortModel(void);
    ~CIoCompletionPortModel(void);

private:
	void _ThreadHanleProc(HANDLE hThread);
	BOOL _IsExitProcessThread(CIoCompletionHandlerAbstract *pCompletionHandler);
    void _IocpProc();
    CIoCompletionPortModelPrivate *m;

    static CIoCompletionPortModel *s_pSinglton;
};

