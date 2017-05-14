#include <WinSock2.h>
#include <MSWSock.h>
#include <ZZUtility/BufferTools/Buffer.h>
#include "TcpSessionClientHandler.h"

class CTcpSessionClientHandlerPrivate
{
	friend class CTcpSessionClientHandler;

	CBuffer *pBuffer;
	WSABUF wsaBuf;
};

CTcpSessionClientHandler::CTcpSessionClientHandler() : m(new CTcpSessionClientHandlerPrivate())
{
}

CTcpSessionClientHandler::~CTcpSessionClientHandler()
{
	delete m;
}

CTcpSessionClientHandler * CTcpSessionClientHandler::NewConnection(const std::string &strHost, USHORT usPort)
{
	if (strHost.empty() || usPort == 0)
		return NULL;

	SOCKET sConnect = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (INVALID_SOCKET == sConnect)
		return NULL;

	SOCKADDR_IN  serverAddr = { 0 };
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(usPort);
	serverAddr.sin_addr.s_addr = INADDR_ANY;
	
	CBuffer *pBuffer = new CBuffer();
	if (!pBuffer->InitBuffer())
	{
		delete pBuffer;
		closesocket(sConnect);
		return NULL;
	}

	if (connect(sConnect, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
	{
		delete pBuffer;
		closesocket(sConnect);
		return NULL;
	}

	CTcpSessionClientHandler *pHandler = new CTcpSessionClientHandler();
	pHandler->m->pBuffer = pBuffer;
	pHandler->m->wsaBuf.buf = pBuffer->Buffer();
	pHandler->m->wsaBuf.len = pBuffer->GetCapacity();

	return pHandler;	
}

void CTcpSessionClientHandler::Destroy()
{
	EnterThread();

	closesocket(m_socket);
	m_socket = INVALID_SOCKET;
	delete m->pBuffer;
	m->pBuffer = NULL;

	LeaveThread();
}

BOOL CTcpSessionClientHandler::OverlapForIOCompletion()
{
	return WSARecv(m_socket, &m->wsaBuf, 1, NULL,0, IocpAsyncOverlap(),NULL ) == WSA_IO_PENDING;
}

BOOL CTcpSessionClientHandler::DataTransferTrigger(DWORD dwNumOfTransportBytes)
{
	return FALSE;
}

BOOL CTcpSessionClientHandler::HandleRaiseError(DWORD dwErrorCode)
{
	return FALSE;
}