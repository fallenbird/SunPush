
#pragma once
#include "Socket.h"

#define _MAX_MSGSIZE	8192-12		//m_dwAnyData+4, m_pInfo+4, m_unMsgSize+4

class CNetMsg : public boost::noncopyable
{
	friend class Session;
public:
	CNetMsg();
	virtual ~CNetMsg();
	
	//------------ static ----------------
	static HANDLE	s_hHeap;
	static UINT		s_uNumAllocsInHeap;

	static BOOL		IsValidPt(void* p);

#ifdef _USE_PACKET_MEMORY_POOL
	void* operator new(std::size_t _Count, void* _Ptr );
	void operator delete(void* _Ptr);
	void operator delete(void *, void *);
	void operator delete(void* _Ptr,const std::nothrow_t&);
#endif
	//------------ common ----------------
	void	Init(void);
	void	Reset(void)	{Init();}

	const	USHORT	GetType	(void)		{return m_unMsgType;}
	const	USHORT	GetSize	(void)		{return (USHORT)m_dwMsgSize;}

	char*	GetBuf(void)	{return m_bufMsg;}

	//------------ virtual ----------------
	virtual BOOL			Create		(char* pMsgBuf, DWORD dwSize);
	virtual void			Process		(DWORD pSocket);

public:
	static int		GetMaxSize	(void)	{return _MAX_MSGSIZE;}
	static USHORT	GetType		(char* pbufMsg, DWORD dwMsgSize);
	static USHORT	GetSize		(char* pbufMsg, DWORD dwMsgSize);

protected:	
	union {
		char	m_bufMsg[_MAX_MSGSIZE];
		struct {
			USHORT	m_unMsgSize;
			USHORT	m_unMsgType;
		};
	};

public:
	DWORD		m_dwMsgSize;	
	DWORD		m_dwSendCount;
};

typedef boost::shared_ptr<CNetMsg> CNetMsgPtr;
CNetMsgPtr GetPoolMessage();						//在内存池中分配一个CNetMsg对象
