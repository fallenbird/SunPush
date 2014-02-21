#include "CommonHeader.h"
#include "WebSvrConnector.h"

#include "MsgHandler.h"
#include "MsgStruct_CS.h"



extern int g_iWebServerConID;

WebSvrConnector::WebSvrConnector()
{

}

WebSvrConnector::~WebSvrConnector()
{

}

void WebSvrConnector::OnConnect( int connindex, int nErrCode )
{
	g_iWebServerConID = connindex;
	//MSG_S2C_SVR_READY_CMD readyPacket;
	//memcpy_s( readyPacket.szServerInfo, INFO_MAX_LEN, "TIOBE于近日如期公布了2013年7月TIOBE 编程语言指数排行榜。此次前三名依然是C 语言、Java和Objective-C，其中Objective-C依然呈现上升趋势。", INFO_MAX_LEN );
	//readyPacket.dwEncKey = 6543;
	//printf("Sending Client Message:[CL_AUTH][CL_AUTH_S2C_READY] ...");
	//::Send( connindex, (char*)&readyPacket, sizeof(MSG_S2C_SVR_READY_CMD) );
}

void WebSvrConnector::OnSend( int connindex, int send_count )
{

}

void WebSvrConnector::OnClose( int connindex, const char* szErrDesc )
{
	volatile int i = 6;
}

void WebSvrConnector::OnReadComplete( int connindex, CNetMsgPtr msgptr, int length )
{
	MSG_BASE* pMsgBase = (MSG_BASE*)msgptr->GetBuf();
	if( pMsgBase->m_byCategory != WS_DATA )
	{
		return;
	}
	g_CSPackHander.ParsePacket_WS( connindex, (MSG_BASE *)msgptr->GetBuf(), length );
}
