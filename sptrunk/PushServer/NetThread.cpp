#include "CommonHeader.h"
#include "NetThread.h"
#include "WebSvrConnector.h"
#include "ClientAcceptor.h"
#include "DBConnector.h"
#include "ServerConfig.h"


ClientAcceptor*			NetThread::m_pClientAcceptor;
WebSvrConnector*		NetThread::m_pWebSvrConnector;
DBConnector*			NetThread::m_pDBConnector;


void NetThread::StartNetThread( int iThreadCnt )
{
	_beginthreadex( NULL, NULL, NetThread::NetThreadPorc, (void*)iThreadCnt, NULL, NULL );
}


void NetThread::StopNetThread()
{
	printf("Client AcceptorÍ£Ö¹¼àÌý, IP[%s:%d]!", theConfig.m_szClientListenIP, theConfig.m_nClientListenPort);
	if( m_pClientAcceptor )
	{ 
		m_pClientAcceptor->StopListen();
	}

	StopSocketService();
	printf(" ASIOÍøÂçÍ£Ö¹!");
}


unsigned int _stdcall NetThread::NetThreadPorc( void* pPara )
{
	m_pClientAcceptor = new ClientAcceptor( theConfig.m_nClientListenPort );
	if(!m_pClientAcceptor->StartListen() )
	{
		return FALSE;
	}
	printf("Client Acceptor Listen, IP[%s:%d]  Started!\n", theConfig.m_szClientListenIP, theConfig.m_nClientListenPort);

	//m_pDBConnector = new DBConnector;
	//m_pDBConnector->AsyncConnect( theConfig.m_szDBServerConnectIP, theConfig.m_nDBServerConnectPort );
	//printf( "Connect Begin Connect DBServer:[%s:%d]", theConfig.m_szDBServerConnectIP, theConfig.m_nDBServerConnectPort);

	m_pWebSvrConnector = new WebSvrConnector;
	m_pWebSvrConnector->AsyncConnect( "127.0.0.1",  33066 );//theConfig.m_nWebServerConnectPort  theConfig.m_szWebServerConnectIP
	printf( "Begin Connect DBServer:[%s:%d]\n", theConfig.m_szWebServerConnectIP, theConfig.m_nWebServerConnectPort );

	StartSocketService( theConfig.m_nSocketThreadCnt );
	return 0;
}
