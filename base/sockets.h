#pragma once
class sockets
{
private:
	static SOCKET sock;

public:
	static bool Connect(byte IP[4], unsigned short Port);
	static bool Send(char* Data, unsigned int size);
	static bool Receive(char* Data, unsigned int size);
	static void Close();
};

#define SOCKET_TIME_OUT 10000