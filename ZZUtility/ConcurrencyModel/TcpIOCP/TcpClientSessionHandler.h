#pragma once

#include <string>
#include <ZZUtility/DLLDefines.h>
#include <ZZUtility/ConcurrencyModel/IoCompletionHandlerAbstract.h>

class CIoCompletionPortModel;
class CTcpClientSessionHandlerPrivate;
class _ZZUTILITY_EXTERN_ CTcpClientSessionHandler : public CIoCompletionHandlerAbstract
{
public:
	CTcpClientSessionHandler();
	~CTcpClientSessionHandler();

	BOOL Connect(const std::string &strHost, USHORT usPort, CIoCompletionPortModel *pAttachIocp);

	BOOL Send(const void *pData,int pDataSize);
protected:

	virtual BOOL OverlapForIOCompletion() override;

	virtual BOOL DataTransferTrigger(DWORD dwNumOfTransportBytes) override;

	virtual void HandleRaiseError(DWORD dwErrorCode) override;


	virtual void Destroy() override;

private:
	CTcpClientSessionHandlerPrivate *m;
	
};

