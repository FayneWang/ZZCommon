
#include <ZZUtility/BufferTools/Buffer.h>
#include "TcpHandlerPrivateTypes.h"
#include "TcpSessionServerHandler.h"



CTcpSessionServerHandler::CTcpSessionServerHandler() : m(new CTcpSessionServerHandlerPrivate())
{
}

CTcpSessionServerHandler::~CTcpSessionServerHandler()
{
	delete m;
}

BOOL CTcpSessionServerHandler::Create(SOCKET sListen, int iRecvBufferSize)
{
	m_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (m_socket == INVALID_SOCKET)
		return FALSE;

	m->pBuffer = new CBuffer(iRecvBufferSize);
	if (!m->pBuffer->InitBuffer())
	{
		goto error_return;
	}

	m->wsabuf.buf = m->pBuffer->Buffer();
	m->wsabuf.len = m->pBuffer->GetCapacity();

	return TRUE;

error_return:
	delete m->pBuffer;
	m->pBuffer = NULL;
	closesocket(m_socket);
	m_socket = INVALID_SOCKET;

	return FALSE;
}

void CTcpSessionServerHandler::Destroy()
{
	closesocket(m_socket);
	m_socket = INVALID_SOCKET;
	delete m->pBuffer;
	m->pBuffer = NULL;
}

BOOL CTcpSessionServerHandler::OverlapForIOCompletion()
{
	return FALSE;
}

BOOL CTcpSessionServerHandler::DataTransferTrigger(DWORD dwNumOfTransportBytes)
{
	return FALSE;
}

BOOL CTcpSessionServerHandler::HandleRaiseError(DWORD dwErrorCode)
{
	return FALSE;
}
