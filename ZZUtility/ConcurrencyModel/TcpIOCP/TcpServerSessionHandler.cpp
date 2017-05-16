
#include <ZZUtility/BufferTools/Buffer.h>
#include "TcpHandlerPrivateTypes.h"
#include "../IoCompletionPortModel.h"
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

BOOL CTcpServerSessionHandler::Create(SOCKET sAccepted, int iRecvBufferSize, CIoCompletionPortModel *pIocpModel)
{
	if (sAccepted == INVALID_SOCKET || sAccepted ==0)
		return FALSE;

	m_socket = sAccepted;

	if (iRecvBufferSize <= 0)
		iRecvBufferSize = BUFER_DEFAULT_SIZE;

	m->pBuffer = new CBuffer(iRecvBufferSize);
	if (!m->pBuffer->InitBuffer())
	{
		goto error_return;
	}

	m->wsaBuf.buf = m->pBuffer->Buffer();
	m->wsaBuf.len = m->pBuffer->GetCapacity();
	if (!pIocpModel->AttachHandler(this))
		goto error_return;

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

CBuffer *CTcpServerSessionHandler::GetBufferObject()
{
	return m->pBuffer;
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

void CTcpServerSessionHandler::Destroy()
{
	if (m_socket != INVALID_SOCKET)
	{
		shutdown(m_socket, SD_BOTH);
		closesocket(m_socket);
		m_socket = INVALID_SOCKET;
		delete m->pBuffer;
		m->pBuffer = NULL;
	}
}