

#ifndef __PROTOCOL_WS_H__
#define __PROTOCOL_WS_H__

#include "GlobalDefine.h"
#include "MsgDefine.h"

#pragma once
#pragma pack(push,1)



// --WebServer --> PushServer
class MSG_W2S_WS_CONNECTED_CMD : public MSG_BASE
{
public:
	MSG_W2S_WS_CONNECTED_CMD()
	{
		m_byCategory = WS_DATA;
		m_byProtocol = W2S_WSCONNECTED_CMD;
	}
};


// --PushServer --> WebServer：认证请求
class MSG_S2W_PUSHSVR_AUTH_SYN : public MSG_BASE
{
public:
	MSG_S2W_PUSHSVR_AUTH_SYN()
	{
		m_byCategory = WS_DATA;
		m_byProtocol = S2W_PUSHSVRAUTH_SYN;
	}
	char szAuthKey[MAX_AUTHSTR_LEN];
};


// --WebServer --> PushServer：认证回复
class MSG_W2S_AUTH_RESULT_ACK : public MSG_BASE
{
public:
	MSG_W2S_AUTH_RESULT_ACK()
	{
		m_byCategory = WS_DATA;
		m_byProtocol = W2S_AUTH_RESULT_ACK;		
	}
	BYTE		byResult;
};


// --WebServer --> PushServer：认证回复
class MSG_W2S_NEWORDERBRD_CMD : public MSG_BASE
{
public:
	MSG_W2S_NEWORDERBRD_CMD()
	{
		m_byCategory = WS_DATA;
		m_byProtocol = W2S_NEWORDERBRD_CMD;		
	}
	int			m_iUsrConID;
	int			m_iOrder;
	char		m_strPhone[MAX_PHONENO_LEN];
	UINT		m_arrDrivers[32];
};


// --WebServer --> PushServer：认证回复
class MSG_S2W_TEST_ORDER_SYN : public MSG_BASE
{
public:
	MSG_S2W_TEST_ORDER_SYN()
	{
		m_byCategory = WS_DATA;
		m_byProtocol = S2W_TEST_ORDER_SYN;		
	}
	BYTE			byOrderType;
	int				iUserGID;
	int				iOrderNo;
	int				iUserPosX;
	int				iUserPosY;
	char			strPhone[MAX_PHONENO_LEN];
	char			strTest[168];
};




#pragma pack(pop)

#endif

