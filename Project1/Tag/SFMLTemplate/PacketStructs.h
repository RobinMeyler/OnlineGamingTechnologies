#pragma once
#include "PacketType.h"
#include "Packet.h"
#include <string> 
#include <memory> 
#include "Global.h"

namespace PacketInfo 
{
	// Chat/string messages
	class ChatMessage
	{
	public:
		ChatMessage(const std::string & str);
		std::shared_ptr<Packet> toPacket(); 
	private:
		std::string m_message;
	};

	// Update positions on server
	class PositionUpdate
	{
	public:
		PositionUpdate(const float t_xPos, const float t_yPos);
		PositionUpdate(const int t_id, const float t_xPos, const float t_yPos);
		std::shared_ptr<Packet> toPacket(); 
	private:
		int m_id;
		float m_xPos;
		float m_yPos;
	};

	// update the player color
	class ColorUpdate
	{
	public:
		ColorUpdate(const int col);
		ColorUpdate(const int t_id, const int col);
		std::shared_ptr<Packet> toPacket(); 
	private:
		int m_id;
		int m_col;
	};

	// Signal that the game has ended
	class EndGame
	{
	public:
		EndGame(const int end);
		std::shared_ptr<Packet> toPacket(); 
	private:
		int m_end;
	};
}