#include "../CommonHeader.h"
#include "Socket.h"
#include "IOPacket.h"
#include "Session.h"
#include "Acceptor.h"
#include "Connector.h"
#include "Socket.h"
#include "MessageEncrypt.h"
#include <boost/enable_shared_from_this.hpp>
#include "NetMsg.h"

extern boost::asio::io_service			g_io_service;
extern boost::object_pool<CNetMsg>		g_poolMessage;
extern bool		g_bStopSocketFlag;


//////////////////////////////////////////////////////////////////////////

SessionManager::SessionManager()
{
}
SessionManager::~SessionManager()
{
}

SessionManager* SessionManager::s_instance = NULL;
SessionManager* SessionManager::GetInstance()
{
	if(s_instance == NULL)
	{
		s_instance = new SessionManager();
	}
	return s_instance;
}

//从内存池分配一个会话对象 
Session* SessionManager::AllocSession()
{
	boost::mutex::scoped_lock lock(m_mutex);
	Session* pSession = m_poolSession.construct();
	if(pSession==NULL)
	{
		theConsole.Error(_T("Acceptor"), _T("m_poolSession.Construct Failure!"));
		return NULL;
	}
	return pSession;
}

//从内存池释放一个会话对象
void SessionManager::DestroySession(Session* pSession)
{
	boost::mutex::scoped_lock lock(m_mutex);
	m_poolSession.destroy(pSession);
}

void SessionManager::RemoveAllSession()
{
	boost::mutex::scoped_lock lock(m_mutex);

	std::map<long, Session*>::iterator ite = m_mapSession.begin();
	for(ite; ite!=m_mapSession.end(); ++ite)
	{
		Session* pSession = ite->second;
		boost::system::error_code ec;
		pSession->socket_.cancel(ec);

		m_poolSession.destroy(pSession);

	}

	m_mapSession.clear();
}

void SessionManager::Release()
{
	delete s_instance;
	s_instance = NULL;
}

void SessionManager::UpdateSession(long nConnectID, Session* pSession)
{
	boost::mutex::scoped_lock lock(m_mutex);

	m_mapSession[nConnectID] = pSession;
}

void SessionManager::RemoveSession(long nConnectID)
{
	boost::mutex::scoped_lock lock(m_mutex);

	m_mapSession.erase(nConnectID);
}

Session* SessionManager::GetSession(long nConnectID)
{
	boost::mutex::scoped_lock lock(m_mutex);

	std::map<long, Session*>::iterator ite = m_mapSession.find(nConnectID);
	if(ite == m_mapSession.end())return NULL;

	return ite->second;
}

UINT32 SessionManager::GetSessionCount()
{
	return (UINT32)m_mapSession.size();
}

UINT32 SessionManager::GetSessionColSize()
{
	return (UINT32)m_poolSession.get_next_size();
}

UINT32 SessionManager::GetMessageColSize()
{
	return (UINT32)g_poolMessage.get_next_size();
}

//////////////////////////////////////////////////////////////////////////


//当前连接最高的ID
volatile long Session::s_global_current_connexindex = 1;

Session::Session():socket_(g_io_service), timer_(g_io_service),	strand_(g_io_service)
{
	m_emSocketStatus = SS_IDLE;
	m_connindex = InterlockedIncrement(&s_global_current_connexindex);
	m_nLocalPort = 0;
	m_nRemotePort = 0;

	m_pAcceptor = NULL;
	m_pConnector = NULL;

	_tcscpy_s(m_strLocalIP, _T("0.0.0.0"));
	_tcscpy_s(m_strRemoteIP, _T("0.0.0.0"));

	InterlockedIncrement((long*)&Socket::s_nTotalSession);

}

Session::~Session()
{
	m_pAcceptor = NULL;
	m_pConnector = NULL;

	InterlockedDecrement((long*)&Socket::s_nTotalSession);
}

