/*
* WorldPacket.h
* WorldPacket Stream
* Copyright (C) 2010-2012 XXZ Team <http://www.aiyou.com/>
* Create by  Along 2011.11.09
* Last update
*/

#ifndef __WORLDPACKET_H__
#define __WORLDPACKET_H__

#include "ByteBuffer.h"
// 游戏系统版本控制
struct PacketOpcode
{
	uint8 m_byCategory;
	uint8 m_byProtocol;
};
template<class T = PacketOpcode>
class WorldPacket : public ByteBuffer 
{
public:
	XY_INLINE WorldPacket() : ByteBuffer(){ memset(&m_tHeader, 0, sizeof(m_tHeader)); }
	XY_INLINE WorldPacket(uint8 category, uint8 protocol, size_t reserve) : ByteBuffer(reserve){ SetCategory(category); SetProtocol(protocol); }
	XY_INLINE WorldPacket(size_t reserve) : ByteBuffer(reserve){ memset(&m_tHeader, 0, sizeof(m_tHeader)); }
	XY_INLINE WorldPacket(T & tPacket, size_t reserve) : ByteBuffer(reserve), m_tHeader(tPacket) {}
	XY_INLINE WorldPacket(const WorldPacket & packet) : ByteBuffer(packet), m_tHeader(packet.m_tHeader) {}

	// 初始化
    XY_INLINE void Initialize(T & tPacket)
    {
        clear();
		memcpy(&m_tHeader, &tPacket, sizeof(T));
    }

	XY_INLINE void	WritePacketHeader()
	{
		if (size() > 0) return;
		append((uint8*)&m_tHeader, sizeof(T));
	}

	XY_INLINE T &	Get()				{ return m_tHeader;	}
    XY_INLINE uint8 GetCategory() const { return m_tHeader.m_byCategory; }
    XY_INLINE void SetCategory(uint8 category) { m_tHeader.m_byCategory = category; }

	XY_INLINE uint8 GetProtocol() const { return m_tHeader.m_byProtocol; }
    XY_INLINE void SetProtocol(uint8 protocol) { m_tHeader.m_byProtocol = protocol; }
protected:
	T		m_tHeader;
};

#endif
