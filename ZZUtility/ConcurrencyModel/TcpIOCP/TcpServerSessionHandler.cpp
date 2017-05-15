
#include <ZZUtility/BufferTools/Buffer.h>
#include "TcpHandlerPrivateTypes.h"
#include "TcpServerSessionHandler.h"

CTcpServerSessionHandler::CTcpServerSessionHandler() : m(new CTcpServerSessionHandlerPrivate())
{
}

CTcpServerSessionHandler::~CTcpServerSessionHandler()
{
	closesocket(m_socket);
	m_socket = INVALID_SOCKET;
	delete m->pBuffer;
	m->pBuffer = NULL;
	delete m;
}

BOOL CTcpServerSessionHandler::Create(int iRecvBufferSize)
{
	m_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (m_socket == INVALID_SOCKET)
		return FALSE;

	m->pBuffer = new CBuffer(iRecvBufferSize);
	if (!m->pBuffer->InitBuffer())
	{
		goto error_return;
	}

	m->wsaBuf.buf = m->pBuffer->Buffer();
	m->wsaBuf.len = m->pBuffer->GetCapacity();

	return TRUE;

error_return:
	delete m->pBuffer;
	m->pBuffer = NULL;
	closesocket(m_socket);
	m_socket = INVALID_SOCKET;

	return FALSE;
}

BOOL CTcpServerSessionHandler::Send(const void *pData, int iDataSize)
{
	return send(m_socket,(const char*)pData,iDataSize,0) != SOCKET_ERROR;
}

BOOL CTcpServerSessionHandler::OverlapForIOCompletion()
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

BOOL CTcpServerSessionHandler::DataTransferTrigger(DWORD dwNumOfTransportBytes)
{
	return FALSE;
}

void CTcpServerSessionHandler::HandleRaiseError(DWORD dwErrorCode)
{
}

void CTcpServerSessionHandler::Destroy()
{
	shutdown(m_socket, SD_BOTH);
	closesocket(m_socket);
	m_socket = INVALID_SOCKET;
	delete m->pBuffer;
	m->pBuffer = NULL;
}