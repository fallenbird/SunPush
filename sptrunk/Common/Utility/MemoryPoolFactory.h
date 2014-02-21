#ifndef __MEMORYPOOLFACTORY_H__
#define __MEMORYPOOLFACTORY_H__


#include "UtilityCommon.h"
#include "MemBand.h"
#include <stdio.h>

namespace util
{

template <class Type>
class CMemoryPoolFactory
{
public:
	typedef Type value_type;
public:
	CMemoryPoolFactory()
		:	m_pBandHead( NULL ),
			m_pFreeBand( NULL ),
			m_dwPoolExtendSize( 0 )
	{
	}
	virtual ~CMemoryPoolFactory()
	{
		Release();
	}
	BOOL Initialize( DWORD dwPoolBasicSize, DWORD dwPoolExtendSize = 0 )
	{
		if( 0 == dwPoolExtendSize )
			m_dwPoolExtendSize = dwPoolBasicSize/2+1;
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
		CHECKNULL_PTR( m_pFreeBand ) ;//  20090903 ÕâÀïµ¼ÖÂµã¾ÅÉÏµÄAgent·þÎñÆ÷¹ÒÁË
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
		ASSERT( !IsBadReadPtr( pn, sizeof(Type) ) );
		ASSERT( !IsBadWritePtr( pn, sizeof(Type) ) );

		return pn;
	}
	inline void Free(Type * pNode)
	{
		ASSERT( !IsBadReadPtr( pNode, sizeof(Type) ) );
		ASSERT( !IsBadWritePtr( pNode, sizeof(Type) ) );

		if(!m_pFreeBand->FreeObject(pNode))
		{
			if( !m_pFreeBand->pNext )
			{
				printf( "¹ÝÈ¯À» ¸¹ÀÌ Çß´Ù.\n" );
				return;
			}

			m_pFreeBand = m_pFreeBand->pNext;


			if(!m_pFreeBand->FreeObject(pNode))	//< ¿©±â¼­ »¶ ³¯°æ¿ì ( m_pFreeBand == NULL ), °³¼ö ÀÌ»óÀÇ pNode¸¦ ¹ÝÈ¯ÇÏ¿© »ý±è
			{
			}
		}
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
		while( pHead )
		{
			++dwTotalNum;
			pHead = pHead->pNext;
		}
		return dwTotalNum;
	}

	inline DWORD GetAvailableNumberOfTypes()
	{
		DWORD dwTotalNum = 0;
		CMemTypeBand<Type> * pHead = m_pBandHead;
		while(pHead)
		{
			dwTotalNum += pHead->GetAvailableNumberOfObjects();
			pHead = pHead->pNext;
		}
		return dwTotalNum;
	}
protected:

	CMemTypeBand<Type> *	m_pBandHead;
	CMemTypeBand<Type> *	m_pFreeBand;
	DWORD					m_dwPoolExtendSize;
};



} /// namespace util



#endif //__MEMORYPOOLFACTORY_H__