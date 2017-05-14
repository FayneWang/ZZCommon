#pragma once

/**
 * @file
 * @author wangzezhou <wangzezhou@163.com>
 * @date 2015-05-16
 * @brief ����ɶ˿�ģ�ͷ�װ��CIoCompletionPortModel����ʱ�Ļ�������塣
 */

#include <wtypes.h>
#include <ZZUtility/DLLDefines.h>

// /**
//  * @enum IoCompletionHandlerStatus
//  *
//  * @brief ��ɶ˿��źŴ����ࣨCIoCompletionHandlerAbstract������ǰ�Ĺ���״̬��
//  *
//  * �������������ɶ˿�ģ�ͷ�װ��CIoCompletionPortModel�ɹ��������״̬���壬���״
//  * ̬��������������ڽ��յ��ź�ʱ��һ���������ķ�ʽ��
//  */
// enum IoCompletionHandlerStatus
// {
//     ICS_OVERLAP_HANDLE,	/** @brief
// 						 * ��ʾ�ѹ����Ĵ��������ǰ�Ĳ���״̬Ӧ�ý���OVERLAPPED
// 						 * ������������OverlapForIoOperation()��ʵ�ֶ��塣
// 						 */
// 
//     ICS_LAST,			/** @brief
// 						 * CIoCompletionHandlerAbstract ���������ӹ���״̬����ʱ��
// 						 * ��һ��״̬ö��ֵ����������ֵ��ʼ���塣
// 						 */
// };

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
	 * �����������ʱ�ᱻ���õĳ�Ա������
	 *
     * �ڲ�ʵ�ֵ���CloseHandle����CIoCompletionHandlerAbstract::m_hHandle����
	 * ������������ٶ���ʱ��������Ҫ����������Դ�������ض������������ʵ�����ٴ���
	 * ���ǵñ������ض���ĺ���δβ�ٵ���һ�����������
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
	 * ��鵱ǰ�����IO�Ƿ��ѱ����٣���CIoCompletionPortModel���󱣳ֹ�����
	 *
	 * @return ��ǰ�����IO������ʱ������TRUE������FALSEʱ����CIoCompletionPortMo-
	 *		   del����ά�ֹ�����ϵ��
	 */
    virtual BOOL IsDestroyed();

    /**
     * ����������ɶ˿�ģ���з�������ʱ�����������������ɶ˿��߳��б��ص���
	 *
	 * @param[in] dwErrorCode �÷���ֵȡ���ڱ�IOCP����ʱ�Ķ���CloseHandleʱ�������
	 *						  �μ���Windows��API GetLastError()��
	 *
	 * @return ������������ص�����ʱ�������ڲ��н���OVERLAPPED���ҳɹ�ʱ������TRUE,
	 *			���򷵻�FALSE��
     */
    virtual BOOL HandleRaiseError(DWORD dwErrorCode) = 0;

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

    BOOL _IsAutoDelete();
    void _AttachIocpModel(); // ���ڶ��̰߳�ȫ�˳�
    void _DetachIocpModel(); // ���ڶ��̰߳�ȫ�˳�

    CIoCompletionHandlerAbstractPrivate *m;
};

