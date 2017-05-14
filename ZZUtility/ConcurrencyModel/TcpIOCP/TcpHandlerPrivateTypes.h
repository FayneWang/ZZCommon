#pragma once

#include <WinSock2.h>
#include <MSWSock.h>
#include <Windows.h>

class CBuffer;
class CTcpSessionServerHandlerPrivate
{
	friend class CTcpListenerHandler;
	friend class CTcpSessionServerHandler;
	WSABUF	wsabuf;
	CBuffer *pBuffer;
};

class CTcpListenerHandlerPrivate
{
	friend class CTcpListenerHandler;

	unsigned short				uServicePort;
 	LPFN_ACCEPTEX				fnAccepteEx;
 	LPFN_GETACCEPTEXSOCKADDRS	lpfnGetAcceptexSockAddrs;

	CTcpSessionServerHandler *pAcceptSession;

	CTcpListenerHandlerPrivate() :
		uServicePort(0),
		lpfnGetAcceptexSockAddrs(NULL),
		fnAccepteEx(NULL),
		pAcceptSession(NULL)
	{}
};
