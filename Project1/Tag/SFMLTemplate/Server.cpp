#include "Server.h"
#include <iostream>
#include <WS2tcpip.h>
#include "PacketStructs.h"
#include "Game.h"
#pragma comment(lib,"ws2_32.lib")


// Server contructor, sets up the port and IP to use, binds and gets ready to listen
Server::Server(int port, bool loopBacktoLocalHost) 
{

	WSAData wsaData;
	WORD DllVersion = MAKEWORD(2, 1);
	if (WSAStartup(DllVersion, &wsaData) != 0) 
	{
		MessageBoxA(0, "WinSock startup failed", "Error", MB_OK | MB_ICONERROR);
		exit(1);
	}
	if (loopBacktoLocalHost)
	{
		inet_pton(AF_INET, "127.0.0.1", &m_addr.sin_addr.s_addr);
	}
	else
	{
		m_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	}

	m_addr.sin_port = htons(port);
	m_addr.sin_family = AF_INET; 

	m_sListen = socket(AF_INET, SOCK_STREAM, 0);
	if (bind(m_sListen, (SOCKADDR*)&m_addr, sizeof(m_addr)) == SOCKET_ERROR) 
	{
		std::string ErrorMsg = "Failed to bind the address to our listening socket. Winsock Error:" + std::to_string(WSAGetLastError());
		MessageBoxA(0, ErrorMsg.c_str(), "Error", MB_OK | MB_ICONERROR);
		exit(1);
	}
	if (listen(m_sListen, SOMAXCONN) == SOCKET_ERROR) 
	{
		std::string ErrorMsg = "Failed to listen on listening socket. Winsock Error:" + std::to_string(WSAGetLastError());
		MessageBoxA(0, ErrorMsg.c_str(), "Error", MB_OK | MB_ICONERROR);
		exit(1);
	}
	m_IDCounter = 0;
	std::thread PST(PacketSenderThread, std::ref(*this));
	PST.detach();
	m_threads.push_back(&PST);
}

// Waits and listens for new connections from clients
bool Server::ListenForNewConnection()
{
	int addrlen = sizeof(m_addr);
	SOCKET newConnectionSocket = accept(m_sListen, (SOCKADDR*)&m_addr, &addrlen);
	if (newConnectionSocket == 0) 
	{
		std::cout << "Failed to accept the client's connection." << std::endl;
		return false;
	}
	else 
	{
		std::lock_guard<std::shared_mutex> lock(m_mutex_connectionMgr); 
		std::shared_ptr<Connection> newConnection(std::make_shared<Connection>(newConnectionSocket));
		m_connections.push_back(newConnection); 
		newConnection->m_ID = m_IDCounter;
		m_IDCounter += 1;
		std::cout << "Client Connected! ID:" << newConnection->m_ID << std::endl;
		std::thread CHT(ClientHandlerThread, std::ref(*this), newConnection);
		CHT.detach();
		m_threads.push_back(&CHT);
		return true;
	}
}

Server::~Server()
{
	m_terminateThreads = true;
	for (std::thread* t : m_threads) 
	{
		t->join();
	}
}

// Handler thread used to determien packet type and process the packets
void Server::ClientHandlerThread(Server& server, std::shared_ptr<Connection> connection)
{
	PacketType packettype;
	while (true)
	{
		if (server.m_terminateThreads == true)
		{
			break;
		}
		if (!server.GetPacketType(connection, packettype))
		{
			break;
		}
		if (!server.ProcessPacket(connection, packettype))
		{
			break;
		}
	}
	std::cout << "Lost connection to client ID: " << connection->m_ID << std::endl;
	server.DisconnectClient(connection); 
	return;
}

// Thread Send packets to the clients
void Server::PacketSenderThread(Server& server) 
{
	while (true)
	{
		if (server.m_terminateThreads == true)
		{
			break;
		}
		std::shared_lock<std::shared_mutex> lock(server.m_mutex_connectionMgr);
		for (auto connect : server.m_connections) 
		{
			if (connect->m_pm.packetsReady())
			{
				std::shared_ptr<Packet> pac = connect->m_pm.getPacket();
				if (!server.sendall(connect, (const char*)(&pac->m_buffer[0]), pac->m_buffer.size()))
				{
					std::cout << "Failed to send packet to ID: " << connect->m_ID << std::endl;
				}
			}
		}
		Sleep(5);
	}
}

void Server::DisconnectClient(std::shared_ptr<Connection> connection) 
{
	std::lock_guard<std::shared_mutex> lock(m_mutex_connectionMgr); 
	Game::m_gamePtr->Clear(connection->m_ID);
	connection->m_pm.clear(); 
	closesocket(connection->m_socket); 
	m_connections.erase(std::remove(m_connections.begin(), m_connections.end(), connection)); 
	std::cout << "Cleaned up client: " << connection->m_ID << "." << std::endl;
	std::cout << "Total connections: " << m_connections.size() << std::endl;
}

