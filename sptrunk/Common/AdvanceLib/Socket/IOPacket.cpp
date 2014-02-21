#include "../CommonHeader.h"
#include "Socket.h"
#include "IOPacket.h"
#include "basetsd.h"

IOPacket::IOPacket()
{
	Clear();
}

IOPacket::~IOPacket(void)
{
}

/************************************************************************/
/*                         private function                             */
/************************************************************************/

void IOPacket::ResetReadPos()
{
	m_pRead = m_szBuffer;										//读取缓冲区的偏移位置
	m_nReadLeavings = m_nWriteLength;							//剩余可读数据长度
	m_nReadLength = 0;	;										//已读数据的长度

}

void IOPacket::ResetWritePos()
{
	m_pWrite = m_szBuffer;											//写入缓冲区的偏移位置 
	m_nWriteLength = 0;;											//已写数据的长度
	m_nWriteLeavings = SOCKET_IOBUFFER_SIZE;							//剩余可写数据长度

}

//截断缓冲区
bool IOPacket::Resize(long nNewAllSize)
{
	if( nNewAllSize > SOCKET_IOBUFFER_SIZE )
	{
		return false;
	}

	if( m_nWriteLength > nNewAllSize ) 
	{
		m_nWriteLength = nNewAllSize;
		m_pWrite = m_szBuffer+m_nWriteLength;
	}
	m_nWriteLeavings = SOCKET_IOBUFFER_SIZE - nNewAllSize;

	if(m_nReadLength > nNewAllSize)
	{
		m_pRead = m_szBuffer+nNewAllSize;
		m_nReadLength = nNewAllSize;
	}

	m_nReadLeavings = nNewAllSize - m_nReadLength;

	return true;
}


void IOPacket::ReadSkip(long length)
{
	if(m_nReadLeavings - length < 0)
	{
		std::exception s("iopacket readskip error, leaving length < skiplength");
		throw(s);
	}

	m_pRead += length;
	m_nReadLength += length;
	m_nReadLeavings -= length;
}

void IOPacket::WriteSkip(long length)
{
	m_pWrite += length;
	m_nWriteLength += length;
	m_nWriteLeavings -= length;
	if(m_nReadLeavings+m_nReadLength < m_nWriteLength)m_nReadLeavings = m_nWriteLength - m_nReadLength;		//写入时增加读取缓冲区长度

}


/************************************************************************/
/*                         public function                              */
/************************************************************************/

void IOPacket::Clear()
{
	ResetWritePos();
	ResetReadPos();

	memset(m_szBuffer, 0, SOCKET_IOBUFFER_SIZE);
}

//////////////////////////////////////////////////////////////////////////
char IOPacket::ReadChar()
{
	char* p = m_pRead;
	ReadSkip(1);
	return p[0];
}

char* IOPacket::ReadCharArray(long length)
{
	char* p = m_pRead;
	ReadSkip(length);
	return p;
}

short* IOPacket::ReadShortArray(long length)
{
	char* p = m_pRead;
	ReadSkip(length*sizeof(short));
	return (short*)p;
}

long* IOPacket::ReadLongArray(long length)
{
	char* p = m_pRead;
	ReadSkip(length*sizeof(long));
	return (long*)p;
}

__int64* IOPacket::ReadLongLongArray(long length)
{
	char* p = m_pRead;
	ReadSkip(length*sizeof(__int64));
	return (__int64*)p;
}

short IOPacket::ReadShort()
{
	char* p = m_pRead;
	ReadSkip(sizeof(short));
	return *(short*)p;
}

long IOPacket::ReadLong()
{
	char* p = m_pRead;
	ReadSkip(sizeof(long));
	return *(long*)p;
}

__int64	IOPacket::ReadLongLong()
{
	char* p = m_pRead;
	ReadSkip(sizeof(__int64));
	return *(__int64*)p;
}

const TCHAR*	IOPacket::ReadString()
{
	short length = ReadShort();
	if(length > m_nReadLeavings)
	{
		std::exception s("io packet readstring error!");
		throw(s);
	}
	char* p = m_pRead;
	ReadSkip(length);
	return (TCHAR*)p;
}

void* IOPacket::ReadPointer()
{
	__int64 l = *(long*)m_pRead;
	void* p = reinterpret_cast<void*>(l);
	ReadSkip(sizeof(long));
	return p;
}


//////////////////////////////////////////////////////////////////////////
//						write function
//////////////////////////////////////////////////////////////////////////
bool IOPacket::WriteChar(char data)
{
	if(m_nWriteLeavings >= sizeof(char))
	{
		m_pWrite[0] = data;
		WriteSkip(sizeof(char));
		return true;
	}
	return false;
}

bool IOPacket::WriteShortArray(const short* data, long length){return WriteCharArray((const char*)data, length*2);}
bool IOPacket::WriteLongArray(const long* data, long length){return WriteCharArray((const char*)data, length*4);}
bool IOPacket::WriteLongLongArray(const __int64* data, long length){return WriteCharArray((const char*)data, length*8);}
bool IOPacket::WriteCharArray(const char* data, long length)
{
	if(m_nWriteLeavings >= length)
	{
		memcpy(m_pWrite, data, length);
		WriteSkip(length);
		return true;
	}
	return false;
}

bool IOPacket::WriteShort(short data)
{
	if(m_nWriteLeavings >= 2)
	{
		*(short*)m_pWrite = data;
		WriteSkip(2);
		return true;
	}
	return false;
}

bool IOPacket::WriteLong(long data)
{
	if(m_nWriteLeavings >= 4)
	{
		*(long*)m_pWrite = data;
		WriteSkip(4);
		return true;
	}
	return false;
}

bool IOPacket::WriteLongLong(__int64 data)
{
	if(m_nWriteLeavings >= 8)
	{
		*(__int64*)m_pWrite = data;
		WriteSkip(8);
		return true;
	}
	return false;
}

bool IOPacket::WriteString(const TCHAR* data)
{
	if(data == NULL)return false;
	int length = (int)(_tcslen(data)+1)*sizeof(TCHAR);
	if(m_nWriteLeavings >= length)
	{
		WriteShort(length);
		memcpy(m_pWrite, data, length);
		WriteSkip(length);
		return true;
	}
	return false;
}

// bool IOPacket::WriteString(const std::string& data)
// {
// 	return WriteString(data.c_str());
// }

bool IOPacket::WritePointer(void* ptr)
{
	// 32 bit
	return WriteLong(PtrToLong(ptr));
}

bool IOPacket::ReWriteChar(long offset, char data)
{
	if(offset > (long)(m_nWriteLength-sizeof(data)))return false;
	m_szBuffer[offset] = data;
	return true;
}

bool IOPacket::ReWriteShort(long offset, short data)
{
	if(offset > (long)(m_nWriteLength-sizeof(data)))return false;
	*(short*)&m_szBuffer[offset] = data;
	return true;
}

bool IOPacket::ReWriteLong(long offset, long data)
{
	if(offset > (long)(m_nWriteLength-sizeof(data)))return false;
	*(long*)&m_szBuffer[offset] = data;
	return true;
}

bool IOPacket::ReWriteLongLong(long offset, __int64 data)
{
	if(offset > (long)(m_nWriteLength-sizeof(data)))return false;
	*(__int64*)&m_szBuffer[offset] = data;
	return true;
}