void Session::start()
{

	//boost::asio::socket_base::send_buffer_size option_send(65535);
	//socket_.set_option(option_send);
	//boost::asio::socket_base::receive_buffer_size option_recv(65535);
	//socket_.set_option(option_recv);

#ifndef _BIG_BUFFER_MODE
	m_nSmallBufferModeReadLeave = -1;
	m_nSmallBufferModeReadOffset = 0;	
	CNetMsgPtr netmsg_ptr = GetPoolMessage();
	_receive_sp(netmsg_ptr, 0, PACKET_HEADER_LENGTH);
#else
	socket_.async_receive(boost::asio::buffer(data_, read_buffer_.GetWriteLeavings()),
		strand_.wrap(boost::bind(&Session::handle_read, this, 	
		boost::asio::placeholders::error,
		boost::asio::placeholders::bytes_transferred)));
#endif

	//size_t cancelcnt = timer_.expires_from_now(boost::posix_time::seconds(10));
	////@return The number of asynchronous operations that were cancelled;第一次启动定时器
	//std::cout << "timer canceled count is = " << cancelcnt << std::endl;

	//timer_.async_wait(strand_.wrap(boost::bind(&Session::handle_timeout,this,
	//	boost::asio::placeholders::error)));        
}

#ifndef _BIG_BUFFER_MODE

void Session::_receive_sp(CNetMsgPtr spMsg, long nRecvOffset, long nRecvLength )
{
	 boost::arg<2> iTransBytes = boost::asio::placeholders::bytes_transferred;
	socket_.async_receive( boost::asio::buffer(spMsg->GetBuf()+nRecvOffset, nRecvLength),
		strand_.wrap(boost::bind(&Session::handle_read_sp, this, spMsg,		
		boost::asio::placeholders::error, nRecvLength ) ) );
}

