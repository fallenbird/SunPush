#ifndef _CLIENT_ACCEPTOR_H_
#define _CLIENT_ACCEPTOR_H_

#include <AdvanceLib/Socket/Acceptor.h>


class ClientAcceptor : public Acceptor
{
public:
	ClientAcceptor(  UINT16 port );
	~ClientAcceptor();


protected:
	virtual void	OnAccept(int connindex);										// --有新的连接连进来了
	virtual void	OnSend(int connindex, int send_count);							// --已经发送出去数据了
	virtual void	OnReadComplete(int connindex, CNetMsgPtr msgptr, int length);	// --成功读取了一条完整消息
	virtual void	OnClose(int connindex, const char* szErrDesc);					// --一个连接关闭了

public:
	//static ClientAcceptor* GetInstance()
	//{
	//	static ClientAcceptor myClientAcceptor;
	//	return &myClientAcceptor;
	//

};


#endif