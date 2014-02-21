#include "CommonHeader.h"
#include "UserManager.h"
#include "ServerConfig.h"
#include "MsgStruct_CS.h"
#include <AdvanceLib/Socket/Socket.h>

UINT32  g_nPlayerColSize;	//取得内存池当前大小
UINT32	g_nCurPlayerCount;	//当前在线玩家数量
UINT32	g_nMaxPlayerCount;	//峰值在线玩家数量

USER_INFOMATION::USER_INFOMATION()
{
	m_nConnectID		=	-1;								// --LoginServer 或者AgentServer 的连接ID
	m_nAccountID		=	0;								// --帐号ID
	m_eUserStatus		=	eus_UNKNOW;						// --当前状态

	memset(m_szIPAddr, 0, sizeof(m_szIPAddr));				// --玩家IP地址
	memset(m_szAccountName, 0, sizeof(m_szAccountName));	// --帐号名称
	memset(m_szPassWord, 0, sizeof(m_szPassWord));

	SYSTEMTIME st;
	GetLocalTime(&st);
	sprintf_s( m_szLoginTime, 24, "%4d-%02d-%02d %02d:%02d:%02d", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
}


USER_INFOMATION::~USER_INFOMATION()
{
}
// 
// BOOL PLAYER_INFOMATION::Send( const char* data, short length )
// {
// 	return ::Send(m_nConnectID, data, length);
// }
// 
// BOOL PLAYER_INFOMATION::Send( MSG_BASE* data, short length )
// {
// 	return ::Send(m_nConnectID, (char*)data, length);
// }

USER_STATUS USER_INFOMATION::ChangeStatus( USER_STATUS newStatus )
{
	USER_STATUS oldStatus = m_eUserStatus;
	m_eUserStatus = newStatus;
	return oldStatus;
}

void USER_INFOMATION::KickUser( const char* lpszErrorDesc, short nErrorCode /*=-1*/)
{
	ASSERT(lpszErrorDesc != NULL);
	//WRITE_LOGFILE("KickUser", e_Error, "[踢出用户] 帐号:%s(%d), IP:%s, 描述:[%s]", m_szAccountName, m_nAccountID, m_szIPAddr, lpszErrorDesc);
	printf( "KickUser[踢出用户] 帐号:%s(%d), IP:%s, 描述:[%s]", m_szAccountName, m_nAccountID, m_szIPAddr, lpszErrorDesc );
	ChangeStatus( eus_ERROR );

	switch ( ServerConfig::s_emServerType )
	{
	case PUSH_SERVER:
		{
			MSG_S2C_KICK_USER_CMD nmsg;
			//memcpy_s( nmsg.szErrorDesc, 64, lpszErrorDesc, 64 );
			SendToClient( &nmsg, sizeof(MSG_S2C_KICK_USER_CMD) );
			//theConsole.Warning( "KickUser", "Player:[%s(%d), Desc:%s]!", m_szAccountName, m_nAccountID, lpszErrorDesc);
		}
		break;
	case AGENT_SERVER:
		{
			//MSG_CA_CONNECTION_KICKUSER msg;
			//msg.nErrorCode = nErrorCode;
			//memcpy_s( msg.szErrorDesc, 64, lpszErrorDesc, sizeof(msg.szErrorDesc) );
			//SendToClient( (char*)&msg, sizeof(MSG_CA_CONNECTION_KICKUSER) );
			//theConsole.Warning("KickUser", "Player:[%s(%d), Desc:%s]!", m_szAccountName, m_nAccountID, lpszErrorDesc);
		}
		break;

	case DATA_SERVER:
		{

		}
		break;


	default:
		{}//theConsole.Error("KickUser", "This Server Don't Support KickUser Message!");
	}
	AsyncClose(m_nConnectID);
}





BOOL USER_INFOMATION::SendToDBProxy( MSG_BASE_FORWARD2* pSendMsg, short length )
{
	//pSendMsg->m_dwKey = m_nAccountID;//m_nConnectID;
	//pSendMsg->m_dwAgentServerConnectID = m_nAgentServerConnectID;
	//return ServerInfoManager::GetInstance()->SendToDBProxy((char*)pSendMsg, length);
	return TRUE;
}


void USER_INFOMATION::SendSysMessage( const char* lpszFormat , ...)
{
	//char lpszpMsg[1024]={'\0'};
	//va_list ap;
	//va_start(ap, lpszFormat);
	//_vsnprintf(lpszpMsg, _countof(lpszpMsg),lpszFormat,ap);
	//SendMessageToClient(eMESSAGE_DISP_TYPE_CHATSYS,MSG_CW_CHAT_SYSTEM_CMD::_COLOR_GREEN,lpszpMsg);
	//va_end(ap);	
}


BOOL USER_INFOMATION::SendToClient( MSG_BASE* pMsg, short length )
{
	return ::Send(m_nConnectID, (const char*)pMsg, length);
}
