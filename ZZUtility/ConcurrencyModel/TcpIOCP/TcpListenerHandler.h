#pragma once

#include <ZZUtility/DLLDefines.h>
#include <ZZUtility/ConcurrencyModel/IoCompletionHandlerAbstract.h>


class CIoCompletionPortModel;
class CTcpServerSessionHandler;
class CTcpListenerHandlerPrivate;
class _ZZUTILITY_EXTERN_ CTcpListenerHandler : public CIoCompletionHandlerAbstract
{
public:
	class ITcpServerSessionFactory
	{
		friend class CTcpListenerHandler;
		virtual CTcpServerSessionHandler *CreateSession() = 0;
		virtual void NewSessionTrigger(CTcpServerSessionHandler *) = 0;
	};

	CTcpListenerHandler();
	~CTcpListenerHandler();

	BOOL Create(USHORT usPort, ITcpServerSessionFactory *pCreator, CIoCompletionPortModel *pAttachIocp);

protected:
	virtual BOOL OverlapForIOCompletion() override;

	virtual BOOL DataTransferTrigger(DWORD dwNumOfTransportBytes) override;

	virtual void HandleRaiseError(DWORD dwErrorCode) override;

	virtual void Destroy() override;

	ITcpServerSessionFactory *m_pServerSessionCreator;

private:
	BOOL	InitializeListernSource(USHORT usPort);

	CTcpListenerHandlerPrivate *m;
};


