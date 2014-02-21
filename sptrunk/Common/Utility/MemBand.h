// MemBand.h: interface for the CMemBand Abstract class.
// Memory Pool Management Class version 1.0
// source code created by Min-Wook Kim (taiyo@webzen.co.kr)
// 2004-7-26
// 
//-------------------------------------------------------------------
// History.
// 2004-3-31	:	MemoryPoolTempl class ver 0.1
// 2004-6-18	:	MemoryPoolTempl class ver 0.2
// 2004-6-21    :	Error Check Byte added ver 0.21
// 2004-7-26    :	MemoryPool Framework ver1.0
// 2004-7-27    :	Bug Fixed! delete [] m_pQueue;
// Be Done
// - MemoryPool template class of inherited-method and factory-method
// - Multi-thread safety
// - Buffer overflow write check
// To do
// - Analysis of amount used for band
//-------------------------------------------------------------------
//
//////////////////////////////////////////////////////////////////////
#ifndef __MEMBAND_H__
#define __MEMBAND_H__


#ifdef _DEBUG
	#include <assert.h>
	#define TASSERT(a) assert(a)
	#define	DEBUG_SLICE_SIZE	4
	#define	DEBUG_SLICE_VALUE	'D'
#else
	#define	DEBUG_SLICE_SIZE	0
#endif 


namespace util
{



#if 0
template <class Type>
class CMemBand
{

	static CMemBand<Type> * 	m_pHeadBand;
	static CMemBand<Type> * 	m_pTailBand;
	static CMemBand<Type> * 	m_pCurBand;

	static DWORD m_dwObjSize;
	static DWORD m_dwPoolBasicSize;
	static DWORD m_dwPoolExtendSize;

	char * m_pBandBody;
	DWORD * m_pQueue;
	DWORD m_dwObjCount;
	DWORD m_dwMaxBandObjCount;
	CMemBand<Type> * m_pPrev;
	CMemBand<Type> * m_pNext;

protected:
	CMemBand()
		:	m_pPrev(NULL),
			m_pNext(NULL),
			m_pBandBody(0),
			m_pQueue(0),
			m_dwObjCount(0),
			m_dwMaxBandObjCount(0)
	{
	}

	virtual ~CMemBand()
	{
	}

	static void * AlloObject()
	{
		if( m_pCurBand->m_dwObjCount > 0 )
		{
			return (void *)*(m_pCurBand->m_pQueue + --m_pCurBand->m_dwObjCount);
		}
		else
		{
			AllocBand();
			m_pCurBand = m_pCurBand->m_pNext;
			return AlloObject();
		}
	}

	static bool FreeObject(void * pObj)
	{
		if( m_pCurBand->m_dwObjCount < m_pCurBand->m_dwMaxBandObjCount )
		{
	#ifdef _DEBUG
			for( int i = 0 ; i < DEBUG_SLICE_SIZE ; ++i )
			{
				if( ((char * )pObj)[m_dwObjSize - DEBUG_SLICE_SIZE + i] != DEBUG_SLICE_VALUE )
					return false;
			}
	#endif
			*(m_pCurBand->m_pQueue + m_pCurBand->m_dwObjCount) = (DWORD)pObj;
			m_pCurBand->m_dwObjCount++;
		}
		else
		{
			m_pCurBand = m_pCurBand->m_pPrev;
			return FreeObject(pObj);
		}

		return true;
	}

	static void InitBand(DWORD dwObjSize, DWORD dwPoolBasicSize, DWORD dwPoolExtendSize)
	{
		m_dwObjSize = dwObjSize + DEBUG_SLICE_SIZE;
		m_dwPoolBasicSize = dwPoolBasicSize;
		m_dwPoolExtendSize = dwPoolExtendSize;
		AllocBand();
	}