void Session::handle_read_sp(CNetMsgPtr spMsgBuffer, const boost::system::error_code& error, size_t bytes_transferred)
{
	Socket::s_nTotalDataRecv += (long)bytes_transferred;
	if(g_bStopSocketFlag || m_emSocketStatus == SS_CLOSED || m_emSocketStatus == SS_ERROR)return;

	if(bytes_transferred == 0 || (bytes_transferred == -1 && WSAGetLastError() != WSAEWOULDBLOCK))
	{
#ifdef _UNICODE
		TCHAR buf[512];
		MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, boost::system::system_error(error).what(), -1, buf, 512);
		return handle_exit(buf);
#else
		return handle_exit(boost::system::system_error(error).what());
#endif
	}
	if(m_nSmallBufferModeReadLeave == -1 && bytes_transferred == PACKET_HEADER_LENGTH)
	{
		PACKET_HEADER* pHeader = (PACKET_HEADER*)spMsgBuffer->GetBuf();
		UINT16 nMessageLength = (UINT16)pHeader->m_nDataLength /*- 4*/;
		m_nSmallBufferModeReadLeave = nMessageLength;
		m_nSmallBufferModeReadOffset = 0;
		if(spMsgBuffer->GetMaxSize() < nMessageLength)
		{
			//如果消息大于接收缓冲区， 则接收并丢弃
			TCHAR szBuffer[512];
			_stprintf_s(szBuffer, _countof(szBuffer), _T("MsgLen:[%d] NetMsg Limit:[%d], Please Reset NetworkMessage Limit!Macro:[_MAX_MSGSIZE]"), nMessageLength, spMsgBuffer->GetMaxSize());
			//::MessageBox(NULL, szBuffer, _T("Error"), MB_OK|MB_ICONERROR);
			assert(szBuffer);
			theConsole.Error(_T("Discard1"), szBuffer);
			_receive_sp(spMsgBuffer, 0, spMsgBuffer->GetMaxSize());
		}
		else if(nMessageLength > 0)
		{
			_receive_sp(spMsgBuffer, 0, nMessageLength);
		}
		else if(nMessageLength == 0)
		{
			//收到0个数据内容的消息， 继续接受下一条消息
			m_nSmallBufferModeReadLeave = -1;
			m_nSmallBufferModeReadOffset = 0;
			CNetMsgPtr netmsg_ptr = GetPoolMessage();
			if(netmsg_ptr.get()==NULL)
			{
				TCHAR buff[512];
				_stprintf_s(buff, _countof(buff), _T("分配消息对象内存失败！无可用内存。总消息量:[%d], 总用内存:[%dKB]"), Socket::s_nNetMsgObjCount, Socket::s_nNetMsgObjCount*_MAX_MSGSIZE/1000);
				theConsole.Error(_T("Socket"), buff);
				return handle_exit(buff);
			}
			_receive_sp(netmsg_ptr, 0, PACKET_HEADER_LENGTH); 
		}
	}
	else
	{
		m_nSmallBufferModeReadLeave -= bytes_transferred;
		m_nSmallBufferModeReadOffset += bytes_transferred;
		if(m_nSmallBufferModeReadLeave == 0)
		{
			//回调上层
			if(spMsgBuffer->GetMaxSize() < m_nSmallBufferModeReadOffset+m_nSmallBufferModeReadLeave)
			{
				theConsole.Error(_T("Discard3"), _T("Discard TooBig Message! MsgLen:[%d]"), m_nSmallBufferModeReadOffset);
			}
			else
			{
				InterlockedIncrement((long*)&Socket::s_nTotalMsgRecvCount);
				//在这里检查Magic Code
				PACKET_HEADER* pHeader = (PACKET_HEADER*)spMsgBuffer->GetBuf();
				spMsgBuffer->m_dwMsgSize = m_nSmallBufferModeReadOffset/*-PACKET_HEADER_LENGTH*/;

  				char connbuf[PACKET_BUFFER_SIZE];
  				char tempbuf[512];
  				sprintf(connbuf, "HEADER(%d)=", PACKET_HEADER_LENGTH);
  				for(int i=0; i<PACKET_HEADER_LENGTH; i++)
  				{
  					sprintf(tempbuf, "%02X ", (char)*(char*)(spMsgBuffer->GetBuf()+i));
  					if (strlen(tempbuf)<=3)strcat(connbuf, tempbuf);
  					else strcat(connbuf, tempbuf+6);
  				}
  				theConsole.Trace("Receive","%s", connbuf);
  
  				sprintf(connbuf, "BODY(%d)=", spMsgBuffer->GetSize());
  				for(int i=0; i<spMsgBuffer->GetSize(); i++)
  				{
  					if(i>100){strcat(connbuf,"...");break;}
  					sprintf(tempbuf, "%02X ", (char)*(char*)(spMsgBuffer->GetBuf()+PACKET_HEADER_LENGTH+i));
  					//while(strlen(tempbuf)>3)strcpy(tempbuf, tempbuf+1);
  					if (strlen(tempbuf)<=3)strcat(connbuf, tempbuf);
  					else strcat(connbuf, tempbuf+6);
  				}
  				theConsole.Trace("Receive","%s", connbuf);
  
 
// 				if(pHeader->m_nMagicCode != 0x8888)									//检查 Magic Code
// 				{
// 					theConsole.Error("Receive", "Magic Code Error!, Code:[0x%X], MSGID=[%d]", pHeader->m_nMagicCode, pHeader->m_nMessageID);
// 					return;
// 				}

				if(m_pAcceptor)m_pAcceptor->OnReadComplete(m_connindex, spMsgBuffer, m_nSmallBufferModeReadOffset);
 				else if(m_pConnector)m_pConnector->OnReadComplete(m_connindex, spMsgBuffer, m_nSmallBufferModeReadOffset);
			}

			m_nSmallBufferModeReadLeave = -1;
			m_nSmallBufferModeReadOffset = 0;
			CNetMsgPtr netmsg_ptr = GetPoolMessage();
			if(netmsg_ptr.get()==NULL)
			{
				TCHAR buff[512];
				_stprintf_s(buff, _countof(buff), _T("分配消息对象内存失败！无可用内存。总消息量:[%d], 总用内存:[%dKB]"), Socket::s_nNetMsgObjCount, Socket::s_nNetMsgObjCount*_MAX_MSGSIZE/1000);
				theConsole.Error(_T("Socket"), buff);
				return handle_exit(buff);
			}
			_receive_sp(netmsg_ptr, 0, PACKET_HEADER_LENGTH);
		}
		else if(spMsgBuffer->GetMaxSize() < m_nSmallBufferModeReadOffset+m_nSmallBufferModeReadLeave)
		{
			//如果消息大于接收缓冲区， 则接收并丢弃
			if(bytes_transferred-m_nSmallBufferModeReadOffset==0)
			{
				PACKET_HEADER* pHeader = (PACKET_HEADER*)spMsgBuffer->GetBuf();
				theConsole.Error(_T("Discard2"), _T("Discard TooBig Message!RecvLen:[%d], MsgCP:[%d/%d]"), bytes_transferred, HIBYTE(pHeader->m_nMessageID), LOBYTE(pHeader));
			}
			_receive_sp(spMsgBuffer, 0, min(m_nSmallBufferModeReadLeave, spMsgBuffer->GetMaxSize()));
		}
		else if(m_nSmallBufferModeReadLeave > 0)
		{
			_receive_sp(spMsgBuffer, m_nSmallBufferModeReadOffset, m_nSmallBufferModeReadLeave);
		} 
	}
}
#else
void Session::handle_read(const boost::system::error_code& error, size_t bytes_transferred)
{
	Socket::s_nTotalDataRecv += (long)bytes_transferred;
	if(g_bStopSocketFlag || m_emSocketStatus == SS_CLOSED || m_emSocketStatus == SS_ERROR)return;

	if(error)
	{
		//if(error.value() == 995)return;		//由于线程退出或应用程序请求，已放弃 I/O 操作。
#ifdef DEBUGINFO_RECEIVE
		theConsole.Warning("Receive", "handle_read-[%s]", boost::system::system_error(error).what());
#endif
		return handle_exit(boost::system::system_error(error).what());
	}

#ifdef DEBUGINFO_RECEIVE
	theConsole.Debug("Receive","CID[%d] From[%s:%d]，Length Current/Addon=[%d/+%d]", m_connindex, GetRemoteIP(), GetRemotePort(),
		read_buffer_.GetWriteLength(), bytes_transferred);
#endif

	if (bytes_transferred <= 0)
	{
#ifdef DEBUGINFO_CONNECT
		return handle_exit("远程主机主动关闭了一个现有连接!");
#endif
	}

	//1. 先将所接到的数据全部放到[读取缓冲区中]
	if(!read_buffer_.WriteCharArray(data_, (long)bytes_transferred))
	{

		char buff[512];
#ifdef DEBUGINFO_RECEIVE
		theConsole.Warning("Receive", "Message TooBig， No Buffer！L=[%d/%d]", read_buffer_.GetWriteLength()+bytes_transferred, SOCKET_IOBUFFER_SIZE);
#endif
		sprintf(buff, "数据长度溢出, 已超出接收缓冲区容量! L=[%u/%u]", read_buffer_.GetWriteLength()+bytes_transferred, SOCKET_IOBUFFER_SIZE);
		return handle_exit(buff);
	}

	//2. 循环开始解决数据缓冲区中的数据，并整合消息回调事件
	while(true)
	{
		if(read_buffer_.GetReadLeavings() < PACKET_HEADER_LENGTH)			//缓冲区剩余数据不足一个消息头
		{
			socket_.async_receive(boost::asio::buffer(data_, read_buffer_.GetWriteLeavings()),
				strand_.wrap(boost::bind(&Session::handle_read, this,
				boost::asio::placeholders::error,
				boost::asio::placeholders::bytes_transferred)));
#ifdef DEBUGINFO_RECEIVE
			theConsole.Warning("Receive", "Header Not Integrity, Continue Receive! [%d/%d]", read_buffer_.GetReadLeavings(), PACKET_HEADER_LENGTH);
#endif
			break;
		}

		//消息头数据已经全部接收
		PACKET_HEADER* pHeader = (PACKET_HEADER*)read_buffer_.GetBuffer();	//取消息头
		UINT16 nMessageSize = pHeader->m_nDataLength;
		assert((short)nMessageSize > 0); //  注释掉这行，有问题就close掉，添加以下判断 Will [2010-12-10 11:24:37 ]
		if (nMessageSize == 0)	Close();
#ifdef USE_ENCRYPT_DATA		//解密操作
		m_encryptRecv.Decrypt((unsigned char *)&nMessageSize, 2, false);	// false: 不移动加密指针
#endif
		if(nMessageSize > PACKET_BUFFER_SIZE)
		{
			char buff[512];
			sprintf(buff, "消息头记录的长度溢出, 已超出接收缓冲区容量! (是否使用了不同的加密算法?)L=[%u/%u]", read_buffer_.GetWriteLength()+bytes_transferred, SOCKET_IOBUFFER_SIZE);
			theConsole.Warning("Receive", "Data TooBig, No Buffer! MSGCP:[%d/%d]? L=[%d/%d]", ((char*)pHeader)[0], ((char*)pHeader)[1], pHeader->m_nDataLength, SOCKET_IOBUFFER_SIZE);
			return handle_exit(buff);
		}
		//if(pHeader->m_nMagic != 0x0000)									//检查 Magic Code
		//{
		//	theConsole.Error("Receive", "Magic Code Error!, MSGID=[%d]", pHeader->m_nMessageID);
		//	return;
		//}

		//char connbuf[PACKET_BUFFER_SIZE];
		//char tempbuf[512];
		//sprintf(connbuf, "HEADER(6)=", pHeader->m_nDataLength);
		//for(int i=0; i<PACKET_HEADER_LENGTH; i++)
		//{
		//	sprintf(tempbuf, "%02X ", (char)*(char*)(read_buffer_.GetBuffer()+i));
		//	strcat(connbuf, tempbuf);
		//}
		//sprintf(tempbuf, " BODY(%d)=", pHeader->m_nDataLength);
		//strcat(connbuf, tempbuf);
		//for(int i=0; i<pHeader->m_nDataLength; i++)
		//{
		//	sprintf(tempbuf, "%02X ", (char)*(char*)(read_buffer_.GetBuffer()+PACKET_HEADER_LENGTH+i));
		//	while(strlen(tempbuf)>3)strcpy(tempbuf, tempbuf+1);
		//	strcat(connbuf, tempbuf);
		//}
		//theConsole.Debug("Receive","%s", connbuf);

		if(nMessageSize+PACKET_HEADER_LENGTH > read_buffer_.GetWriteLength())
		{
			//已收到的数据不足一个消息实体的数据
			//memset(data_, 0xCC, sizeof(data_));

			socket_.async_receive(boost::asio::buffer(data_, read_buffer_.GetWriteLeavings()),
				strand_.wrap(boost::bind(&Session::handle_read, this,
				boost::asio::placeholders::error,
				boost::asio::placeholders::bytes_transferred)));

#ifdef DEBUGINFO_RECEIVE
			theConsole.Debug("Receive", "Message Not Integrity, Continue Receive! [%d/%d]", read_buffer_.GetReadLeavings(), pHeader->m_nDataLength);
#endif

			break;
		}

#ifdef DEBUGINFO_RECEIVE
		theConsole.Notice("Receive", "Receive Complete, TotalLength[%d], BufferLength=[%d]",nMessageSize, read_buffer_.GetWriteLength());
#endif

		//////////////////////////////////////////////////////////////////////////
		//3. 在这里整合一个消息，并回调上层事件
		read_buffer_.ResetReadPos();
		CNetMsgPtr netmsg_ptr = GetPoolMessage();
		//memcpy(data_, read_buffer_.ReadCharArray(nMessageSize), nMessageSize);
		read_buffer_.ReadCharArray(sizeof(PACKET_HEADER));
		//assert(netmsg_ptr->GetMaxSize() >= nMessageSize);	// 注释掉此行，添加以下判断 Will [2010-12-10 11:26:28 ]
		if(netmsg_ptr->GetMaxSize() < nMessageSize) Close();//  Will [2010-12-10 11:27:25 ]
		memcpy(netmsg_ptr->GetBuf(), read_buffer_.ReadCharArray(nMessageSize), nMessageSize);

#ifdef USE_ENCRYPT_DATA		//解密操作
		m_encryptRecv.Decrypt((UINT8*)netmsg_ptr->m_bufMsg, nMessageSize);
#endif
		if(m_pAcceptor)m_pAcceptor->OnReadComplete(m_connindex, netmsg_ptr, nMessageSize);
		else if(m_pConnector)m_pConnector->OnReadComplete(m_connindex, netmsg_ptr, nMessageSize);

		//4. 将已处理的消息数据从缓冲区中移除
		int nLeavingLength = read_buffer_.GetReadLeavings();
		if(nLeavingLength>0)memcpy(data_, read_buffer_.GetBuffer() + nMessageSize+sizeof(PACKET_HEADER), nLeavingLength);
		read_buffer_.Clear();
		if(nLeavingLength>0)read_buffer_.WriteCharArray(data_, nLeavingLength);

		//5. 如果缓冲区剩余数据，则继续接收， 否则的话， 就从新接收消息头数据
		if(nLeavingLength >= PACKET_HEADER_LENGTH)
		{
			PACKET_HEADER* pHeader = (PACKET_HEADER*)read_buffer_.GetBuffer();
			nMessageSize = pHeader->m_nDataLength;
#ifdef USE_ENCRYPT_DATA		//解密操作
			m_encryptRecv.Decrypt((unsigned char *)&nMessageSize, 2, false);	// false: 不移动加密指针
#endif
			if(nMessageSize <= nLeavingLength)continue;		//还够一条消息的
		}

#ifdef DEBUGINFO_RECEIVE
		theConsole.Debug("Receive", "No Integrity Message, Leave[%d]", nLeavingLength);
#endif

		memset(data_, 0xCC, sizeof(data_));
		socket_.async_receive(boost::asio::buffer(data_, read_buffer_.GetWriteLeavings()),
			strand_.wrap(boost::bind(&Session::handle_read, this,
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred)));

		break;
	}
}
#endif

