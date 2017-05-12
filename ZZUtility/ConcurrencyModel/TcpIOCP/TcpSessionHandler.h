#pragma once

#include <stdint.h>
#include <ZZUtility/ConcurrencyModel/IoCompletionHandlerAbstract.h>

class CTcpSessionHandlerPrivate;
class CTcpSessionHandler : public CIoCompletionHandlerAbstract
{
private:
	friend class CTcpListenerHandler;

protected:
	CTcpSessionHandler();
	~CTcpSessionHandler();
	
	virtual BOOL Create(SOCKET sListen,int iRecvBufferSize);

	virtual BOOL OverlapForIOCompletion() override;


	virtual BOOL DataTransferTrigger(DWORD dwNumOfTransportBytes) override;


	virtual BOOL HandleRaiseError(DWORD dwErrorCode) override;

private:
	CTcpSessionHandlerPrivate *m;
};

