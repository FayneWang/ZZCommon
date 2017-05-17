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
	 * 创建TcpListener对象内部使用资源，并直接将listen socket关联到 CIoCompletionPortModel* 对象内的 IOCP 句柄中。	 *
	 *
	 * @param[in] usPort TCP listen socket 连接的端口，当值为0时。listen 端口由系统自动分配。
	 *
	 * @param[in] pCreator CTcpServerSesionHandler创建工厂。执行Create时，会先调用CreateSession()创建 CTcpServerSesionHandler
	 *						对象用来等待TCP客户端进行连接。每当有新的TCP连接到达时，会回调这个参数对象会再回调 NewSessionTrigger(
	 *						CTcpServerSessionHandler *pNewSession)，其中pNewSession为新连接的会话对象。
	 *
	 * @param[in] pAttachIocp CTcpListenerHandler类对象所关联的完成端口模型（CIoCompletionPortModel）对象。
	 *
	 * @return 创建成功时返回TRUE，否则返回FALSE.
	 */
	BOOL Create(USHORT usPort, INewConnectionCallback *pCreator, CIoCompletionPortModel *pAttachIocp);

	virtual void Destroy() override;

	/**
	 * 获取等待Tcp连接的监听端口。
	 * 这个函数必须在Create(...)调用成功之后才有效。
	 *
	 * @return 调用成功时，返回Tcp连接的监听端口，否则返回0。
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


