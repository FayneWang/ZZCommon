#include <WinSock2.h>
#include <MSWSock.h>
#include <ZZUtility/BufferTools/Buffer.h>
#include <ZZUtility/ConcurrencyModel/IoCompletionPortModel.h>

#include "TcpClientSessionHandler.h"

class CTcpClientSessionHandlerPrivate
{
	friend class CTcpClientSessionHandler;

	CBuffer *pBuffer;
	WSABUF wsaBuf;
};

CTcpClientSessionHandler::CTcpClientSessionHandler() : m(new CTcpClientSessionHandlerPrivate())
{
}

CTcpClientSessionHandler::~CTcpClientSessionHandler()
{
	Destroy();
	delete m;
}

BOOL CTcpClientSessionHandler::Connect(const std::string &strHost, USHORT usPort, CIoCompletionPortModel *pAttachIocp)
{
	if (strHost.empty() || usPort == 0 || pAttachIocp == NULL)
		return FALSE;

	m_socket  = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (INVALID_SOCKET == m_socket)
		goto error_exit;

	{
		SOCKADDR_IN  serverAddr = { 0 };
		serverAddr.sin_family = AF_INET;
		serverAddr.sin_port = htons(usPort);
		serverAddr.sin_addr.s_addr = inet_addr(strHost.c_str());
		if (connect(m_socket, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
			goto error_exit;
	}

	m->pBuffer = new CBuffer();
	if (!m->pBuffer->InitBuffer())
	{
		delete m->pBuffer;
		closesocket(m_socket);
		m_socket = INVALID_SOCKET;
		return FALSE;
	}
	m->wsaBuf.buf = m->pBuffer->Buffer();
	m->wsaBuf.len = m->pBuffer->GetCapacity();

	if (!pAttachIocp->AttachHandler(this))
		goto error_exit;

	return TRUE;	

error_exit:

	closesocket(m_socket);
	m_socket = INVALID_SOCKET;
	delete m->pBuffer;
	m->pBuffer = NULL;

	return FALSE;
}

BOOL CTcpClientSessionHandler::Send(const void *pData, int pDataSize)
{
	return send(m_socket, (const char*)pData, pDataSize, 0) != SOCKET_ERROR;
}

BOOL CTcpClientSessionHandler::OverlapForIOCompletion()
{
	DWORD dwFlags = 0;
	if (WSARecv(m_socket, &m->wsaBuf, 1, NULL, &dwFlags, IocpAsyncOverlap(), NULL) != SOCKET_ERROR)
	{
		return TRUE;
	}
	else
	{
		return WSAGetLastError() == WSA_IO_PENDING;
	}

	return FALSE;
}

BOOL CTcpClientSessionHandler::DataTransferTrigger(DWORD dwNumOfTransportBytes)
{
	return FALSE;
}

void CTcpClientSessionHandler::HandleRaiseError(DWORD dwErrorCode)
{
}

void CTcpClientSessionHandler::Destroy()
{
	shutdown(m_socket, SD_BOTH);
	closesocket(m_socket);
	m_socket = INVALID_SOCKET;
	delete m->pBuffer;
	m->pBuffer = NULL;
}
