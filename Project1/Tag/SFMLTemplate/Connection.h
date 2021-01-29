#pragma once
#include <WinSock2.h>
#include "PacketManager.h"

class Connection
{
public:
	Connection(SOCKET socket)
		:m_socket(socket)
	{
	}
	SOCKET m_socket;

	PacketManager m_pm;
	int m_ID = 0;
};
