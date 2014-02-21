// PushServer.cpp : 定义控制台应用程序的入口点。
//

#include "CommonHeader.h"
#include "PushServer.h"
#include "NetThread.h"



int main( int argc, char* argv )
{
	printf( "PushServer正在启动...\n");
	PushServer m_PushServer;
	if( !m_PushServer.InitServer() )
	{
		printf( "PushServer Initialization Failed! \n" );
		system( "pause" );
		return 0;
	}

	printf( "Start Server Start Success! \n");
	m_PushServer.RunServer();

	// --即时关闭
	printf( "Server Is Terminated... \n" );
	m_PushServer.Release();
	NetThread::StopNetThread();
	return 0;
}

