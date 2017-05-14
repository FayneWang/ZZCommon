#pragma once

#include <stdint.h>
#include <ZZUtility/ConcurrencyModel/IoCompletionHandlerAbstract.h>

class CTcpSessionServerHandlerPrivate;
class CTcpSessionServerHandler : public CIoCompletionHandlerAbstract
{
private:
	friend class CTcpListenerHandler;

protected:
	CTcpSessionServerHandler();
	~CTcpSessionServerHandler();
	
	
	BOOL Create(SOCKET sListen,int iRecvBufferSize) ;


	virtual void Destroy() override;

	virtual BOOL OverlapForIOCompletion() override;


	virtual BOOL DataTransferTrigger(DWORD dwNumOfTransportBytes) override;


	virtual BOOL HandleRaiseError(DWORD dwErrorCode) override;

private:
	CTcpSessionServerHandlerPrivate *m;
};

