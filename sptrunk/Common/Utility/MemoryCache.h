#ifndef __MEMORY_CACHE_H__
#define __MEMORY_CACHE_H__

#pragma once

#include <assert.h>

// NOT yet
#define tDefaultSize	10


template <class DataType, int _size = tDefaultSize>
class CacheSegment
{
	CacheSegment():m_pp(NULL),m_pn(NULL),m_iAvailableNum(_size)
	{
		assert( _size != 0 );
		//int i = 0;
		//while( m_pDataCache[i] = new DataType && i++ < ( _size -1 ) ){}
	}
	~CacheSegment(){}

	DataType *	Alloc()
	{
		DataType * pData = NULL;
		if( --m_iAvailableNum >= 0)
		{
			pData = m_pDataCache[m_iAvailableNum] = new ;
			m_pDataCache[m_iAvailableNum] = NULL;
		}
		return 	pData;
	}
	BOOL Free( DataType * pData )
	{
		if( m_iAvailableNum == _size ) return FALSE;

		m_pDataCache[m_iAvailableNum++] = pData;
	}
	VOID operator << ( CacheSegment<DataType, _size> * pSeg )
	{
		assert( m_pn == NULL );
		m_pp = pSeg;
		pSeg >> m_pp;
	}
	VOID operator >> ( CacheSegment<DataType, _size> * pSeg )
	{
		assert( m_pp == NULL );
		m_pn = pSeg;
		pSeg << m_pp;
	}

private:
	int								m_iAvailableNum;
	DataType *						m_pDataCache[_size];
	CacheSegment<DataType, _size> *	m_pn;
	CacheSegment<DataType, _size> *	m_pp;
};



template <class DataType, int _size = tDefaultSize>
class MemoryCache
{
	friend class CacheSegment;
public:
	MemoryCache()
	{
		assert( _size != 0 );
		m_pSegment = new CacheSegment<DataType, _size>;
	}
	~MemoryCache();

	DataType * CreateData()
	{
		DataType * pData = m_pSegment->Alloc();
		if( !pData )
		{
			CacheSegment<DataType, _size> * m_pTmp = ( new CacheSegment<DataType, _size> ) << m_pSegment;
			m_pSegment	= m_pTmp;
			return CreateData();
		}
		return pData;
	}

	VOID DestroyData( DataType * pData )
	{
		if( !m_pSegment->Free( pData ) )
		{
			m_pSegment
			DestroyData( pData );
		}
	}
	
private:
	CacheSegment<DataType, _size> * m_pSegment;
};


#endif // __MEMORY_CACHE_H__
