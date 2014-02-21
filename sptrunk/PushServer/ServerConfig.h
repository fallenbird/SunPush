#pragma once


enum eSVR_TYPE
{
	NULL_SERVER 			= 0,
	AGENT_SERVER			= 1,
	PUSH_SERVER				= 2,
	DATA_SERVER				= 3,
};


#define		DEFAULT_CHANNEL_UPDATE_TIMER		5					//默认定时刷新频道列表到客户端的间隔时间(s) 
#define		DEFAULT_PCCU_UPDATE_TIMER			180					//默认定时记录PCCU的间隔时间(s) 

#define		DEFAULT_DBSVR_CONNECT_IP			"127.0.0.1"			//默认连接 DB数据库服务器的IP地址
#define		DEFAULT_DBSVR_CONNECT_PORT			19010				//默认连接 DB数据库服务器的端口
#define		DEFAULT_WEBSVR_CONNECT_IP			"0.0.0.0"			//默认监听 Web服务器的IP地址
#define		DEFAULT_WEBSVR_CONNECT_PORT			20120				//默认监听 Web服务器的端口
#define		DEFAULT_CLIENT_LISTEN_IP			"0.0.0.0"			//默认监听 客户端的IP地址
#define		DEFAULT_CLIENT_LISTEN_PORT			44405				//默认监听 客户端的端口


struct ServerConfig
{
public:
	ServerConfig()
	{
		memset(this, 0x0, sizeof(ServerConfig));
	}

	BOOL		LoadConfigFile( const char* lpszFileName = "PushServer.ini" );			// --加载配置文件

	char		m_szDBServerConnectIP[16];			// --连接 DB数服务器的IP地址
	char		m_szWebServerConnectIP[16];			// --连接 Web服务器的IP地址
	char		m_szClientListenIP[16];				// --监听 客户端的IP地址
	UINT16		m_nDBServerConnectPort;				// --连接 DB数据库服务器的端口
	UINT16		m_nWebServerConnectPort;			// --连接 Web服务器的端口
	UINT16		m_nClientListenPort;				// --监听 客户端的端口
	int			m_nSocketThreadCnt;					// --	
	static		eSVR_TYPE	s_emServerType;			// --服务器类型
};

extern ServerConfig	theConfig;