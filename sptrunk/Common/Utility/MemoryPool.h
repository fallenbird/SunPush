#ifndef _MEMORYPOOL_H_
#define _MEMORYPOOL_H_

#include <vector>

namespace util
{


template <typename T>
class MemoryPool
{
public:
	MemoryPool( DWORD dwSize )
	{
		m_dwMaxSize = dwSize;

		vector<T*>::iterator it;

		for( DWORD i = 0; i < dwSize; ++i )
		{
			m_list.push_back( new T );
		}		

		InitializeCriticalSection( &m_cs );
	}

	~MemoryPool()
	{
		Clear();
		DeleteCriticalSection( &m_cs );
	}

	T* Alloc()
	{
		EnterCriticalSection( &m_cs );
		
		assert( !m_list.empty() );

		return m_list.pop_back();

		LeaveCriticalSection( &m_cs );
	}

	void Restore( T* pItem )
	{
		EnterCriticalSection( &m_cs );

		assert( m_list.size() < m_dwMaxSize );

		m_list.push_back( pItem );

		LeaveCriticalSection( &m_cs );
	}

private:
	void Clear()
	{
		vector<T*>::iterator it;
		for( it = m_list.begin(); it < m_list.end(); ++it )
		{
			delete (*it);
		}
		m_list.clear();
	}

private:
	CRITICAL_SECTION	m_cs;
	vector<T*>			m_list;
	DWORD				m_dwMaxSize;
};

};	//end of namespace util

#endif // _MEMORYPOOL_H_