#ifdef _USE_PACKET_MEMORY_POOL_SEND
void Session::Send(CNetMsgPtr msgptr, int length)
//void Session::Send(const char* data, int length)
{
	if(m_emSocketStatus == SS_CLOSED || m_emSocketStatus == SS_ERROR)return;
	InterlockedIncrement((long*)&Socket::s_nTotalMsgSendCount);

#ifdef DEBUGINFO_SEND
	PACKET_HEADER* pHeader = (PACKET_HEADER*)data;
	//	theConsole.Debug("Send", "Send Message CID:[%d], MID:[%d], Len:[%d]", m_connindex, pHeader->m_nMessageID, length);
	//	assert(pHeader->m_nDataLength == length);
#endif

	const char* pFinalSendData = msgptr->GetBuf();
#ifdef USE_ENCRYPT_DATA							//加密操作, 将发送数据进行复制
	char buff[PACKET_BUFFER_SIZE];
	memcpy(buff, msgptr->GetBuf(), length);
	m_encryptSend.Encrypt((unsigned char *)buff, length);
	pFinalSendData = buff;
#endif

	boost::asio::const_buffers_1 buffer(pFinalSendData, length);
	boost::system::error_code ec;
	boost::asio::async_write(socket_,buffer,strand_.wrap(boost::bind(&Session::handle_write,
		this,boost::asio::placeholders::error, msgptr,length)));


	InterlockedExchange(&Socket::s_nTotalDataSendWait, Socket::s_nTotalDataSendWait+length);
	InterlockedIncrement(&Socket::s_nNetMsgSendWaitCount);
}
#else
void Session::Send(const char* data, int length)
{
	if(m_emSocketStatus == SS_CLOSED || m_emSocketStatus == SS_ERROR)return;
	InterlockedIncrement((long*)&Socket::s_nTotalMsgSendCount);

#ifdef DEBUGINFO_SEND
	PACKET_HEADER* pHeader = (PACKET_HEADER*)data;
	//	theConsole.Debug("Send", "Send Message CID:[%d], MID:[%d], Len:[%d]", m_connindex, pHeader->m_nMessageID, length);
	//	assert(pHeader->m_nDataLength == length);
#endif

	const char* pFinalSendData = data;
#ifdef USE_ENCRYPT_DATA							//加密操作, 将发送数据进行复制
	char buff[PACKET_BUFFER_SIZE];
	memcpy(buff, data,length);
	m_encryptSend.Encrypt((unsigned char *)buff, length);
	pFinalSendData = buff;
#endif

	boost::asio::const_buffers_1 buffer(pFinalSendData, length);
	boost::system::error_code ec;
	boost::asio::async_write(socket_,buffer,strand_.wrap(boost::bind(&Session::handle_write,
		this,boost::asio::placeholders::error, data,length)));


	InterlockedExchange(&Socket::s_nTotalDataSendWait, Socket::s_nTotalDataSendWait+length);
}
#endif


