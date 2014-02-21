
#ifndef __PACKETSTRUCT_CL_H__
#define __PACKETSTRUCT_CL_H__

#include "GlobalDefine.h"
#include "MsgDefine.h"



#pragma pack(push,1)

// --PushServer --> Client
class MSG_S2C_SVR_READY_CMD : public MSG_BASE
{
public:
	MSG_S2C_SVR_READY_CMD()
	{
		m_byCategory = CS_LOGON;
		m_byProtocol = S2C_SVR_READY_CMD;
	}
	SHORT		sHighVer;					// --高版本号
	SHORT		sLowVer;					// --低版本号
    int			iEncKey;					// --密钥
	char		szAuthKey[MAX_AUTHSTR_LEN];	// --认证串
};


// --Client --> PushServer：认证请求
class MSG_C2S_USER_AUTH_SYN : public MSG_BASE
{
public:
	MSG_C2S_USER_AUTH_SYN()
	{
		m_byCategory = CS_LOGON;
		m_byProtocol = C2S_USER_AUTH_SYN;
	}
	char		szUserID[ID_MAX_LEN];		// --帐号
	char		szPasswd[PASSWD_MAX_LEN];	// --密码
};


// --PushServer --> Client：认证回复
class MSG_S2C_USER_AUTH_ACK : public MSG_BASE
{
public:
	MSG_S2C_USER_AUTH_ACK()
	{
		m_byCategory = CS_LOGON;
		m_byProtocol = S2C_USER_AUTH_ACK;		
	}
	BYTE		byResult;
};


// --PushServer --> Client：踢出通知
class MSG_S2C_KICK_USER_CMD : public MSG_BASE
{
public:
	MSG_S2C_KICK_USER_CMD()
	{
		m_byCategory = CS_LOGON;
		m_byProtocol = S2C_KICK_USER_CMD;
	}
	UINT16				nErrorCode;
};


// --Client-->PushServer ：测试下单
class MSG_C2S_TEST_ORDER_SYN : public MSG_BASE
{
public:
	MSG_C2S_TEST_ORDER_SYN()
	{
		m_byCategory = CS_LOGON;
		m_byProtocol = C2S_TEST_ORDER_SYN;
	}
	BYTE			byOrderType;
	int				iUserPosX;
	int				iUserPosY;
	int				iOrderNo;
	char			strPhone[MAX_PHONENO_LEN];
	char			strTest[168];
};


class MSG_C2S_TEST_ORDER_ACK : public MSG_BASE
{
public:
	MSG_C2S_TEST_ORDER_ACK()
	{
		m_byCategory = CS_LOGON;
		m_byProtocol = C2S_TEST_ORDER_ACK;
	}
	BYTE			byOrderType;
};


#pragma pack(pop)

#endif // __PACKETSTRUCT_CL_H__

