#pragma once

#include <ZZUtility/ConcurrencyModel/IoCompletionHandlerAbstract.h>

class CTcpIOCPHandlerPrivate;
class CTcpIOCPHandler : public CIoCompletionHandlerAbstract
{
public:
	CTcpIOCPHandler(USHORT usPort);
	~CTcpIOCPHandler();
protected:
	virtual BOOL Create() override;


	virtual BOOL OverlapForIoOperation() override;


	virtual BOOL DataTransferTrigger(DWORD dwNumOfTransportBytes) override;


	virtual BOOL HandleRaiseError(DWORD dwErrorCode) override;

private:
	CTcpIOCPHandler(CTcpIOCPHandler &){}


	CTcpIOCPHandlerPrivate *m;
};

