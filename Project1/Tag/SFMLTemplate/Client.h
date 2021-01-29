#pragma once

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma comment(lib, "ws2_32.lib")

#include <WinSock2.h> 
#include "PacketManager.h" 

#include "Global.h"


class Client
{
public: 
	Client(const char* ip, const int port);
	bool Connect();
	void Disconnect();
	void SendString(const std::string& str);
	void sendPosition(float xPos, float yPos);
	void sendColor(int t_playerCol);
	void sendEnd(int t_playerCol);
	~Client();
private:
	bool CloseConnection();
	bool ProcessPacketType(const PacketType packetType);

	static void ClientThread(Client& client); 
	static void PacketSenderThread(Client& client); 

	bool sendall(const char* data, const int totalBytes);
	bool recvall(char* data, const int totalBytes);

	bool Getint32_t(std::int32_t& int32_t);
	bool GetPacketType(PacketType& packetType);
	bool GetString(std::string& str);
	bool GetFloat(float& flt);

	bool m_killThreads = false;
	bool m_isConnected = false;
	SOCKET m_connection;
	SOCKADDR_IN m_addr; 
	PacketManager m_pm; 

	std::thread m_pst; 
	std::thread m_ct; 
};


