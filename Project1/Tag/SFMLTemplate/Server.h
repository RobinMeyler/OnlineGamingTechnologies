#pragma once

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma comment(lib, "ws2_32.lib")
#include <iostream>
#include <string>
#include <WinSock2.h>
#include <shared_mutex> 
#include "Global.h"
#include "PacketManager.h" 
#include <vector> 
#include <shared_mutex> 
#include "Global.h"

#include "Connection.h"
class Server
{
public: 
	Server(int port, bool loopBacktoLocalHost = true);
	~Server();
	bool ListenForNewConnection();
private: 

	bool Getint(std::shared_ptr<Connection> connection, std::int32_t& int32_t);
	bool GetFloat(std::shared_ptr<Connection> connection, float& flt);
	bool GetPacketType(std::shared_ptr<Connection> connection, PacketType& packetType);

	bool sendall(std::shared_ptr<Connection> connection, const char* data, const int totalBytes);
	bool recvall(std::shared_ptr<Connection> connection, char* data, int totalBytes);

	void SendString(std::shared_ptr<Connection> connection, const std::string& str);
	bool GetString(std::shared_ptr<Connection> connection, std::string& str);

	bool ProcessPacket(std::shared_ptr<Connection> connection, PacketType packetType);
	void DisconnectClient(std::shared_ptr<Connection> connection); 

	static void ClientHandlerThread(Server& server, std::shared_ptr<Connection> connection);
	static void PacketSenderThread(Server& server);
private: 
	std::vector<std::shared_ptr<Connection>> m_connections;
	std::shared_mutex m_mutex_connectionMgr; 
	int m_IDCounter = 0;
	SOCKADDR_IN m_addr;
	SOCKET m_sListen;
	bool m_terminateThreads = false;
	std::vector<std::thread*> m_threads; 
};
