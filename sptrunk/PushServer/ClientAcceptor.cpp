#include "CommonHeader.h"
#include "ClientAcceptor.h"
#include "UserManager.h"
#include "MsgStruct_CS.h"
#include "MsgHandler.h"

ClientAcceptor::ClientAcceptor( UINT16 port ):Acceptor( port )
{
	
}

ClientAcceptor::~ClientAcceptor()
{

}


void ClientAcceptor::OnAccept( int connindex )
{
	const char* lpszIPAddr = SessionManager::GetInstance()->GetSession(connindex)->GetRemoteIP();

	//DISPMSG_NOTICE("Acceptor", "Client IP:[%s] Connected, ID:[%d]", lpszIPAddr, connindex);
	printf( "Client Acceptor Client IP:[%s] Connected, ID:[%d] \n", lpszIPAddr, connindex );
	//WRITE_LOGFILE(LCONNECT_LOG_NAME, e_Error, "[建立连接] 客户端IP=[%s] 与服务器建立连接 ConnID=[%d] 总连接数:[%d]", lpszIPAddr, connindex, Socket::s_nTotalConneced);
	USER_INFOMATION* pUserInfo = UserInfoManager<USER_INFOMATION>::GetInstance()->AddUser( connindex );
	if( NULL == pUserInfo )
	{
		return CloseSocket(connindex);
	}
	pUserInfo->ChangeStatus( eus_CONNECTED );
	strncpy_s( pUserInfo->m_szIPAddr, 16, lpszIPAddr, sizeof( pUserInfo->m_szIPAddr ) );

	// --给APP终端
	MSG_S2C_SVR_READY_CMD readyPacket;
	readyPacket.sHighVer = 7;
	readyPacket.sLowVer =  8;
	readyPacket.iEncKey =  666;
	memcpy_s( readyPacket.szAuthKey, MAX_AUTHSTR_LEN, TO_WS_AUTH_STR, MAX_AUTHSTR_LEN );
	printf("Sending Client Message:[CS_LOGON][MSG_S2C_SVR_READY_CMD] ...");
	::Send( connindex, (char*)&readyPacket, sizeof(MSG_S2C_SVR_READY_CMD) );
}


void ClientAcceptor::OnClose( int connindex, const char* szErrDesc )
{

}

void ClientAcceptor::OnSend( int connindex, int send_count )
{

}


void ClientAcceptor::OnReadComplete( int connindex, CNetMsgPtr msgptr, int length )
{
	MSG_BASE* pMsgBase = (MSG_BASE*)msgptr->GetBuf();
	if( pMsgBase->m_byCategory != CS_LOGON )
	{
		return;
	}

	USER_INFOMATION* pUserInfo = UserInfoManager<USER_INFOMATION>::GetInstance()->FindUserByConnectID( connindex );
	if( NULL == pUserInfo )
	{
		return CloseSocket( connindex );
	}
	g_CSPackHander.ParsePacket_CS( pUserInfo, (MSG_BASE *)msgptr->GetBuf(), length );
}


