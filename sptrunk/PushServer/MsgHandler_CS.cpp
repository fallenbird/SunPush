#include "CommonHeader.h"
#include "MsgHandler.h"
#include "MsgStruct_CS.h"
#include "MsgStruct_WS.h"
#include "AdvanceLib/Socket/Socket.h"

extern int g_iWebServerConID;

// --处理消息函数
void PacketHandler::Handler_C2S_USER_AUTH_SYN( USER_INFOMATION * pUserInfo, MSG_BASE * pMsg, WORD wSize )
{
	printf( " 从Client收到消息:[CS_LOGON][C2S_USER_AUTH_SYN] \n");
	if( wSize != sizeof(MSG_C2S_USER_AUTH_SYN) )
	{
		return pUserInfo->KickUser( "XXX" );
	}
	MSG_C2S_USER_AUTH_SYN * pRecvMsg = (MSG_C2S_USER_AUTH_SYN*)pMsg;

	// --解密
	// 	if(!Crypt::PacketDecode( (unsigned char *)pRecvMsg->szPasswd,PASSWD_MAX_LEN,(unsigned char *)&pRecvMsg->szPasswd, 0 ))
	// 	{
	// 		WRITE_LOGFILE(LKICKOUT_LOG_NAME,e_Error, "[登陆] 帐号:%s(0), 描述:[登陆失败， Crypt::PacketDecode出现致命错误！]", pRecvMsg->szID);
	// 		return pPlayerInfo->KickUser(ERRSTR_LC_MSGDATA_ERROR);
	// 	}

	strncpy_s( pUserInfo->m_szAccountName, ID_MAX_LEN, pRecvMsg->szUserID, sizeof( pUserInfo->m_szAccountName ) );


	MSG_S2C_USER_AUTH_ACK ackMsg;
	ackMsg.byResult = 1;
	pUserInfo->SendToClient( &ackMsg, sizeof(ackMsg) );

	// --向数据库申请认证
	//MSG_UD_ACCOUNTINFO_REQ_SYN msg;
	//msg.m_dwKey = pPlayerInfo->m_nConnectID;
	//SAFE_STR_CPY( msg.szID , pRecvMsg->szID ); 
	//SAFE_STR_CPY( msg.szPasswd  ,pRecvMsg->szPasswd );

	//if(!ServerInfoManager::GetInstance()->SendToDBProxy((char*)&msg, sizeof(msg)))
	//{
	//	DISPMSG_WARNNING("Handler", "Client Login Failure, DBProxy Don't Connected！");
	//	WRITE_LOGFILE(LKICKOUT_LOG_NAME,e_Error, "[登陆] 帐号:%s(0), 描述:[登陆失败， DBProxyServer 未连接！]", pRecvMsg->szID);
	//	return pPlayerInfo->KickUser(ERRSTR_LC_DBPROXY_CONNECT_ERROR);
	//}

	//DISPMSG_DEBUG("Send", "DBProxy Message:[UD_ACCOUNTINFO][UD_ACCOUNTINFO_REQ_SYN] Sended!");
	//PLAYER_STATUS oldStatus = pPlayerInfo->ChangeStatus(PS_LOGIN_WAITMSG_DL_ACCOUNTINFO_REQ_ACK);
}



void PacketHandler::Handler_C2S_TEST_ORDER_SYN( USER_INFOMATION * pUserInfo, MSG_BASE * pMsg, WORD wSize )
{
	printf( " 从Client收到消息:[CS_LOGON][MSG_C2S_TEST_ORDER_SYN] \n");
	if( wSize != sizeof(MSG_C2S_TEST_ORDER_SYN) )
	{
		return pUserInfo->KickUser( "XXX" );
	}
	MSG_C2S_TEST_ORDER_SYN * pRecvMsg = (MSG_C2S_TEST_ORDER_SYN*)pMsg;
	//strncpy_s( pUserInfo->m_szAccountName, ID_MAX_LEN, pRecvMsg->szUserID, sizeof( pUserInfo->m_szAccountName ) );

	MSG_S2W_TEST_ORDER_SYN wsMsg;
	wsMsg.byOrderType = pRecvMsg->byOrderType;
	wsMsg.iUserGID = pUserInfo->m_nConnectID;
	wsMsg.iOrderNo = pRecvMsg->iOrderNo;
	wsMsg.iUserPosX = pRecvMsg->iUserPosX;
	wsMsg.iUserPosY = pRecvMsg->iUserPosY;
	memcpy_s( wsMsg.strPhone, MAX_PHONENO_LEN, pRecvMsg->strPhone, MAX_PHONENO_LEN );
	::Send( g_iWebServerConID, (char*)&wsMsg, sizeof(MSG_S2W_TEST_ORDER_SYN) );
}