// Process the packet using the type to recieve the correct info int he correct ofder, then sned it to the clients
bool Server::ProcessPacket(std::shared_ptr<Connection> connection, PacketType packetType)
{
	switch (packetType)
	{
	case PacketType::ChatMessages: 
	{
		std::string message; 
		if (!GetString(connection, message))
		{
			return false;
		}
						 
		PacketInfo::ChatMessage cm(message);
		std::shared_ptr<Packet> msgPacket = std::make_shared<Packet>(cm.toPacket());
		{
			std::shared_lock<std::shared_mutex> lock(m_mutex_connectionMgr);
			for (auto conn : m_connections) 
			{
				if (conn == connection)
				{
					continue;
				}
				conn->m_pm.add(msgPacket);
			}
		}
		std::cout << "Processed chat message packet from user ID: " << connection->m_ID << std::endl;
		break;
	}
	case PacketType::PositionUpdate: 
	{
		int id;
		float xPos, yPos;
		if (!Getint(connection, id))
			return false;
		if (!GetFloat(connection, xPos))
			return false;
		if (!GetFloat(connection, yPos))
			return false;

		PacketInfo::PositionUpdate pos(connection->m_ID, xPos, yPos);
		std::shared_ptr<Packet> msgPacket = std::make_shared<Packet>(pos.toPacket());
		{
			std::shared_lock<std::shared_mutex> lock(m_mutex_connectionMgr);
			for (auto conn : m_connections) 
			{
				if (conn == connection) 
					continue;
				conn->m_pm.add(msgPacket);
			}
		}
		break;
	}
	case PacketType::ColorUpdate: 
	{
		int col, id;

		if (!Getint(connection, id))
			return false;
		if (!Getint(connection, col))
			return false;

		PacketInfo::ColorUpdate color(connection->m_ID, col);
		std::shared_ptr<Packet> msgPacket = std::make_shared<Packet>(color.toPacket()); 
		{
			std::shared_lock<std::shared_mutex> lock(m_mutex_connectionMgr);
			for (auto conn : m_connections) 
			{
				if (conn == connection)
				{
					continue;
				}
				conn->m_pm.add(msgPacket);
			}
		}
	}
	break;
	case PacketType::EndGame: 
	{
		int end;

		if (!Getint(connection, end))
			return false;
		
		PacketInfo::ColorUpdate color(end);
		std::shared_ptr<Packet> msgPacket = std::make_shared<Packet>(color.toPacket()); 
		{
			std::shared_lock<std::shared_mutex> lock(m_mutex_connectionMgr);
			for (auto conn : m_connections) 
			{
				if (conn == connection) 
					continue;
				conn->m_pm.add(msgPacket);
			}
		}
	}
	break;
	default: 
	{
		std::cout << "Unrecognized packet: " << (std::int32_t)packetType << std::endl; 
		return false;
	}
	}
	return true;
}

bool Server::recvall(std::shared_ptr<Connection> connection, char* data, int totalbytes)
{
	int bytesReceived = 0;
	while (bytesReceived < totalbytes) 
	{
		int retnCheck = recv(connection->m_socket, data + bytesReceived, totalbytes - bytesReceived, 0);
		if (retnCheck == SOCKET_ERROR || retnCheck == 0)
		{
			return false;
		}
		bytesReceived += retnCheck; 
	}
	return true; 
}

bool Server::sendall(std::shared_ptr<Connection> connection, const char* data, const int totalBytes)
{
	int bytesSent = 0;
	while (bytesSent < totalBytes)
	{
		int retnCheck = send(connection->m_socket, data + bytesSent, totalBytes - bytesSent, 0); 
		if (retnCheck == SOCKET_ERROR) 
			return false; 
		bytesSent += retnCheck;
	}
	return true; 
}

bool Server::Getint(std::shared_ptr<Connection> connection, std::int32_t& int32_t)
{
	if (!recvall(connection, (char*)&int32_t, sizeof(std::int32_t)))
	{
		return false;
	}
	int32_t = ntohl(int32_t); 
	return true;
}

bool Server::GetPacketType(std::shared_ptr<Connection> connection, PacketType& packetType)
{
	std::int32_t packettype_int;
	if (!Getint(connection, packettype_int))
	{
		return false;
	}
	packetType = (PacketType)packettype_int;
	return true;
}

void Server::SendString(std::shared_ptr<Connection> connection, const std::string& str)
{
	PacketInfo::ChatMessage message(str);
	connection->m_pm.add(message.toPacket());
}

bool Server::GetString(std::shared_ptr<Connection> connection, std::string& str)
{
	std::int32_t bufferlength;
	if (!Getint(connection, bufferlength))
	{
		return false;
	}
	if (bufferlength == 0) return true;
	str.resize(bufferlength); 
	return recvall(connection, &str[0], bufferlength);
}

bool Server::GetFloat(std::shared_ptr<Connection> connection, float& flt)
{
	if (!recvall(connection, (char*)&flt, sizeof(float)))
	{
		return false;
	}
	flt = ntohl(flt);
	return true;
}