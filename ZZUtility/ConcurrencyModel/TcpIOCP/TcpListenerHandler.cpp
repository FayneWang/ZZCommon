
#include <WinSock2.h>
#include <mswsock.h>
#include "../IoCompletionPortModel.h"
#include "TcpHandlerPrivateTypes.h"
#include "TcpServerSessionHandler.h"
#include "TcpListenerHandler.h"


CTcpListenerHandler::CTcpListenerHandler(void) : m(new CTcpListenerHandlerPrivate()), m_pServerSessionCreator(NULL)
{
	WSADATA wsaData = { 0 };
	WSAStartup(MAKEWORD(2, 2), &wsaData);
}

CTcpListenerHandler::~CTcpListenerHandler(void)
{
	Destroy();
	WSACleanup();
	WaitDetachIocpModel();

	delete m;
}

BOOL CTcpListenerHandler::Create(USHORT usPort, ITcpServerSessionFactory *pCreator, CIoCompletionPortModel *pAttachIocp)
{
	if (!InitializeListernSource(usPort))
		return FALSE;

	m_pServerSessionCreator = pCreator;
	m->pIocpModel = pAttachIocp;
	pAttachIocp->AttachHandler(this);

	return TRUE;
}

BOOL CTcpListenerHandler::InitializeListernSource(USHORT usPort)
{
	m_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (INVALID_SOCKET == m_socket)
	{
		return FALSE;
	}

	struct sockaddr_in sockAddrIn = { 0 };
	sockAddrIn.sin_family = AF_INET;
	sockAddrIn.sin_addr.s_addr = ADDR_ANY;
	sockAddrIn.sin_port = htons(usPort);
	if (bind(m_socket, (struct sockaddr*)&sockAddrIn, sizeof(sockAddrIn)) != 0)
	{
		closesocket(m_socket);
		m_socket = INVALID_SOCKET;
		return FALSE;
	}

	if (listen(m_socket, SOMAXCONN) != 0)
	{
		closesocket(m_socket);
		m_socket = INVALID_SOCKET;
		return FALSE;
	}

	GUID	guidAcceptEx = WSAID_ACCEPTEX;
	DWORD	dwFunPointerSize = 0;
	if (WSAIoctl(m_socket, SIO_GET_EXTENSION_FUNCTION_POINTER,
		&guidAcceptEx, sizeof(guidAcceptEx), &m->fnAccepteEx,
		sizeof(LPFN_ACCEPTEX), &dwFunPointerSize, nullptr, nullptr) != 0)
	{
		closesocket(m_socket);
		m_socket = INVALID_SOCKET;
		return FALSE;
	}

	return TRUE;
}

BOOL CTcpListenerHandler::OverlapForIOCompletion()
{
	m->pAcceptSession = m_pServerSessionCreator->CreateSession();
	if (!m->pAcceptSession->Create(2048))
			goto error_exit;

 	CTcpServerSessionHandlerPrivate *&pSessionMembers = m->pAcceptSession->m;
	if (m->fnAccepteEx(m_socket, m->pAcceptSession->m_socket, pSessionMembers->wsaBuf.buf, 0, 
		sizeof(struct sockaddr_in) + 16, sizeof(struct sockaddr_in) + 16, // +16 详见 AcceptEx 函数参数 dwLocalAddressLength 和 dwRemoteAddressLength 
		NULL, IocpAsyncOverlap()))
	{
		return TRUE;
	}
	else
	{
		return GetLastError() == ERROR_IO_PENDING;
	}

error_exit:

	// TODO: handle error,and then restore work.
	delete m->pAcceptSession;
	m->pAcceptSession = NULL;
	return FALSE;
}

BOOL CTcpListenerHandler::DataTransferTrigger(DWORD dwNumOfTransportBytes)
{
// 	{ // TODO: 保留获取远程连接IP使用。
// 		GUID	guidAcceptEx = WSAID_GETACCEPTEXSOCKADDRS;
// 		DWORD	dwFunPointerSize = 0;
// 		LPFN_GETACCEPTEXSOCKADDRS fnGetAcceptExSockAddrs;
// 		if (WSAIoctl(m_socket, SIO_GET_EXTENSION_FUNCTION_POINTER,
// 			&guidAcceptEx, sizeof(guidAcceptEx), &fnGetAcceptExSockAddrs,
// 			sizeof(LPFN_GETACCEPTEXSOCKADDRS), &dwFunPointerSize, nullptr, nullptr) == 0)
// 		{
// 			struct sockaddr_in *pLocal, pRemote;
// 			int iLocal, iRemote;
// 			fnGetAcceptExSockAddrs(m->pAcceptSession->m->wsabuf.buf,
// 				0,
// 				sizeof(struct sockaddr_in) + 16, sizeof(struct sockaddr_in) + 16,
// 				(struct sockaddr**)&pLocal, &iLocal, (struct sockaddr**)&pRemote, &iRemote);
// 
// 			const char *pstrAddr = inet_ntoa(pLocal->sin_addr);
// 			pstrAddr = pstrAddr;
// 		}
// 	}

	if (!m->pIocpModel->AttachHandler(m->pAcceptSession))
	{
		delete m->pAcceptSession;
	}
	m_pServerSessionCreator->NewSessionTrigger(m->pAcceptSession);
	m->pAcceptSession = NULL;

	return TRUE;
}

void CTcpListenerHandler::HandleRaiseError(DWORD dwErrorCode)
{
}

void CTcpListenerHandler::Destroy()
{
	closesocket(m_socket);
	m_socket = INVALID_SOCKET;

	delete m->pAcceptSession;
	m->pAcceptSession = NULL;

	m->pIocpModel = NULL;
}
