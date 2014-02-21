#ifndef _WEBSVR_CONNECTOR_H_
#define _WEBSVR_CONNECTOR_H_

#include <AdvanceLib/Socket/connector.h>

class WebSvrConnector : public Connector
{
public:
	WebSvrConnector();
	~WebSvrConnector();


protected:
	virtual void	OnConnect(int connindex, int nErrCode);							//有新的连接连进来了
	virtual void	OnSend(int connindex, int send_count);							//已经发送出去数据了
	virtual void	OnReadComplete(int connindex, CNetMsgPtr msgptr, int length);	//成功读取了一条完整消息
	virtual void	OnClose(int connindex, const char* szErrDesc);					//一个连接关闭了

};


#endif