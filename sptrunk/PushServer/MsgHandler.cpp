#include "CommonHeader.h"
#include "MsgHandler.h"
#include "MsgDefine.h"


PacketHandler g_CSPackHander;

// --构造函数
PacketHandler::PacketHandler()
{
	m_pFuncMap_CS	= new util::FunctionMap;
	m_pFuncMap_WS	= new util::FunctionMap;
	RegisterHandler_CS();
	RegisterHandler_WS();
}


// --析构函数
PacketHandler::~PacketHandler()
{
	if( m_pFuncMap_CS )		
	{
		delete m_pFuncMap_CS;
	}
	if( m_pFuncMap_WS )
	{
		delete m_pFuncMap_WS;
	}
}


// --注册CS消息处理器
BOOL PacketHandler::RegisterHandler_CS()
{
	if( !AddHandler_CS( CS_LOGON, C2S_USER_AUTH_SYN, Handler_C2S_USER_AUTH_SYN ) )
	{
		return FALSE;
	}
	if( !AddHandler_CS( CS_LOGON, C2S_TEST_ORDER_SYN, Handler_C2S_TEST_ORDER_SYN ) )
	{
		return FALSE;
	}
	return TRUE;
}


// --注册WS消息处理器
BOOL PacketHandler::RegisterHandler_WS()
{
	if( !AddHandler_WS( WS_DATA, W2S_WSCONNECTED_CMD, Handler_W2S_WSCONNECTED_CMD ) )
	{
		return FALSE;
	}
	if( !AddHandler_WS( WS_DATA, W2S_AUTH_RESULT_ACK, Handler_W2S_AUTH_RESULT_ACK ) )
	{
		return FALSE;
	}
	if( !AddHandler_WS( WS_DATA, W2S_NEWORDERBRD_CMD, Handler_W2S_NEWORDERBRD_CMD ) )
	{
		return FALSE;
	}
	return TRUE;
}


// --向函数map添加函数
BOOL PacketHandler::AddHandler_CS( BYTE category, BYTE protocol, PacketHandler::fnHandler_CS fnHandler )
{
	FUNC_CS * pFuncInfo	= new FUNC_CS;
	pFuncInfo->m_dwFunctionKey	= MAKEWORD( category, protocol );
	pFuncInfo->m_fnHandler		= fnHandler;
	return m_pFuncMap_CS->Add( pFuncInfo );
}


// --向函数map添加函数
BOOL PacketHandler::AddHandler_WS( BYTE category, BYTE protocol, PacketHandler::fnHandler_WS fnHandler )
{
	FUNC_WS * pFuncInfo	= new FUNC_WS;
	pFuncInfo->m_dwFunctionKey	= MAKEWORD( category, protocol );
	pFuncInfo->m_fnHandler		= fnHandler;
	return m_pFuncMap_WS->Add( pFuncInfo );
}


// --解析CS消息函数
BOOL PacketHandler::ParsePacket_CS( USER_INFOMATION * pUserInfo, MSG_BASE * pMsg, WORD wSize )
{
	FUNC_CS * pFuncInfo = (FUNC_CS *)m_pFuncMap_CS->Find( MAKEWORD( pMsg->m_byCategory, pMsg->m_byProtocol ) );
	if( pFuncInfo == NULL )
	{
		/////WRITE_LOGFILE(LSYSTEM_LOG_NAME, e_Error, "[PacketHandler::ParsePacket_CU] [C:%d][P:%d]", pMsg->m_byCategory, pMsg->m_byProtocol );
		return FALSE;
	}
	pFuncInfo->m_fnHandler( pUserInfo, pMsg, wSize );
	return TRUE;
}


// --解析WS消息函数
BOOL PacketHandler::ParsePacket_WS( int nConnIndex, MSG_BASE *pMsg, WORD wSize )
{
	FUNC_WS * pFuncInfo = (FUNC_WS *)m_pFuncMap_WS->Find( MAKEWORD( pMsg->m_byCategory, pMsg->m_byProtocol ) );
	if( pFuncInfo == NULL )
	{
		return FALSE;
	}
	pFuncInfo->m_fnHandler( nConnIndex, pMsg, wSize );
	return TRUE;
}