	static void AllocBand()
	{
		CMemBand<Type> * pBand = new CMemBand;
		DWORD dwPoolSize = 0;
		if( !m_pHeadBand )
		{
			dwPoolSize = m_dwPoolBasicSize;
			m_pCurBand = m_pHeadBand = m_pTailBand	= pBand;
		}
		else
		{
			dwPoolSize = m_dwPoolExtendSize;
			m_pTailBand->m_pNext		= pBand;
			pBand->m_pPrev				= m_pTailBand;
			m_pTailBand					= pBand;
			
		}

		pBand->m_pBandBody			= new char[m_dwObjSize*dwPoolSize];
	#ifdef _DEBUG
		DWORD j = 0;
		while( j < dwPoolSize )
		{
			memset( &pBand->m_pBandBody[ m_dwObjSize * ( j + 1 ) - DEBUG_SLICE_SIZE ], DEBUG_SLICE_VALUE, DEBUG_SLICE_SIZE );
			++j;
		}
	#endif
		pBand->m_pQueue				= new DWORD[dwPoolSize];
		DWORD i = 0;
		while( i < dwPoolSize )
		{
			*(pBand->m_pQueue + i) = (DWORD)(pBand->m_pBandBody + i * m_dwObjSize);
			++i;
		}
		
		pBand->m_dwObjCount			= dwPoolSize;
		pBand->m_dwMaxBandObjCount	= dwPoolSize;
	}

	static void FreeBand()
	{
		CMemBand<Type> * dp = m_pHeadBand;
		while(dp)
		{
			m_pHeadBand = m_pHeadBand->m_pNext;
			delete [] dp->m_pBandBody;
			delete [] dp->m_pQueue;
			delete dp;
			dp = m_pHeadBand;
		}
		m_pHeadBand = NULL;
	}

};

template <class Type> CMemBand<Type> * CMemBand<Type>::m_pHeadBand = NULL;
template <class Type> CMemBand<Type> * CMemBand<Type>::m_pTailBand = NULL;
template <class Type> CMemBand<Type> * CMemBand<Type>::m_pCurBand = NULL;

template <class Type> DWORD CMemBand<Type>::m_dwObjSize = 0;
template <class Type> DWORD CMemBand<Type>::m_dwPoolBasicSize = 0;
template <class Type> DWORD CMemBand<Type>::m_dwPoolExtendSize = 0;

#endif

template <typename DataType>
class CMemTypeBand
{
	DataType **		ppDataType;
	DataType *		pDeleteArray;
	DWORD			nMaxBandObjectNum;
	DWORD			FreeIdx;

public:
	CMemTypeBand *	pPrev;
	CMemTypeBand *	pNext;

	CMemTypeBand():pPrev(NULL),pNext(NULL){}

	virtual ~CMemTypeBand()
	{
		pPrev=NULL;
		pNext=NULL;
		delete [] ppDataType;
		delete [] pDeleteArray;
	}

	inline DWORD GetAvailableNumberOfObjects()
	{
		return FreeIdx;
	}

	inline DWORD GetMaxNumberOfObjects()
	{
		return nMaxBandObjectNum;
	}

	DataType * GetObject( DWORD dwIdx )
	{
		return &pDeleteArray[dwIdx];
	}

	static CMemTypeBand * AllocBand( CMemTypeBand<DataType> *& head, DWORD nMaxObject )
	{
		CMemTypeBand<DataType>* pp = new CMemTypeBand<DataType>;
		pp->ppDataType = new DataType*[nMaxObject];
		memset(pp->ppDataType, 0, sizeof(DataType*)*nMaxObject);
		pp->pDeleteArray = new DataType[nMaxObject];
		for(DWORD i = 0 ; i < nMaxObject ; i++)
		{
			pp->ppDataType[i] = &pp->pDeleteArray[i];
		}
		pp->nMaxBandObjectNum	= nMaxObject;
		pp->FreeIdx				= nMaxObject;
		pp->pNext				= head;
		if(head)
			head->pPrev			= pp;
		head = pp;

		return pp;
	}

	static void FreeBand( CMemTypeBand<DataType> * pDelBand )
	{
		//head node one call, all delete;
		CMemTypeBand<DataType> * db = pDelBand;
		while(db)
		{
			CMemTypeBand<DataType> * tmp = db;
			db = db->pNext;
			delete tmp;
		}
	}
	DataType * AlloObject()
	{
		//	TASSERT(FreeIdx-1 >= 0);
		if(--FreeIdx == -1)
		{
			FreeIdx = 0;
			return NULL;
		}
		else
			return ppDataType[FreeIdx];
	}

	bool FreeObject(DataType * data)
	{
		//	TASSERT(FreeIdx+1 < nMaxNum);
		if(FreeIdx  < nMaxBandObjectNum )
			ppDataType[FreeIdx++] = data;
		else
			return false;
		return true;
	}
};



} /// namespace util


#endif // __MEMBAND_H__
