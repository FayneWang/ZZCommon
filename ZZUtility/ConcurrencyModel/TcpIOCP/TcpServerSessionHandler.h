#pragma once

#include <ZZUtility/DLLDefines.h>
#include <ZZUtility/ConcurrencyModel/IoCompletionHandlerAbstract.h>

class CBuffer;
class CIoCompletionPortModel;
class CTcpServerSessionHandlerPrivate;
class _ZZUTILITY_EXTERN_ CTcpServerSessionHandler : public CIoCompletionHandlerAbstract
{
private:
	friend class CTcpListenerHandler;

public:
	CTcpServerSessionHandler();
	~CTcpServerSessionHandler();	

	virtual BOOL Create(SOCKET sAccepted, int iRecvBufferSize, CIoCompletionPortModel *pIocpModel);
	BOOL Send(const void *pData,int iDataSize);

protected:
	CBuffer *GetBufferObject();

	virtual BOOL OverlapForIOCompletion() override;


	virtual void Destroy() override;

private:
	CTcpServerSessionHandlerPrivate *m;
};

