
#include <ZZUtility/BufferTools/Buffer.h>
#include "TcpHandlerPrivateTypes.h"
#include "TcpSessionHandler.h"



CTcpSessionHandler::CTcpSessionHandler() : m(new CTcpSessionHandlerPrivate())
{
}

CTcpSessionHandler::~CTcpSessionHandler()
{
	delete m->pBuffer;
	delete m;
}

BOOL CTcpSessionHandler::Create(SOCKET sListen, int iRecvBufferSize)
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

// 	if (!AcceptEx(sListen, m_socket, m->wsabuf.buf, m->wsabuf.len - (sizeof(struct sockaddr_in) + 16) * 2,
// 		sizeof(struct sockaddr_in) + 16, sizeof(struct sockaddr_in) + 16, // +16 详见 AcceptEx 函数参数 dwLocalAddressLength 和 dwRemoteAddressLength 
// 		NULL, IocpAsyncOverlap()))
// 	{
// 		goto error_return;
// 	}

	return TRUE;

error_return:
	delete m->pBuffer;
	m->pBuffer = NULL;
	closesocket(m_socket);
	m_socket = INVALID_SOCKET;

	return FALSE;
}

BOOL CTcpSessionHandler::OverlapForIOCompletion()
{
	return FALSE;
}

BOOL CTcpSessionHandler::DataTransferTrigger(DWORD dwNumOfTransportBytes)
{
	return FALSE;
}

BOOL CTcpSessionHandler::HandleRaiseError(DWORD dwErrorCode)
{
	return FALSE;
}
