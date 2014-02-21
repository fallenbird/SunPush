#include "CommonHeader.h"
#include "ServerConfig.h"

ServerConfig  theConfig;

eSVR_TYPE	ServerConfig::s_emServerType;					// --服务器类型

BOOL ServerConfig::LoadConfigFile( const char* lpszFileName /*= _T("PushServer.ini") */ )
{
	memcpy_s( m_szDBServerConnectIP, 16, DEFAULT_DBSVR_CONNECT_IP, 16 );
	memcpy_s( m_szWebServerConnectIP, 16, DEFAULT_WEBSVR_CONNECT_IP, 16 );
	memcpy_s( m_szClientListenIP, 16, DEFAULT_CLIENT_LISTEN_IP, 16 );

	m_nDBServerConnectPort = DEFAULT_DBSVR_CONNECT_PORT;
	m_nWebServerConnectPort = DEFAULT_WEBSVR_CONNECT_PORT;
	m_nClientListenPort = DEFAULT_CLIENT_LISTEN_PORT;	
	m_nSocketThreadCnt = 5;
	return TRUE;
}
