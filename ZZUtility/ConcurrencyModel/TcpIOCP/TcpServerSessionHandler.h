#pragma once

#include <stdint.h>
#include <ZZUtility/DLLDefines.h>
#include <ZZUtility/ConcurrencyModel/IoCompletionHandlerAbstract.h>

class CTcpServerSessionHandlerPrivate;
class _ZZUTILITY_EXTERN_ CTcpServerSessionHandler : public CIoCompletionHandlerAbstract
{
private:
	friend class CTcpListenerHandler;

public:
	CTcpServerSessionHandler();
	~CTcpServerSessionHandler();	
	
	BOOL Create(int iRecvBufferSize) ;

	BOOL Send(const void *pData,int iDataSize);
protected:

	virtual BOOL OverlapForIOCompletion() override;


	virtual BOOL DataTransferTrigger(DWORD dwNumOfTransportBytes) override;


	virtual void HandleRaiseError(DWORD dwErrorCode) override;


	virtual void Destroy() override;

private:
	CTcpServerSessionHandlerPrivate *m;
};

