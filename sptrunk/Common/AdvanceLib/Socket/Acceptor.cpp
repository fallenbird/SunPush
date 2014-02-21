#include "../CommonHeader.h"
#include "Acceptor.h"
#include "Session.h"

extern boost::asio::io_service			g_io_service;
extern bool		g_bStopSocketFlag;

Acceptor::Acceptor(UINT16 port):acceptor_(g_io_service,boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port))
{
	m_nListenPort = port;
}

Acceptor::~Acceptor()
{
//	StopListen();
}

BOOL Acceptor::StartListen()
{
	if(g_bStopSocketFlag)return FALSE;

	Session* pClientSession = SessionManager::GetInstance()->AllocSession();
	if(pClientSession==NULL)return FALSE;

	acceptor_.async_accept(pClientSession->socket_,
		pClientSession->strand_.wrap(boost::bind(&Acceptor::handle_accept, this, pClientSession,
		boost::asio::placeholders::error)));

	InterlockedIncrement((long*)&Socket::s_nTotalListening);
#ifdef DEBUGINFO_CONNECT
	theConsole.Debug(_T("Acceptor"), _T("Already Created Listen Port, Port=[%d]"), m_nListenPort);
#endif

	return TRUE;
}

void Acceptor::StopListen()
{
	boost::system::error_code error;
// 	acceptor_.cancel(error);
// 	if(error)
// 	{
// 		theConsole.Error("Acceptor", "StopListen Failure-%s", boost::system::system_error(error).what());
// 		return;
// 	}
	acceptor_.close(error);
	if(error)
	{
		theConsole.Error(_T("Acceptor"), _T("StopListen Failure-%s"), boost::system::system_error(error).what());
		return;
	}

#ifdef DEBUGINFO_CONNECT
	theConsole.Debug(_T("Acceptor"), _T("Listen Port Already Closed, Port=[%d]"), m_nListenPort);
#endif

	InterlockedDecrement((long*)&Socket::s_nTotalListening);
}

void Acceptor::handle_accept(Session* pClientSession,const boost::system::error_code& error)
{

	if(error)
	{
		theConsole.Warning(_T("Acceptor"), _T("handle_accept-%s"), boost::system::system_error(error).what());
		//if(g_bStopSocketFlag)return;
		SessionManager::GetInstance()->DestroySession(pClientSession);
	}
	else
	{
		InterlockedIncrement((long*)&Socket::s_nTotalConneced);

		boost::system::error_code remote_error;
		pClientSession->socket_.remote_endpoint(remote_error);
		if(remote_error)
		{
			theConsole.Error(_T("Acceptor"), _T("handle_accept-[Remote Host SocketID Exception!]"));
			pClientSession->handle_exit(_T("Remote Host SocketID Exception!"));
		}
		else
		{

			pClientSession->m_emSocketStatus = SS_CONNECTED;
			pClientSession->m_pAcceptor = this;
			SessionManager::GetInstance()->UpdateSession(pClientSession->m_connindex, pClientSession);
			pClientSession->GetRemoteIP();			//»º´æ
			pClientSession->GetRemotePort();		//»º´æ
			this->OnAccept(pClientSession->m_connindex);

#ifdef DEBUGINFO_CONNECT
			theConsole.Debug(_T("Acceptor"), _T("New Connect[Total=%d], IP=[%s], Port=[%d]"), Socket::s_nTotalConneced, 
				pClientSession->GetRemoteIP(), pClientSession->GetRemotePort());
#endif

			pClientSession->start();
		}

		Session* pAcceptSession = SessionManager::GetInstance()->AllocSession();
		if(pAcceptSession==NULL)return;

		acceptor_.async_accept(pAcceptSession->socket_,
			pAcceptSession->strand_.wrap(boost::bind(&Acceptor::handle_accept, this, pAcceptSession,
			boost::asio::placeholders::error)));
	}


}