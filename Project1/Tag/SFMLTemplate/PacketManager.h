#pragma once
#include "Packet.h" 
#include <memory> 
#include <queue>
#include <mutex> 

class PacketManager
{
private:
	std::queue<std::shared_ptr<Packet>> m_packets;
	std::mutex m_packetLock;
public:
	void clear();
	bool packetsReady();
	void add(std::shared_ptr<Packet> p);
	std::shared_ptr<Packet> getPacket();
};