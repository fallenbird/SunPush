#ifndef __SAFEMEMORYPOOLFACTORY_H__
#define __SAFEMEMORYPOOLFACTORY_H__
// SafeMemoryPoolFactory.h: interface for the CMemoryPoolFactory class.
// Memory Pool Management Class version 1.01
// source code created by Min-Wook Kim (taiyo@webzen.co.kr)
// 2004-1-10
// 
//-------------------------------------------------------------------
// History.
// 2004-3-31	:	MemoryPoolTempl class ver 0.1
// 2004-6-18	:	MemoryPoolTempl class ver 0.2
// 2004-6-21    :	Error Check Byte added ver 0.21
// 2004-7-26    :	MemoryPool Framework ver1.0
// 2005-1-10    :	SafeMemoryPoolFactory class added 1.01
// 2005-3-28    :	set default value as dwPoolExtendSize = dwPoolBasicSize/2+1
// Be Done
// - MemoryPool template class of inherited-method and factory-method
// - Multi-thread safety
// - Buffer overflow write check
// To do
// - Analysis of amount used for band
//-------------------------------------------------------------------
// example)
//
//	1. Thread-NonSafetyPool sample code
//  class Money1 { ... };
//
//  void main() 
//  {
//    CMemoryPoolFactory<Money1> * pool = new CMemoryPoolFactory<Money1>;
//    pool->Initialize( 10000, 510 );
//
//    Money1 * pmoney11 = pool->Alloc();
//    pool->Free(pmoney11);
//
//    pool->Release();
//  }
//
//	2. Thread-SafetyPool sample code
//  class Money1 { ... };
//
//  void main() 
//  {
//    SafeMemoryPoolFactory<Money1> * pool = new SafeMemoryPoolFactory<Money1>; //!!!!!!!! different thing !!!!!!!
//    pool->Initialize( 10000, 510 );
//
//    Money1 * pmoney11 = pool->Alloc();
//    pool->Free(pmoney11);
//
//    pool->Release();
//  }
//
// error situation)
// If (m_pFreeBand of m_pFreeBand->pNext) is NULL in Free() function,
// =>possibility
// 1. Free()을 Alloc()호출한 횟수이상을 호출했을 때 
// 2. 또는 Free(node)의 node가 뜩몸되게 Free되었을 때
//////////////////////////////////////////////////////////////////////

#pragma once

#include "MemBand.h"

namespace util
{


template <class Type>
class SafeMemoryPoolFactory
{
public:
	SafeMemoryPoolFactory()
		:	m_pBandHead( NULL ),
			m_pFreeBand( NULL ),
			m_dwPoolExtendSize( 0 ){}

	virtual ~SafeMemoryPoolFactory()
	{
		Release();
		DeleteCriticalSection( &m_cs );
	}
	BOOL Initialize( DWORD dwPoolBasicSize, DWORD dwPoolExtendSize = 0 )
	{
		
		InitializeCriticalSection( &m_cs );

		if( 0 == dwPoolExtendSize )
			m_dwPoolExtendSize = dwPoolExtendSize/2+1;
		else
			m_dwPoolExtendSize = dwPoolExtendSize;

		if( NULL == m_pBandHead )
		{
			CMemTypeBand<Type>::AllocBand( m_pBandHead, dwPoolBasicSize );
			m_pFreeBand = m_pBandHead;
			if( !m_pFreeBand ) 
				return FALSE;
			return TRUE;
		}
		return FALSE;
	}

	void Release()
	{
		if( m_pBandHead )
		{
			CMemTypeBand<Type>::FreeBand( m_pBandHead );
			m_pBandHead = NULL;
			m_pFreeBand = NULL;
		}
	}
	
	inline Type * Alloc()
	{
		lock();
		Type * pn = m_pFreeBand->AlloObject();
		if(pn == NULL)
		{
			if( !m_pFreeBand->pPrev )
			{
				CMemTypeBand<Type>::AllocBand( m_pBandHead, m_dwPoolExtendSize );
				m_pFreeBand = m_pBandHead;
				pn = m_pFreeBand->AlloObject();
			}
			else
			{
				m_pFreeBand = m_pFreeBand->pPrev;
				pn = m_pFreeBand->AlloObject();
			}
		}
		unlock();
		return pn;
	}
	inline void Free(Type * pNode)
	{
		lock();
		if(!m_pFreeBand->FreeObject(pNode))
		{
			m_pFreeBand = m_pFreeBand->pNext;
			if(!m_pFreeBand->FreeObject(pNode))
			{
				__asm int 3;
			}
		}
		unlock();
	}
	
	inline DWORD GetPoolBasicSize()
	{
		return m_pBandHead->GetMaxNumberOfObjects();
	}

	inline DWORD GetPoolExtendSize()
	{
		return m_dwPoolExtendSize;
	}

	inline DWORD GetNumberOfBands()
	{
		DWORD dwTotalNum = 0;
		CMemTypeBand<Type> *	pHead = m_pBandHead;
		while( pHead = pHead->pNext )
			++dwTotalNum;
		return dwTotalNum;
	}

	inline DWORD GetAvailableNumberOfTypes()
	{
		DWORD dwTotalNum = 0;
		CMemTypeBand<Type> * pHead = m_pBandHead;
		while( pHead = pHead->pNext )
			dwTotalNum += pHead->GetAvailableNumberOfObjects();
		return dwTotalNum;
	}
protected:

	inline void				lock() 		{ EnterCriticalSection(&m_cs); }
	inline void				unlock() 	{ LeaveCriticalSection(&m_cs); }

	CRITICAL_SECTION		m_cs;

	CMemTypeBand<Type> *	m_pBandHead;
	CMemTypeBand<Type> *	m_pFreeBand;
	DWORD					m_dwPoolExtendSize;
};



}

#endif // __SAFEMEMORYPOOLFACTORY_H__