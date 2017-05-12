#pragma once

#include <ZZUtility/DLLDefines.h>
#include <ZZUtility/ConcurrencyModel/IoCompletionHandlerAbstract.h>

class CTcpListenerHandlerPrivate;
class _ZZUTILITY_EXTERN_ CTcpListenerHandler : public CIoCompletionHandlerAbstract
{
public:
	static CTcpListenerHandler *CreateAndAttachToIocp(USHORT sPort);

protected:
	CTcpListenerHandler();
	~CTcpListenerHandler();

	virtual void Destroy() override;

	virtual BOOL IsDestroyed() override;

	virtual BOOL OverlapForIOCompletion() override;

	virtual BOOL DataTransferTrigger(DWORD dwNumOfTransportBytes) override;

	virtual BOOL HandleRaiseError(DWORD dwErrorCode) override;

private:
	BOOL _InitSessionForAccept();

	CTcpListenerHandlerPrivate *m;
};


