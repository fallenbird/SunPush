#ifndef _USER_MANAGER_H_
#define _USER_MANAGER_H_

#include <map>
#include <utility>

using namespace std;

#include "GlobalDefine.h"
#pragma pack(4)

enum USER_STATUS
{
	eus_ERROR		= -1,
	eus_UNKNOW		= 0,					// --客户端刚登陆， 时间很小
	eus_CONNECTED	= 1,					// --已经连接
	eus_LOGINED		= 2,					// --已经登陆
	eus_READY		= 3,					// --已经成功处理了令牌消息
};

/* 类名: PLAYER_INFOMATION
 * 说明: 通用服务器登陆用户的基本信息
 *       注意一但使用该结构的服务器需要有自己的对象属性, 则需要派生这个类来实现子属性
 */
struct MSG_BASE;
struct MSG_BASE_FORWARD2;
struct USER_INFOMATION
{
public:
	USER_INFOMATION();
	~USER_INFOMATION();

	USER_STATUS		ChangeStatus(USER_STATUS newStatus);
	void			KickUser(const char* lpszErrorDesc, short nErrorCode = -1);

	/************************************************************************/
	/*                              通用部分                                */
	/************************************************************************/
	long			m_nConnectID;						// --连接ID
	UINT32			m_nAccountID;						// --帐号ID
	USER_STATUS		m_eUserStatus;						// --当前状态
	char			m_szIPAddr[16];						// --用户IP地址
	char			m_szAccountName[32];				// --帐号名称
	char			m_szLoginTime[24];					// --用户登陆时间
	char			m_szPassWord[32];                   // --用户密码



	BOOL			SendToClient( MSG_BASE* pMsg, short length );

	BOOL			SendToDBProxy(MSG_BASE_FORWARD2* data, short length);
	void			SendSysMessage(const char* lpszFormat , ...);
};

/* 类名: UserInfoManager
 * 说明: 通用的用户信息管理器, 并包含了一些通用的方法
 */
template<class T> class UserInfoManager
{
public:
	~UserInfoManager();
	 static UserInfoManager* GetInstance();

	// --添加新用户， 注意这个函数需要调用二次， 第一次调用只加入一个连接ID KEY， 第二次再调用的时候会把用户帐号KEY与GUID KEY加入进去
	T*			AddUser(long nConnectID, UINT32 nAccountID=0, UINT64 nGUID=0);	

	// --移除一个用户的信息
	void		RemoveUserByAccountID(UINT32 nAccountID);
	void		RemoveUserByConnectID(long nConnectID);

	// --更新一个用户的信息
	BOOL		UpdateUserByConnectID(long nConnectID, const T* pPlayerInfo);

	// --查找用户的信息
	T*			FindUserByConnectID(long nConnectID);
	T*			FindUserByAccountID(UINT32 nAccountID);

	UINT32		GetAccountIDByUserName(const TCHAR* lpszUserName);
	UINT32		GetCurUserCount(){return g_nCurPlayerCount;}
	UINT32		GetMaxUserCount(){return g_nMaxPlayerCount;}
	UINT32		GetUserColSize(){return g_nPlayerColSize;}

	void		NoticeMessage(const char* pMsg, short nSize);

private:
	UserInfoManager(){}
	static UserInfoManager*		s_instance;
	std::map<UINT32, T*>		m_mapAIDToINFO;		// --帐号ID与用户信息
	std::map<long  , T*>		m_mapCIDToINFO;		// --连接ID与用户信息
	boost::object_pool<T>		m_poolPlayers;		// --内存池
	UINT32						m_nCurPlayerCount;	// --当前在线用户数量
	UINT32						m_nMaxPlayerCount;	// --峰值在线用户数量
	boost::mutex				m_mutex;
};

extern UINT32  g_nPlayerColSize;					// --取得内存池当前大小
extern UINT32  g_nCurPlayerCount;					// --当前在线用户数量
extern UINT32  g_nMaxPlayerCount;					// --峰值在线用户数量


//-----------------------------------------------------------------------/
/*                              模板实现                                */
//-----------------------------------------------------------------------/
template<class T> UserInfoManager<T>* UserInfoManager<T>::s_instance = NULL;
template<class T> UserInfoManager<T>* UserInfoManager<T>::GetInstance()
{
	if(s_instance == NULL)
	{
		s_instance = new UserInfoManager;
	}
	return s_instance;
}


