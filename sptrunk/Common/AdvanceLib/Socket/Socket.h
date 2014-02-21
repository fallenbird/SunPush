#pragma once


#define SESSION_POOL_SIZE			10					//连接会话内存池对象初始数量
#define PACKET_POOL_SIZE			500					//消息内存池对象初始数量
#define PACKET_BUFFER_SIZE			8192-48				//单个消息的最大长度
#define SOCKET_IOBUFFER_SIZE		16384-48			//发送/接收缓冲区最大缓冲数据数量(为啥-48 因为sizeof IOPacket == 16384)

#define PACKET_HEADER_LENGTH		4

struct PACKET_HEADER
{
	unsigned int 	m_nDataLength;
	unsigned short	m_nVersionReserved;
	unsigned short	m_nMessageID;
};

struct DATA_HEADER
{
	DATA_HEADER():m_byMaxVersion(0),m_byMinVersion(1),m_byMsgType(0),m_byMsgID(0){}
	DATA_HEADER (unsigned char byMsgType, unsigned char byMsgID):m_byMaxVersion(0),m_byMinVersion(1),m_byMsgType(byMsgType),m_byMsgID(byMsgID){}
	unsigned char			m_byMaxVersion;
	unsigned char			m_byMinVersion;
	unsigned char			m_byMsgType;
	unsigned char			m_byMsgID;
};

//////////////////////////////////////////////////////////////////////////
//开始网络服务管理器, 程序启动时通过单独线程调用, 会阻塞
void	StartSocketService(int thread_num = 5);

//停止网络服务管理器
void	StopSocketService();

//向Socket发送数据
bool	Send(long nConnectID, const char* data, unsigned short length);
bool	SendMulti( long nConnectID, short nMessageCount, const char** ppMsg, short* pwSize );
bool	SendEx(long nConnectID, short nMessageCount, const char** ppMsg, short* pwSize );

//关闭Socket连接(同步关闭)
void	CloseSocket(long nConnectID);
//关闭Socket连接(异步关闭)
void	AsyncClose(long nConnectID, long nWaitSeconds = 10);

//////////////////////////////////////////////////////////////////////////
//							提供数据统计接口
class Socket
{
public:
	static	volatile	long	s_nTotalThread;				//总网络线程数量		
	static	volatile	long	s_nTotalSession;			//所有会话总数		
	static	volatile	long	s_nTotalListening;			//正在监听的会话数量
	static	volatile	long	s_nTotalConnecting;			//正在连接中的会话
	static	volatile	long	s_nTotalConneced;			//已经与服务器建立连接的客户端数量
	static	volatile	long	s_nTotalClosed;				//总连接已关闭的会话数量

	static	volatile	long	s_nTotalDataSend;			//总数据发送数量
	static	volatile	long	s_nTotalDataRecv;			//总数据接收数量
	static	volatile	long	s_nTotalDataSendWait;		//总等待数据发送数量
	static	volatile	long	s_nTotalDataSendDiscard;	//总数据发送丢弃数量
	static	volatile	long	s_nTotalDataRecvWait;		//总等待数据接收数量

	static	volatile	long	s_nTotalDataSendAvg;		//总平均每分数据发送数量
	static	volatile	long	s_nTotalDataRecvAvg;		//总平均每分数据接收数量

	static	volatile	long	s_nTotalMsgRecvCount;		//总接收消息量
	static	volatile	long	s_nTotalMsgSendCount;		//总发送消息量

	static	volatile	long	s_nNetMsgObjCount;			//消息对象在内存池中已分配的数量

};

