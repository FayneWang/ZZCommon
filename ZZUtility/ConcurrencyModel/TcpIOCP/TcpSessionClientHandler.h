#pragma once

#include <string>
#include <ZZUtility/DLLDefines.h>
#include <ZZUtility/ConcurrencyModel/IoCompletionHandlerAbstract.h>

class CTcpSessionClientHandlerPrivate;
class _ZZUTILITY_EXTERN_ CTcpSessionClientHandler : public CIoCompletionHandlerAbstract
{
public:
	static CTcpSessionClientHandler *NewConnection(const std::string &strHost,USHORT uShort);

protected:
	CTcpSessionClientHandler();
	~CTcpSessionClientHandler();

	virtual BOOL OverlapForIOCompletion() override;

	virtual BOOL DataTransferTrigger(DWORD dwNumOfTransportBytes) override;

	virtual BOOL HandleRaiseError(DWORD dwErrorCode) override;

	virtual void Destroy() override;

private:
	CTcpSessionClientHandlerPrivate *m;
	
};

