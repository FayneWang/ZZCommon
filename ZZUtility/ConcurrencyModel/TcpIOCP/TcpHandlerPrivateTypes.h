#pragma once

#include <WinSock2.h>
#include <MSWSock.h>
#include <Windows.h>

class CBuffer;
class CTcpSessionHandlerPrivate
{
	friend class CTcpListenerHandler;
	friend class CTcpSessionHandler;
	WSABUF	wsabuf;
	CBuffer *pBuffer;
};

class CTcpListenerHandlerPrivate
{
	friend class CTcpListenerHandler;

	unsigned short				uServicePort;
 	LPFN_ACCEPTEX				fnAccepteEx;
 	LPFN_GETACCEPTEXSOCKADDRS	lpfnGetAcceptexSockAddrs;

	CTcpSessionHandler *pCurrentAcceptSession;

	CTcpListenerHandlerPrivate() :
		uServicePort(0),
		lpfnGetAcceptexSockAddrs(NULL),
		fnAccepteEx(NULL),
		pCurrentAcceptSession(NULL)
	{}
};
