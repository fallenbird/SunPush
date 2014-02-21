#pragma once

#include "Session.h"
#include "NetMsg.h"

class Acceptor
{
	friend class Session;
public:
	Acceptor(UINT16 port);
	virtual ~Acceptor();
	//开始监听端口
	BOOL	StartListen();
	void	StopListen();

	Acceptor(Acceptor &);
	Acceptor & operator = (Acceptor &);

protected:
	virtual void OnAccept(int /*connindex*/){}										//有新的连接连进来了
	virtual void OnSend(int /*connindex*/, int /*send_count*/){}						//已经发送出去数据了
	virtual void OnReadComplete(int /*connindex*/, CNetMsgPtr /*msgptr*/, int /*length*/){}	//成功读取了一条完整消息
	virtual void OnClose(int /*connindex*/, const TCHAR* /*szErrDesc*/){}				//一个连接关闭了

private:
	void handle_accept(Session* new_session,const boost::system::error_code& error);
	boost::asio::ip::tcp::acceptor acceptor_;

	UINT16	m_nListenPort;
};	
