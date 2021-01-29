#include "PacketManager.h"


// Check if the Manager has any packets ready to go
bool PacketManager::packetsReady()
{
	std::lock_guard<std::mutex> dolock(m_packetLock);
	return (!m_packets.empty()); 
}

// Adds a packet to the packet queue
void PacketManager::add(std::shared_ptr<Packet> p)
{
	std::lock_guard<std::mutex> dolock(m_packetLock); 
	m_packets.push(std::move(p));
}

// Gets the next Packet for sending
std::shared_ptr<Packet> PacketManager::getPacket()
{
	std::lock_guard<std::mutex> dolock(m_packetLock);
	std::shared_ptr<Packet> p = m_packets.front(); 
	m_packets.pop(); 
	return p; 
}

// Clear the queue upon ending
void PacketManager::clear()
{
	std::lock_guard<std::mutex> dolock(m_packetLock);
	m_packets = std::queue<std::shared_ptr<Packet>>{};
}
