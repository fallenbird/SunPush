#pragma once
#include <boost/noncopyable.hpp>

#include "Socket.h"
////////////////////////////////////////////////////////////////////////
//这个类只用在服务器的所有会话收发数据缓冲区中, 不用在独立消息中
class IOPacket : public boost::noncopyable
{
	class Session;
public:
	IOPacket();
	~IOPacket(void);

	char		ReadChar();
	char* 		ReadCharArray(long length);
	short*		ReadShortArray(long length);
	long*		ReadLongArray(long length);
	__int64*	ReadLongLongArray(long length);
	short		ReadShort();
	long		ReadLong();
	__int64		ReadLongLong();
	const TCHAR*	ReadString();
	void*			ReadPointer();

	bool	ReverseReadLong(long& data);
	bool	ReverseReadLongLong(__int64& data);

	bool	WriteChar(char data);
	bool	WriteCharArray(const char* data, long length);
	bool	WriteShort(short data);
	bool	WriteShortArray(const short* data, long length);
	bool	WriteLong(long data);
	bool	WriteLongArray(const long* data, long length);
	bool	WriteLongLong(__int64 data);
	bool	WriteLongLongArray(const __int64* data, long length);
	bool	WriteString(const TCHAR* data);
	//bool	WriteString(const std::string& data);
	bool	WritePointer(void* ptr);

	bool	ReWriteChar(long offset, char data);
	bool	ReWriteShort(long offset, short data);
	bool	ReWriteLong(long offset, long data);
	bool	ReWriteLongLong(long offset, __int64 data);

	char*	GetBuffer(){return /*m_pWrite-m_nWriteLength;*/m_szBuffer;}
	void	ResetReadPos();
	void	ResetWritePos();
	bool	Resize(long nNewAllSize);			//截断缓冲区
	void	Clear();

	long	GetWriteLength(){return m_nWriteLength;}
	long	GetWriteLeavings(){return m_nWriteLeavings;}
	long	GetReadLength(){return m_nReadLength;}
	long	GetReadLeavings(){return m_nReadLeavings;}

	void	ReadSkip(long length);
	void	WriteSkip(long length);

private: 
 
	char	m_szBuffer[SOCKET_IOBUFFER_SIZE];
	char*	m_pWrite;							//当前输入流指针
	char*	m_pRead;							//当前输出流指针
	long	m_nWriteLength;						//已写数据的长度
	long	m_nWriteLeavings;					//剩余可写数据长度
	long	m_nReadLength;						//已读数据的长度
	long	m_nReadLeavings;					//剩余可读数据长度
};

