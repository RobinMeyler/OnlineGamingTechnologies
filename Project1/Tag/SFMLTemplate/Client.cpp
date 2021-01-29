#include "Client.h"
#include <Ws2tcpip.h> 
#pragma comment(lib,"ws2_32.lib") 
#include <iostream> 
#include "PacketStructs.h"
#include "Game.h"

// Contruct client with networking info
Client::Client(const char* ip, const int port)
{
	//Winsock Startup
	WSAData wsaData;
	WORD DllVersion = MAKEWORD(2, 1);
	if (WSAStartup(DllVersion, &wsaData) != 0)
	{
		MessageBoxA(0, "Winsock startup failed", "Error", MB_OK | MB_ICONERROR);
		exit(0);
	}

	inet_pton(AF_INET, ip, &m_addr.sin_addr.s_addr); 
	m_addr.sin_port = htons(port); 
	m_addr.sin_family = AF_INET; 
}

// Try connect to the sever
bool Client::Connect()
{
	m_connection = socket(AF_INET, SOCK_STREAM, 0);
	int sizeofaddr = sizeof(m_addr);
	if (connect(m_connection, (SOCKADDR*)&m_addr, sizeofaddr) != 0) 
	{
		return false;
	}

	std::cout << "Connected!" << std::endl;
	m_pst = std::thread(PacketSenderThread, std::ref(*this)); 
	m_pst.detach();
	m_ct = std::thread(ClientThread, std::ref(*this));
	m_ct.detach();
	return true;
}

// Clsoe connection if needed
bool Client::CloseConnection()
{
	m_killThreads = true;
	if (closesocket(m_connection) == SOCKET_ERROR)
	{
		if (WSAGetLastError() == WSAENOTSOCK) 
			return true; 

		std::string ErrorMessage = "Failed to close the socket. Winsock Error: " + std::to_string(WSAGetLastError()) + ".";
		MessageBoxA(0, ErrorMessage.c_str(), "Error", MB_OK | MB_ICONERROR);
		return false;
	}
	return true;
}

Client::~Client()
{
	CloseConnection();
	m_pst.join();
	m_ct.join();
}

// Process every PAcket that comes in and filter them
bool Client::ProcessPacketType(PacketType packetType)
{
	switch (packetType)
	{
	case PacketType::ChatMessages:
	{
		std::string Message; 
		if (!GetString(Message)) 
			return false; 
		std::cout << Message << std::endl; 
		break;
	}
	case PacketType::PositionUpdate: 
	{
		int id;
		float xPos, yPos;
		if (!Getint32_t(id))
			return false;
		if (!GetFloat(xPos))
			return false;
		if (!GetFloat(yPos))
			return false;
		Game::m_gamePtr->updatePosition(id, xPos, yPos);
		break;
	}
	case PacketType::ColorUpdate: 
	{
		int id, col;
		if (!Getint32_t(id))
			return false;
		if (!Getint32_t(col))
			return false;

		Game::m_gamePtr->updateColor(id, col);
		break;
	}
	case PacketType::EndGame: 
	{
		int end;
		if (!Getint32_t(end))
			return false;
		if(end == 1)
			Game::m_gamePtr->endGame(end);

		break;
	}
	default: 
		std::cout << "Unrecognized PacketType: " << (std::int32_t)packetType << std::endl; 
		break;
	}
	return true;
}

// Threaded for the connection to manage packets
void Client::ClientThread(Client& client)
{
	PacketType PacketType;
	while (true)
	{
		if (client.m_killThreads == true)
		{
			break;
		}
		if (!client.GetPacketType(PacketType))
		{
			break;
		}
		if (!client.ProcessPacketType(PacketType))
		{
			break;
		}
	}
	std::cout << "Lost connection to the server.\n";
	client.m_killThreads = true;
	client.CloseConnection();
}

// threaded to send thread to the server
void Client::PacketSenderThread(Client& client) 
{
	while (true)
	{
		if (client.m_killThreads == true)
			break;
		while (client.m_pm.packetsReady())
		{
			std::shared_ptr<Packet> p = client.m_pm.getPacket();
			if (!client.sendall((const char*)(&p->m_buffer[0]), p->m_buffer.size()))
			{
				std::cout << "Failed to send packet to server..." << std::endl;
				break;
			}
		}
		Sleep(5);
	}
	std::cout << "Packet thread closing..." << std::endl;
}

void Client::Disconnect()
{
	m_pm.clear();
	closesocket(m_connection);
	std::cout << "Disconnected from server." << std::endl;
}

bool Client::recvall(char* data, int totalBytes)
{
	int bytesReceived = 0;
	while (bytesReceived < totalBytes) 
	{
		int RetnCheck = recv(m_connection, data + bytesReceived, totalBytes - bytesReceived, 0); 
		if (RetnCheck == SOCKET_ERROR || RetnCheck == 0) 
		{
			return false;
		}
		bytesReceived += RetnCheck;
	}
	return true;
}

bool Client::sendall(const char* data, const int totalBytes)
{
	int bytesSent = 0; 
	while (bytesSent < totalBytes) 
	{
		int RetnCheck = send(m_connection, data + bytesSent, totalBytes - bytesSent, 0); 
		if (RetnCheck == SOCKET_ERROR) 
		{
			return false;
		}
		bytesSent += RetnCheck; 
	}
	return true; 
}

bool Client::Getint32_t(std::int32_t& int32_t)
{
	if (!recvall((char*)&int32_t, sizeof(std::int32_t)))
	{
		return false;
	}
	int32_t = ntohl(int32_t); 
	return true;
}

bool Client::GetPacketType(PacketType& packetType)
{
	std::int32_t packetType_int;
	if (!Getint32_t(packetType_int))
	{
		return false;
	}
	packetType = (PacketType)packetType_int;
	return true;
}

void Client::SendString(const std::string& str)
{
	PacketInfo::ChatMessage cm(str);
	m_pm.add(cm.toPacket());
}

bool Client::GetString(std::string& str)
{
	int32_t bufferlength; 
	if (!Getint32_t(bufferlength))
	{
		return false;
	}
	if (bufferlength == 0) return true;
	str.resize(bufferlength); 
	return recvall(&str[0], bufferlength);
}

bool Client::GetFloat(float& flt)
{
	if (!recvall((char*)&flt, sizeof(float)))
	{
		return false;
	}
	flt = ntohl(flt); 
	return true;
}

void Client::sendPosition(float xPos, float yPos)
{
	PacketInfo::PositionUpdate pos(xPos, yPos);
	m_pm.add(pos.toPacket());
}

void Client::sendColor(int t_playerCol)
{
	PacketInfo::ColorUpdate col(t_playerCol);
	m_pm.add(col.toPacket());
}

void Client::sendEnd(int end)
{
	PacketInfo::EndGame m_end(end);
	m_pm.add(m_end.toPacket());
}
