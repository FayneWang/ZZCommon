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

//class CIoCompletionPortModel;
class CTcpListenerHandlerPrivate
{
	friend class CTcpListenerHandler;

	char						szAcceptBuffer[(sizeof(SOCKADDR_IN) + 16)*2];
	SOCKET						sAccept;
 	LPFN_ACCEPTEX				fnAccepteEx;

	CTcpListenerHandlerPrivate() :
		sAccept(INVALID_SOCKET),
		fnAccepteEx(NULL)
	{}
};
