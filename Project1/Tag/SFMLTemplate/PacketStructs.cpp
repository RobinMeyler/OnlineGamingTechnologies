#include "PacketStructs.h"

namespace PacketInfo
{
	ChatMessage::ChatMessage(const std::string& msg)
		:m_message(msg)
	{
	}

	std::shared_ptr<Packet> ChatMessage::toPacket()
	{
		std::shared_ptr<Packet> p = std::make_shared<Packet>();
		p->Append(PacketType::ChatMessages);
		p->Append(m_message.size());
		p->Append(m_message);
		return p;
	}

	PositionUpdate::PositionUpdate(const float t_xPos, const float t_yPos) :
		m_xPos(t_xPos),
		m_yPos(t_yPos),
		m_id(100)
	{
	}

	PositionUpdate::PositionUpdate(const int t_id, const float t_xPos, const float t_yPos) :
		m_id(t_id),
		m_xPos(t_xPos),
		m_yPos(t_yPos)
	{
	}

	std::shared_ptr<Packet> PositionUpdate::toPacket()
	{
		std::shared_ptr<Packet> p = std::make_shared<Packet>();
		p->Append(PacketType::PositionUpdate);
		p->Append(m_id);
		p->Append(m_xPos);
		p->Append(m_yPos);
		return p;
	}
	ColorUpdate::ColorUpdate(const int col) :
		m_col(col),
		m_id(100)
	{
	}

	ColorUpdate::ColorUpdate(const int t_id, const int col) :
		m_col(col),
		m_id(t_id)
	{
	}

	std::shared_ptr<Packet> ColorUpdate::toPacket()
	{
		std::shared_ptr<Packet> p = std::make_shared<Packet>();
		p->Append(PacketType::ColorUpdate);
		p->Append(m_id);
		p->Append(m_col);
		return p;
	}
	EndGame::EndGame(const int end) : 
		m_end(end)
	{
	}
	std::shared_ptr<Packet> EndGame::toPacket()
	{
		std::shared_ptr<Packet> p = std::make_shared<Packet>();
		p->Append(PacketType::EndGame);
		p->Append(m_end);
		return p;
	}
}