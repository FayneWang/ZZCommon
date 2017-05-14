
#include <WinSock2.h>
#include <mswsock.h>
#include "../IoCompletionPortModel.h"
#include "TcpHandlerPrivateTypes.h"
#include "TcpSessionServerHandler.h"
#include "TcpListenerHandler.h"


CTcpListenerHandler::CTcpListenerHandler(void) : m(new CTcpListenerHandlerPrivate())
{
}

CTcpListenerHandler::~CTcpListenerHandler(void)
{
	WaitDetachIocpModel();

	delete m;
}

CTcpListenerHandler * CTcpListenerHandler::CreateAndAttachToIocp(USHORT sPort)
{
	SOCKET sListenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (INVALID_SOCKET == sListenSocket)
		return NULL;

	struct sockaddr_in sockAddrIn = { 0 };
	sockAddrIn.sin_family = AF_INET;
	sockAddrIn.sin_addr.s_addr = ADDR_ANY;
	sockAddrIn.sin_port = htons(sPort);
	if (bind(sListenSocket, (struct sockaddr*)&sockAddrIn, sizeof(sockAddrIn)) != 0)
	{
		closesocket(sListenSocket);
		sListenSocket = INVALID_SOCKET;
		return NULL;
	}

	if (listen(sListenSocket, SOMAXCONN) != 0)
	{
		closesocket(sListenSocket);
		sListenSocket = INVALID_SOCKET;
		return FALSE;
	}

	GUID	guidAcceptEx = WSAID_ACCEPTEX;
	DWORD	dwFunPointerSize = 0;
	LPFN_ACCEPTEX lpfnAcceptEx;
	if (WSAIoctl(sListenSocket, SIO_GET_EXTENSION_FUNCTION_POINTER,
		&guidAcceptEx, sizeof(guidAcceptEx), &lpfnAcceptEx,
		sizeof(LPFN_ACCEPTEX), &dwFunPointerSize, nullptr, nullptr) != 0)
	{
		closesocket(sListenSocket);
		sListenSocket = INVALID_SOCKET;
		return NULL;
	}

	CTcpListenerHandler *pListener = new CTcpListenerHandler();
	pListener->m_socket = sListenSocket;
	pListener->m->fnAccepteEx = lpfnAcceptEx;

	return pListener;
}

BOOL CTcpListenerHandler::OverlapForIOCompletion()
{
	if (m->pAcceptSession == NULL)
		m->pAcceptSession = new CTcpSessionServerHandler();

	if (!m->pAcceptSession->Create(m_socket, 4096))
		goto error_exit;

	CTcpSessionServerHandlerPrivate *&pSessionMembers = m->pAcceptSession->m;
	if (!m->fnAccepteEx(m_socket, m->pAcceptSession->m_socket, pSessionMembers->wsabuf.buf,
		pSessionMembers->wsabuf.len - (sizeof(struct sockaddr_in) + 16) * 2,
		sizeof(struct sockaddr_in) + 16, sizeof(struct sockaddr_in) + 16, // +16 详见 AcceptEx 函数参数 dwLocalAddressLength 和 dwRemoteAddressLength 
		NULL, IocpAsyncOverlap()))
	{
		if (GetLastError() == ERROR_IO_PENDING)
			return TRUE;
	}

error_exit:

	// TODO: handle error,and then restore work.
	delete m->pAcceptSession;
	m->pAcceptSession = NULL;
	return FALSE;
}

BOOL CTcpListenerHandler::DataTransferTrigger(DWORD dwNumOfTransportBytes)
{
	if (dwNumOfTransportBytes != 0)
	{
		m->pAcceptSession->DataTransferTrigger(dwNumOfTransportBytes);
		CIoCompletionPortModel::Instance()->AttachHandler(m->pAcceptSession);
		m->pAcceptSession = NULL;
	}

	return TRUE;
}

BOOL CTcpListenerHandler::_InitSessionForAccept()
{
// 	if (AcceptEx(m_socket, m->pSessionForAccept->m_socket,
// 		m->pSessionForAccept->m->wsaBuf.buf,
// 		m->pSessionForAccept->m->wsaBuf.len - (sizeof(struct sockaddr_in) + 16) * 2,
// 		sizeof(struct sockaddr_in) + 16, sizeof(struct sockaddr_in) + 16, nullptr,
// 		IocpAsyncOverlap())) // +16 详见 AcceptEx 函数参数 dwLocalAddressLength 和 dwRemoteAddressLength 
// 	{
// 		if (WSAGetLastError() != WSA_IO_PENDING)
// 		{
// 
// 			return FALSE;
// 		}
// 	}

	return TRUE;
}

BOOL CTcpListenerHandler::HandleRaiseError(DWORD dwErrorCode)
{
	return FALSE;
}

void CTcpListenerHandler::Destroy()
{
	closesocket(m_socket);
	m_socket = INVALID_SOCKET;
}

BOOL CTcpListenerHandler::IsDestroyed()
{
	return m_socket == INVALID_SOCKET;
}