void Session::Close(long nWaitSeconds)
{
	if(nWaitSeconds != 0)
	{
		timer_.expires_from_now(boost::posix_time::seconds(nWaitSeconds));
		timer_.async_wait(boost::bind(&Session::handle_exit, this, _T("本地机器主动关闭了一个现有连接!")));
	}
	else
	{
		handle_exit(_T("本地机器主动关闭了一个现有连接!"));
	}
}

#ifdef _USE_PACKET_MEMORY_POOL_SEND
void Session::handle_write(const boost::system::error_code& error, CNetMsgPtr spMsg, size_t bytes_transferred)
#else
void Session::handle_write(const boost::system::error_code& error, const char* spMsg, size_t bytes_transferred)
#endif
{
	if(m_emSocketStatus == SS_CLOSED || m_emSocketStatus == SS_ERROR)return;

	if(m_pAcceptor)m_pAcceptor->OnSend(m_connindex, (long)bytes_transferred);
	else if(m_pConnector)m_pConnector->OnSend(m_connindex, (long)bytes_transferred);

	if (!error)
	{
		InterlockedExchange(&Socket::s_nTotalDataSendWait, (long)Socket::s_nTotalDataSendWait-bytes_transferred);
		Socket::s_nTotalDataSend += (long)bytes_transferred;
	}
	else
	{
		//__asm int 3;  //  Will [2011-1-26 13:56:55 ] 调试用
		Socket::s_nTotalDataSendDiscard += (long)bytes_transferred;
		theConsole.Error(_T("Socket"), _T("Send Fail! CID:[%d], [%d]Bytes, [%s]"), m_connindex, (UINT32)bytes_transferred, boost::system::system_error(error).what());
	}

}

