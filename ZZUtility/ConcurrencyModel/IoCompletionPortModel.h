#pragma once

/**
 * @file
 * @author wangzezhou <83209400@qq.com>
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
* @breif Windows IO Completion Model(��ɶ˿�)ģ�ͷ�װ�࣬������һ������ʵ�֡�
*
* ������ȫ�̰߳�ȫ����ʵ����@see ��CIoCompletionHandlerAbstract��������һ��Э��ʹ�á�
* �ڲ��������С�����߳����Ĵ����������Ǹ��ݹ�����@see ��CIoCompletionHandlerAbstract��ʵ��������
* CPU���ں����������Զ����ơ�
*
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
    struct ThreadData
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

