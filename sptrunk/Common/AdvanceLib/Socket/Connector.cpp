#include "../CommonHeader.h"
#include "Connector.h"

#include <boost/asio/local/stream_protocol.hpp>

extern boost::asio::io_service			g_io_service;
extern bool		g_bStopSocketFlag;

Connector::Connector()
{
	m_pConnectSession = NULL;
}

Connector::~Connector()
{

}

void Connector::AsyncConnect( const char* szIP, UINT16 port )
{
	if(g_bStopSocketFlag)return;

	if(m_pConnectSession!=NULL)
	{
//		theConsole.Error("Connect", "Repeat Connect Target Host:[%s:%d]", szIP, port);
//		return;
		//using namespace boost::asio;
		//namespace local = boost::asio::local;
		//boost::asio::local::stream_protocol sp;
		//m_pConnectSession->socket_.cancel();
		//m_pConnectSession->socket_.close();
		//CloseSocket(m_pConnectSession->m_connindex);
	}
	//else
	{
		m_pConnectSession = SessionManager::GetInstance()->AllocSession();
		if(m_pConnectSession==NULL)
		{
			theConsole.Error(_T("Connect"), _T("Alloc Session Failure, No Memory!!!"));
			return;
		}
		m_pConnectSession->m_pConnector = this;

	}

	InterlockedIncrement((long*)&Socket::s_nTotalConnecting);

	boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::tcp::v4(), port);
	boost::asio::ip::address_v4 v4addr;
	endpoint.address(boost::asio::ip::address(v4addr.from_string(szIP)));

	m_pConnectSession->m_emSocketStatus = SS_CONNECTING;
	m_pConnectSession->socket_.async_connect(endpoint,
		m_pConnectSession->strand_.wrap(boost::bind(&Connector::handle_connect, this, m_pConnectSession,
		boost::asio::placeholders::error)));

#ifdef DEBUGINFO_CONNECT
	theConsole.Debug(_T("Connect"), _T("Now Connecting Host:[%s:%d]"), szIP, port);
#endif

	//size_t cancelcnt = m_pConnectSession->timer_.expires_from_now(boost::posix_time::seconds(3));
	//m_pConnectSession->timer_.async_wait(
	//	m_pConnectSession->strand_.wrap(boost::bind(&Connector::handle_timeout,this, m_pConnectSession, boost::asio::placeholders::error))); 

}

void Connector::DisConnect()
{
	if(m_pConnectSession == NULL)return;
	CloseSocket(m_pConnectSession->m_connindex);
}

void Connector::handle_connect(Session* new_session,  const boost::system::error_code& error )
{		

	if(error)
	{
#ifdef DEBUGINFO_CONNECT
		theConsole.Warning(_T("Connect"), _T("handle_connect-%s"), boost::system::system_error(error).what());
#endif

		printf( "´íÎó:%s \n", boost::system::system_error(error).what() );
		if( g_bStopSocketFlag )
		{
			return;
		}
		InterlockedDecrement((long*)&Socket::s_nTotalConnecting);

		new_session->m_emSocketStatus = SS_ERROR;
		SessionManager::GetInstance()->DestroySession(new_session);
		m_pConnectSession = NULL;
	}
	else
	{
		//InterlockedIncrement((long*)&Socket::s_nTotalConneced);
#ifdef DEBUGINFO_CONNECT
		theConsole.Success(_T("Connect"), _T("Connect Dest Host[%s:%d] Success!"), new_session->GetRemoteIP(), new_session->GetRemotePort());
#endif
		new_session->m_emSocketStatus = SS_CONNECTED;
		SessionManager::GetInstance()->UpdateSession(new_session->m_connindex, new_session);
		new_session->start();
	}

	OnConnect(new_session->m_connindex, error.value());
}

void Connector::handle_timeout( Session* new_session, const boost::system::error_code& error )
{
	//if(new_session->m_emSocketStatus == SS_IDLE)
	//{
	//	OnConnect(new_session->m_connindex, error.value());
	//	m_pConnectSession->Close();
	//}
}