void Session::handle_timeout(const boost::system::error_code& e)
{
	if(m_emSocketStatus == SS_CLOSED || m_emSocketStatus == SS_ERROR)return;

	size_t ret = this->socket_.send(boost::asio::const_buffers_1("", 1));

	if (e != boost::asio::error::operation_aborted)
	{
		std::cout << "timeout now.\n";    
		//	this->handle_exit();
	}
	else
	{//被取消
		std::cout << "cancelled now.\n";
	}            
	return;
}

void Session::handle_exit(const TCHAR* szDesc)
{
	if(m_emSocketStatus == SS_CLOSED || m_emSocketStatus == SS_ERROR)return;

	this->m_emSocketStatus = SS_CLOSED;
	InterlockedIncrement((long*)&Socket::s_nTotalClosed);

#ifdef DEBUGINFO_CONNECT
		theConsole.Debug(_T("Close"), _T("CID[%d] Remote Connect Closed, IP=[%s], Port=[%d]"), m_connindex, GetRemoteIP(), GetRemotePort());
#endif
	if(m_pAcceptor)
	{
		m_pAcceptor->OnClose(this->m_connindex, szDesc);
		InterlockedDecrement((long*)&Socket::s_nTotalConneced);
		m_pAcceptor = NULL;
	}
	else if(m_pConnector/* && m_pConnector->m_pConnectSession*/)
	{
		m_pConnector->OnClose(this->m_connindex, szDesc);
		InterlockedDecrement((long*)&Socket::s_nTotalConnecting);
		m_pConnector->m_pConnectSession = NULL;
		m_pConnector = NULL;
	}
	else
	{
		theConsole.Error(_T("Close"), _T("handle_exit Occur Error! Very Serious!"));
		//__asm int 3; //  Will [2011-2-27 6:46:03 ] 很危险
	}

	if (!SessionManager::GetInstance()->GetSession(m_connindex))
	{
		//ASSERT("重复调用Session::handle_exit");
		theConsole.Error(_T("RemoveSession"), _T("重复调用Session::handle_exit connindex: %d"), m_connindex);
		socket_.close();
		return;
	}
	SessionManager::GetInstance()->RemoveSession(m_connindex);
	socket_.close();


	/**//*
		Any asynchronous send, receive
		or connect operations will be cancelled immediately, and will complete
		with the boost::asio::error::operation_aborted error
		*/


	timer_.expires_from_now(boost::posix_time::seconds(10)); //  Will [2011-2-27 5:25:13 ] 不明白为什么需要等待,但好像不等会有问题，问下夏，这里由5改成10更好点可能
	timer_.async_wait(boost::bind(&Session::handle_destory, this, boost::asio::placeholders::error));

}

