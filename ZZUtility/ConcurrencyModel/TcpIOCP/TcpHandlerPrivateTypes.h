#pragma once

#include <WinSock2.h>
#include <MSWSock.h>
#include <Windows.h>

class CBuffer;
class CTcpServerSessionHandlerPrivate
{
	friend class CTcpListenerHandler;
	friend class CTcpServerSessionHandler;
	WSABUF	wsaBuf;
	CBuffer *pBuffer;
};

class CIoCompletionPortModel;
class CTcpListenerHandlerPrivate
{
	friend class CTcpListenerHandler;

	CIoCompletionPortModel		*pIocpModel;
	CTcpServerSessionHandler	*pAcceptSession;

 	LPFN_ACCEPTEX				fnAccepteEx;

	CTcpListenerHandlerPrivate() :
		pIocpModel(0),
		pAcceptSession(NULL),
		fnAccepteEx(NULL)
	{}
};
