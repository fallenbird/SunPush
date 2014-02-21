#ifndef _NET_THREAD_H_
#define _NET_THREAD_H_

class ClientAcceptor;
class WebSvrConnector;
class DBConnector;

class NetThread
{
// --不允许实例化
private:
	NetThread();
	~NetThread();

public:
	static void StartNetThread( int iThreadCnt );
	static void StopNetThread();
	static unsigned int _stdcall NetThreadPorc( void* pPara );


private:
	static ClientAcceptor*		m_pClientAcceptor;
	static WebSvrConnector*		m_pWebSvrConnector;
	static DBConnector*			m_pDBConnector;

};


#endif