void Session::handle_destory(const boost::system::error_code& error)
{
#ifdef DEBUGINFO_CONNECT
	theConsole.Debug(_T("Close"), _T("CID[%d] Remote Connect Session Destoryed, IP=[%s], Port=[%d]"), m_connindex, GetRemoteIP(), GetRemotePort());
#endif

	SessionManager::GetInstance()->DestroySession(this);
	InterlockedDecrement((long*)&Socket::s_nTotalClosed);
}

//////////////////////////////////////////////////////////////////////////

const TCHAR*	Session::GetLocalIP()
{
	if(_tcslen(m_strLocalIP)>7)return m_strLocalIP;
#ifdef _UNICODE
	MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, socket_.local_endpoint().address().to_string().c_str(), -1, m_strLocalIP, 16);
#else
	_tcscpy(m_strLocalIP, socket_.local_endpoint().address().to_string().c_str());
#endif
	return m_strLocalIP;
}

UINT16	Session::GetLocalPort()
{
	if(m_nLocalPort==0)m_nLocalPort = socket_.local_endpoint().port();
	return m_nLocalPort;
}

const char*	Session::GetRemoteIP()
{
	if(_tcslen(m_strRemoteIP)>7)return m_strRemoteIP;
	if(m_emSocketStatus == SS_CLOSED)return _T("0.0.0.0");
#ifdef _UNICODE
	MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, socket_.remote_endpoint().address().to_string().c_str(), -1, m_strRemoteIP, 16);
#else
	_tcscpy(m_strRemoteIP, socket_.remote_endpoint().address().to_string().c_str());
#endif
	return m_strRemoteIP;
}

UINT16	Session::GetRemotePort()
{
	if(!socket_.is_open())return 0;
	if(m_nRemotePort==0)m_nRemotePort = socket_.remote_endpoint().port();
	return m_nRemotePort;
}

SOCKET_STATUS Session::GetSocketStatus()
{
	return m_emSocketStatus;
}
