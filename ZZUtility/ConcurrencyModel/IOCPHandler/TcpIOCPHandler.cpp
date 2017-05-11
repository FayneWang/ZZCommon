
#include <WinSock2.h>
#include "TcpIOCPHandler.h"


class CTcpIOCPHandlerPrivate
{
	friend class CTcpIOCPHandler;

	USHORT usPort;
};

CTcpIOCPHandler::CTcpIOCPHandler(USHORT usPort) : m(new CTcpIOCPHandlerPrivate())
{
	m->usPort = usPort;
}

CTcpIOCPHandler::~CTcpIOCPHandler()
{
	WaitDetachIocpModel();
	delete m;
}

BOOL CTcpIOCPHandler::Create()
{
	m_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (INVALID_SOCKET == m_socket)
		return FALSE;

	struct sockaddr_in sockAddrIn = { 0 };
	sockAddrIn.sin_family = AF_INET;
	sockAddrIn.sin_addr.s_addr = ADDR_ANY;
	//socket服务端端口暂定为8888
	sockAddrIn.sin_port = htons(m->usPort);
	if (bind(m_socket, (struct sockaddr*)&sockAddrIn, sizeof(sockAddrIn)) != 0)
	{
		closesocket(m_socket);
		m_socket = INVALID_SOCKET;
		return FALSE;
	}

	// 仅允许一个会话存证，所在最大积累取小值为 1。
	if (listen(m_socket, 1) != 0)
	{
		closesocket(m_socket);
		m_socket = INVALID_SOCKET;
		return FALSE;
	}

	struct sockaddr_in sockAddr = { 0 };
	int iAddrLen = sizeof(sockAddr);
	if (getsockname(m_socket, (struct sockaddr*)&sockAddr, &iAddrLen) != 0)
	{
		closesocket(m_socket);
		m_socket = INVALID_SOCKET;
		return FALSE;
	}

	// 获取完成接口使用的函数指针。
// 	GUID	guidAcceptex = WSAID_ACCEPTEX;
// 	DWORD	dwFunPointerSize = 0;
// 	if (WSAIoctl(m_socket, SIO_GET_EXTENSION_FUNCTION_POINTER,
// 		&guidAcceptex, sizeof(guidAcceptex), &m->lpfnAcceptex,
// 		sizeof(m->lpfnAcceptex), &dwFunPointerSize, nullptr, nullptr) != 0)
// 	{
// 		closesocket(m_socket);
// 		m_socket = INVALID_SOCKET;
// 		return FALSE;
// 	}

	// m->uServicePort = ntohs(sockAddr.sin_port);

	return TRUE;
}

BOOL CTcpIOCPHandler::OverlapForIoOperation()
{
	return TRUE;
}

BOOL CTcpIOCPHandler::DataTransferTrigger(DWORD dwNumOfTransportBytes)
{
	return FALSE;
}

BOOL CTcpIOCPHandler::HandleRaiseError(DWORD dwErrorCode)
{
	return FALSE;
}
