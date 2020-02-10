#include "main.h"

SOCKET sockets::sock = 0;

bool sockets::Connect(byte IP[4], unsigned short Port)
{
	DWORD socketError;

	XNetStartupParams param;
	memset(&param, 0, sizeof(XNetStartupParams));

	param.cfgSizeOfStruct = sizeof(XNetStartupParams);
	param.cfgFlags = XNET_STARTUP_BYPASS_SECURITY;
	if ((socketError = NetDll_XNetStartup(XNCALLER_SYSAPP, &param)) != 0)
	{
		return false;
	}

	WSADATA wsa;
	if ((socketError = NetDll_WSAStartupEx(XNCALLER_SYSAPP, MAKEWORD(2, 2), &wsa, 0)) != 0)
	{
		return false;
	}

	if ((sock = NetDll_socket(XNCALLER_SYSAPP, AF_INET, SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET)
	{
		return false;
	}

	bool bSockOpt = 1;
	if (NetDll_setsockopt(XNCALLER_SYSAPP, sock, SOL_SOCKET, 0x5801, (PCSTR)&bSockOpt, sizeof(BOOL)) != 0)
	{
		return false;
	}

	int sendRecvSize = 1024 * 2;
	NetDll_setsockopt(XNCALLER_SYSAPP, sock, SOL_SOCKET, SO_SNDBUF, (PCSTR)&sendRecvSize, sizeof(int));
	NetDll_setsockopt(XNCALLER_SYSAPP, sock, SOL_SOCKET, SO_RCVBUF, (PCSTR)&sendRecvSize, sizeof(int));

	sockaddr_in socket;
	socket.sin_family = AF_INET;
	socket.sin_port = htons(Port);
	//socket.sin_addr.s_addr = inet_addr(IP);
	socket.sin_addr.S_un.S_un_b.s_b1 = IP[0];
	socket.sin_addr.S_un.S_un_b.s_b2 = IP[1];
	socket.sin_addr.S_un.S_un_b.s_b3 = IP[2];
	socket.sin_addr.S_un.S_un_b.s_b4 = IP[3];

	if (NetDll_connect(XNCALLER_SYSAPP, sock, (struct sockaddr*)&socket, sizeof(sockaddr_in)) == SOCKET_ERROR)
		return false;

	return true;
}

bool sockets::Send(char* Data, unsigned int size)
{
	if (NetDll_send(XNCALLER_SYSAPP, sock, Data, size, 0) == SOCKET_ERROR)
	{
		return false;
	}
	return true;
}

bool sockets::Receive(char* Data, unsigned int size)
{
	if (NetDll_recv(XNCALLER_SYSAPP, sock, Data, size, 0) == SOCKET_ERROR)
	{
		return false;
	}
	return true;
}

void sockets::Close()
{
	NetDll_shutdown(XNCALLER_SYSAPP, sock, 2);
	NetDll_closesocket(XNCALLER_SYSAPP, sock);
}