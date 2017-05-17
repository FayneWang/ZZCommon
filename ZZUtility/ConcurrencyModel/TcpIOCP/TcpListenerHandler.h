#pragma once

#include <ZZUtility/DLLDefines.h>
#include <ZZUtility/ConcurrencyModel/IoCompletionHandlerAbstract.h>


class CIoCompletionPortModel;
class CTcpServerSessionHandler;
class CTcpListenerHandlerPrivate;
class _ZZUTILITY_EXTERN_ CTcpListenerHandler : public CIoCompletionHandlerAbstract
{
public:
	class INewConnectionCallback
	{
		friend class CTcpListenerHandler;
		virtual void NewConnectionTrigger(SOCKET sAccepted) = 0;
	};

	CTcpListenerHandler();
	~CTcpListenerHandler();

	/**
	 * ����TcpListener�����ڲ�ʹ����Դ����ֱ�ӽ�listen socket������ CIoCompletionPortModel* �����ڵ� IOCP ����С�	 *
	 *
	 * @param[in] usPort TCP listen socket ���ӵĶ˿ڣ���ֵΪ0ʱ��listen �˿���ϵͳ�Զ����䡣
	 *
	 * @param[in] pCreator CTcpServerSesionHandler����������ִ��Createʱ�����ȵ���CreateSession()���� CTcpServerSesionHandler
	 *						���������ȴ�TCP�ͻ��˽������ӡ�ÿ�����µ�TCP���ӵ���ʱ����ص��������������ٻص� NewSessionTrigger(
	 *						CTcpServerSessionHandler *pNewSession)������pNewSessionΪ�����ӵĻỰ����
	 *
	 * @param[in] pAttachIocp CTcpListenerHandler���������������ɶ˿�ģ�ͣ�CIoCompletionPortModel������
	 *
	 * @return �����ɹ�ʱ����TRUE�����򷵻�FALSE.
	 */
	BOOL Create(USHORT usPort, INewConnectionCallback *pCreator, CIoCompletionPortModel *pAttachIocp);

	virtual void Destroy() override;

	/**
	 * ��ȡ�ȴ�Tcp���ӵļ����˿ڡ�
	 * �������������Create(...)���óɹ�֮�����Ч��
	 *
	 * @return ���óɹ�ʱ������Tcp���ӵļ����˿ڣ����򷵻�0��
	 */
	USHORT GetListenPort();
protected:
	virtual BOOL OverlapForIOCompletion() override;

	virtual BOOL DataTransferTrigger(DWORD dwNumOfTransportBytes) override;

	virtual void HandleRaiseError(DWORD dwErrorCode) override;


	INewConnectionCallback *m_pNewConnectCallback;

private:
	BOOL	InitializeListernSource(USHORT usPort);

	CTcpListenerHandlerPrivate *m;
};


