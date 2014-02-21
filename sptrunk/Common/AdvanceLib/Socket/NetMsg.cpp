// NetMsg.cpp: implementation of the CNetMsg class.
//
//////////////////////////////////////////////////////////////////////
#include "../CommonHeader.h"
#include "NetMsg.h"

#include "Socket.h"

extern boost::object_pool<CNetMsg>		g_poolMessage;

// static 
HANDLE	CNetMsg::s_hHeap	=NULL;
UINT	CNetMsg::s_uNumAllocsInHeap	=0;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CNetMsg::CNetMsg()
{
#ifdef _DEBUG
	memset(m_bufMsg, 0, sizeof(m_bufMsg));
#endif
	m_unMsgSize	=0;
	m_unMsgType	=-1;
	m_dwSendCount = 0;
}

//////////////////////////////////////////////////////////////////////
CNetMsg::~CNetMsg()
{

}

//////////////////////////////////////////////////////////////////////
BOOL CNetMsg::IsValidPt(void* p)
{
	if (!s_hHeap || !p)
		return false;

	if (::IsBadCodePtr((FARPROC)p))
		return false;

	return ::HeapValidate(s_hHeap, 0, p);
}

//////////////////////////////////////////////////////////////////////
void CNetMsg::Init()
{
	memset(m_bufMsg, 0, sizeof(m_bufMsg));

	m_unMsgSize	=0;
	m_unMsgType	=-1;
}

//////////////////////////////////////////////////////////////////////
BOOL CNetMsg::Create(char* pbufMsg, DWORD dwMsgSize)
{
	if(!pbufMsg)
		return false;

	if((unsigned short)dwMsgSize != CNetMsg::GetSize(pbufMsg, dwMsgSize))
		return false;

	if(-1 == CNetMsg::GetType(pbufMsg, dwMsgSize))
		return false;

	memcpy(this->m_bufMsg, pbufMsg, dwMsgSize);
	return true;
}

//////////////////////////////////////////////////////////////////////
// default process is broadcast this msg to all room mates
void CNetMsg::Process(DWORD pSocket)
{
}

unsigned short	CNetMsg::GetType(char* pbufMsg, DWORD dwMsgSize)
{
	// check it...
	if(!pbufMsg || (int)dwMsgSize > CNetMsg::GetMaxSize())
		return 0;

	if(dwMsgSize <= 2*sizeof(unsigned short))
		return 0;

	unsigned short* punPointer	=(unsigned short*)pbufMsg;
	return punPointer[1];
}

//////////////////////////////////////////////////////////////////////
unsigned short	CNetMsg::GetSize(char* pbufMsg, DWORD dwMsgSize)
{
	// check it...
	if(!pbufMsg || (int)dwMsgSize > CNetMsg::GetMaxSize())
		return 0;

	if(dwMsgSize <= 2*sizeof(unsigned short))
		return 0;

	return	*((unsigned short*)pbufMsg) + PACKET_HEADER_LENGTH;
}

#ifdef _USE_PACKET_MEMORY_POOL
void* CNetMsg::operator new(std::size_t _Count, void* _Ptr ) throw( )
{
	return _Ptr;
}

void CNetMsg::operator delete(void *, void *) throw( )
{
	__asm int 3;
}

void CNetMsg::operator delete(void* _Ptr,const std::nothrow_t&) throw( )
{
	__asm int 3;
}


boost::mutex g_poolMutex;
void CNetMsg::operator delete(void* _Ptr) throw( )
{
	InterlockedDecrement((long*)&Socket::s_nNetMsgObjCount);
	boost::mutex::scoped_lock lock(g_poolMutex);
	g_poolMessage.destroy((CNetMsg*)_Ptr);
}
#endif


//在内存池中分配一个CNetMsg对象
CNetMsgPtr GetPoolMessage()
{
#ifdef _USE_PACKET_MEMORY_POOL
	InterlockedIncrement((long*)&Socket::s_nNetMsgObjCount);
	boost::mutex::scoped_lock lock(g_poolMutex);
	CNetMsgPtr ptr(g_poolMessage.construct());
	if(ptr.get()==NULL)
	{
		theConsole.Error("Packet","No PoolMem");
		__asm INT 3;
	}
#else
	CNetMsgPtr ptr(new CNetMsg);
	ptr->Init();

#endif
	return ptr;
}
