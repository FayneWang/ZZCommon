#pragma once

/**
 * @file
 * @author wangzezhou <wangzezhou@163.com>
 * @date 2015-05-16
 * @brief ����ɶ˿�ģ�ͷ�װ��CIoCompletionPortModel����ʱ�Ļ�������塣
 */

#include <wtypes.h>
#include <ZZUtility/DLLDefines.h>

/**
 * @class CIoCompletionHandlerAbstract
 *
 * @breif ��ɶ˿ڲ���ģ���е�IO�źŴ���ʱ����Ӧ�ź���Ӧ������Ķ�����Ļ��ࡣ
 *
 * ��������ඨ��������ɶ˿ڲ���ģ��CIoCompletionPortModel��������ʱ��ʹ�õĳ���
 * ���ͣ����ж����˴���ӿڡ����ض��崿�ӿں�ͨ�õĹ����ӿڡ�������صĸ����ӿڽ��ܣ�
 * ��鿴��������ϸ˵����
 *
 */

class CIoCompletionHandlerAbstractPrivate;
class _ZZUTILITY_EXTERN_ CIoCompletionHandlerAbstract
{
    friend class CIoCompletionPortModel;
public:
    CIoCompletionHandlerAbstract(void);
    virtual ~CIoCompletionHandlerAbstract(void) = 0;

	/**
	 * ���ô��������IoCompletionPortModel����ɹ�Detach()֮���Զ����١�
	 * ֻ�е�����ɹ���IoCompletionPortModel����Attach()ʱ�������ò���Ч��
	 *
	 * @warning ���������ʹ��new�����Ķ������ʹ�øú�������ΪTRUE��
	 * 
	 * @param[in] bAutoDelete AutoDeleteĬ��ΪFALSE��������AutoDeleteΪTRUEʱ����
	 *	����ɶ˿�ģ�Ͷ���Detach()֮��ǰ�����Զ�delete��
	 */
    void SetAutoDelete(BOOL bAutoDelete);

protected:  // protected function
    OVERLAPPED *IocpAsyncOverlap();

	void	EnterThread();
	void	LeaveThread();

protected:  // protected member.

	/// ������ɶ˿�ʱʹ�õĿ�overlap�ľ����
	union
	{
		HANDLE	m_hHandle;	///<	file handle
		SOCKET	m_socket;	///<	network socket 
	};			

// 	/// ������ǵ�ǰ����Ĺ���״̬�Ķ���, @see {IoCompletionHandlerStatus}��
//     int             m_iIoCompletionStatus; 


protected: // virtual function
    
    /**
	 * ���ٶ�����ڲ���Դ��
	 *
	 * ʹ�� CIoCompletionPortModel ����� void DetachHandler(CIoCompletionHandlerAbstract *)�������ʵ������ʱ��������������
	 * �����ڲ�����ʵ������ m_hHandle �� m_socket ������������ֵΪ INVALID_HANDLE_VALUE �� INVALID_SOCKET����ȷ�������������ʱ����쳣��
     * ��������ʵ��Ϊ������ģ������������ʱ����쳣��
     */
    virtual void Destroy() = 0;

    /**
     * ��Ҫ�����Ա����m_hHandleʹ�õ���Դ����ɶ˿ھ�������ص������ݡ�
     */
    virtual BOOL OverlapForIOCompletion() = 0;

	/**
	 * �����CIoCompletionHandlerAbstract::m_iIoCompletionStatus����������
	 * ��ICS_OVERLAP_HANDLE״̬ʱ�� CIoCompletionHandlerAbstract::m_hHandle���ź�
	 * ������ʱ�ĵ��ú�����
	 *
	 * @param[in] dwNumOfTransferedBytes CIoCompletionHandlerAbstract::m_hHandle
	 *	�źŴ���ʱCIoCompletionHandlerAbstract::m_pMemoryBuffer���յ���������
	 *
	 * @return �����һ�������ж�CIoCompletionHandlerAbstract::m_hHandle����overlap
	 * ʧ��ʱ����FALSE���ɹ�ʱ����TRUE.
	 *
	 */
    virtual BOOL DataTransferTrigger(DWORD dwNumOfTransportBytes) = 0;

    /**
     * ����������ɶ˿�ģ���з�������ʱ�����������������ɶ˿��߳��б��ص���
	 *
	 * @param[in] dwErrorCode �÷���ֵȡ���ڱ�IOCP����ʱ�Ķ���CloseHandleʱ�������
	 *						  �μ���Windows��API GetLastError()��
	 *
	 * @return ������������ص�����ʱ�������ڲ��н���OVERLAPPED���ҳɹ�ʱ������TRUE,
	 *			���򷵻�FALSE��
     */
	virtual void HandleRaiseError(DWORD dwErrorCode){};

	/**
	 * ������󱻳ɹ�CIoCompletionPortModel::Attachʱ�����⺯����ȷ�����󱻳ɹ�����
	 * ���٣���֤����ȫ��
	 *
	 * ����������������������У������ٶ���ǰ��������������Ա�֤������̰߳�ȫ��
	 */
    void WaitDetachIocpModel();

private:
	/**
	 * ��ֹ����ʵ����
	 */
	CIoCompletionHandlerAbstract(CIoCompletionHandlerAbstract &){}

    void _AttachIocpModel(); // ���ڶ��̰߳�ȫ�˳�
    void _DetachIocpModel(); // ���ڶ��̰߳�ȫ�˳�

    CIoCompletionHandlerAbstractPrivate *m;
};