// --加入用户
template<class T> 
T* UserInfoManager<T>::AddUser(long nConnectID, UINT32 nAccountID/*=0*/, UINT64 nGUID/*=0*/)
{
	boost::mutex::scoped_lock lock(m_mutex);
	std::map<long, T*>::iterator ite = m_mapCIDToINFO.find(nConnectID);
	T* pPlayerInfo = NULL;
	if(ite == m_mapCIDToINFO.end())
	{
		pPlayerInfo = static_cast<T*>(m_poolPlayers.construct());
		if(pPlayerInfo==NULL)return NULL;

		pPlayerInfo->m_nConnectID = nConnectID;
		m_mapCIDToINFO.insert(make_pair(nConnectID, pPlayerInfo));
	}
	else
	{
		pPlayerInfo = (T*)ite->second;
	}

	if(nAccountID != 0)
	{
		pPlayerInfo->m_nAccountID = nAccountID;
		std::pair<std::map<UINT32, T*>::iterator, bool> ret = m_mapAIDToINFO.insert(make_pair(nAccountID, pPlayerInfo));
		ASSERT(ret.second);
		if(!ret.second)return NULL;
	}

	g_nPlayerColSize  = (UINT32)m_poolPlayers.get_next_size();
	g_nCurPlayerCount = (UINT32)m_mapCIDToINFO.size();
	if(g_nCurPlayerCount > g_nMaxPlayerCount)
	{
		g_nMaxPlayerCount = g_nCurPlayerCount;
	}
	return pPlayerInfo;
}


// --通过连接ID移除用户
template<class T> 
void UserInfoManager<T>::RemoveUserByConnectID(long nConnectID)
{
	boost::mutex::scoped_lock lock(m_mutex);
	std::map<long, T*>::iterator ite = m_mapCIDToINFO.find(nConnectID);
	if(ite == m_mapCIDToINFO.end())return;

	T* pPlayerInfo = (T*)ite->second;
	UINT32 nAccountID = pPlayerInfo->m_nAccountID;

	m_mapCIDToINFO.erase(nConnectID);
	if(nAccountID!=0)m_mapAIDToINFO.erase(nAccountID);
}


// --通过帐号ID移除用户
template<class T> 
void UserInfoManager<T>::RemoveUserByAccountID( UINT32 nAccountID )
{
	boost::mutex::scoped_lock lock(m_mutex);
	std::map<UINT32, T*>::iterator ite = m_mapAIDToINFO.find(nAccountID);
	if(ite == m_mapAIDToINFO.end())return;
	T* pPlayerInfo = (T*)ite->second;
	UINT32 nConnectID = pPlayerInfo->m_nConnectID;
	lock.unlock();
	RemovePlayerByConnectID(nConnectID);
}


// --通过连接ID更新用户
template<class T> 
BOOL UserInfoManager<T>::UpdateUserByConnectID( long nConnectID, const T* pPlayerInfo )
{
	boost::mutex::scoped_lock lock(m_mutex);
	std::map<long, T*>::iterator ite = m_mapCIDToINFO.find(nConnectID);
	if(ite == m_mapCIDToINFO.end())return FALSE;
	T* pSrcPlayerInfo = ite->second;
	*pSrcPlayerInfo = *pPlayerInfo;
	return TRUE;
}


// --通过连接ID查找用户的信息
template<class T> 
T* UserInfoManager<T>::FindUserByConnectID(long nConnectID)
{
	boost::mutex::scoped_lock lock(m_mutex);
	std::map<long, T*>::iterator ite = m_mapCIDToINFO.find(nConnectID);
	if(ite == m_mapCIDToINFO.end())return NULL;
	return ite->second;
}


// --通过帐号ID查找用户的信息
template<class T> 
T* UserInfoManager<T>::FindUserByAccountID(UINT32 nAccountID)
{
	boost::mutex::scoped_lock lock(m_mutex);
	std::map<UINT32, T*>::iterator ite = m_mapAIDToINFO.find(nAccountID);
	if(ite == m_mapAIDToINFO.end())return NULL;
	return ite->second;
}


// --以下是通用用户相关方法定义
template<class T> 
void UserInfoManager<T>::NoticeMessage(const char* pMsg, short nSize)
{
	boost::mutex::scoped_lock lock(m_mutex);
	MSG_BASE_FORWARD2* pSendMsg = (MSG_BASE_FORWARD2*)pMsg;
	std::map<long  , T*>::iterator ite = m_mapCIDToINFO.begin();
	for(ite; ite!=m_mapCIDToINFO.end(); ++ite)
	{
		ite->second->SendToClient(pMsg, nSize);
		//int nSendConnectID = -1;
		//switch(theConfig.s_emServerType)
		//{
		//case LOGIN_SERVER:	
		//case AGENT_SERVER:		nSendConnectID = ite->first;																break;
		//case WORLD_SERVER:
		//case GAME_SERVER:		pSendMsg->m_dwKey = ite->first; nSendConnectID = ite->second->m_nAgentServerConnectID;		break;
		//case GAME_DBPROXY:		pSendMsg->m_dwKey = ite->first; nSendConnectID = ite->second->m_nGameServerConnectID;		break;
		//default:	ASSERT(!"服务器类型未定义！");
		//}
		//::Send(nSendConnectID, pMsg, nSize);
	}
}





#endif 