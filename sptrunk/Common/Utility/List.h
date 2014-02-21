// List.h: interface for the CList class.
// source code created by Min-Wook Kim (taiyo@webzen.co.kr)
// Lastupdated with memoryPool at 2004-7-29
// 
//-------------------------------------------------------------------
// History.
// 2004-07-28 : created with memoryPool
//-------------------------------------------------------------------
//////////////////////////////////////////////////////////////////////
#ifndef __LIST_H__
#define __LIST_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MemoryPoolFactory.h"

using namespace util;

typedef void* POS;


namespace util
{



template <class T>
class CList  
{
public:
	CList(DWORD dwBlockSize = 10) : m_pHead(0), m_pTail(0), m_nCount(0)
	{
		m_pNodePool = new CMemoryPoolFactory<NODE>;
		m_pNodePool->Initialize(dwBlockSize, dwBlockSize/2+1);
	}
	virtual ~CList()
	{
		DeleteAll();
		m_pNodePool->Release();
		delete m_pNodePool;
	}
	
protected:
    class NODE
    {
	public:
        T           pElement;
        NODE *		pPrev;
        NODE *		pNext;
        NODE() { pPrev=0; pElement=0; pNext=0; }
    };
    
    NODE *	m_pHead;
    NODE *	m_pTail;
    short	m_nCount;
	CMemoryPoolFactory<NODE>	* m_pNodePool;

public:
    // Get element of head
    inline T GetHead(){ return m_pHead->pElement; }
    
    // Add to linked list head
    void AddHead(T pElement)
    {
        NODE *newNode = m_pNodePool->Alloc();
        newNode->pPrev = 0;
        newNode->pElement = pElement;
        newNode->pNext = 0;
        
        if (0 == m_pHead)
        {
            m_pHead = newNode;
            m_pHead->pNext = 0;
            m_pTail = m_pHead;
        }
        else
		{
            m_pHead->pPrev = newNode;
            newNode->pNext = m_pHead;
            m_pHead = newNode;
        }
        ++m_nCount;
    }
    
    // Delete head
    T DeleteHead()
    {
		if (!m_pHead) return 0;

        T pElement = 0;
        if( m_pHead == m_pTail )
        {
            pElement = m_pTail->pElement;
            
            m_pNodePool->Free(m_pHead);
            m_pHead = m_pTail = 0;
            m_nCount = 0;
        }
        else 
		{
            NODE *pCurrent = m_pHead;
            m_pHead = m_pHead->pNext;
            m_pHead->pPrev = 0;
            
            pElement = pCurrent->pElement;
            
            m_pNodePool->Free(pCurrent);
            --m_nCount;
        }
        return pElement;
    }
    

    
    inline T GetTail(){ return m_pTail->pElement; }
    
    // Add to linked list tail
    void AddTail(T pElement)
    {
        NODE *newNode = m_pNodePool->Alloc();
        newNode->pPrev = 0;
        newNode->pElement = pElement;
        newNode->pNext = 0;
        
        if (0 == m_pTail)
        {
            m_pTail = newNode;
            m_pHead = m_pTail;
        }
        else
		{
            m_pTail->pNext = newNode;
            newNode->pPrev = m_pTail;
            m_pTail = newNode;
        }
        ++m_nCount;
    }
    
    // Delete tail
    T DeleteTail()
    {
		if (!m_pTail) return 0;

        T pElement = 0;
        if( m_pTail == m_pHead )
        {
            pElement = m_pTail->pElement;
            
            m_pNodePool->Free(m_pTail);
            m_pTail = m_pHead = 0;
            m_nCount = 0;
        }
        else 
		{
            NODE *pCurrent = 0;
            NODE *pAfter = m_pHead;
            while(1)
            {
                pCurrent = pAfter->pNext;
                if( pCurrent->pNext == 0 )
					break;
                pAfter = pCurrent;
            }
            pAfter->pNext = 0;
            m_pTail = pAfter;
            
            pElement = pCurrent->pElement;
            
            m_pNodePool->Free(pCurrent);
            --m_nCount;
        }
        return pElement;
    }
    

    
    // Insert element before index
    int InsertBefore(T pElement, int index)
    {
        if (index == 0) AddHead(pElement);
        else {
            NODE *pAfter = 0;
            NODE *pCurrent = m_pHead;
            while (0 < index-- && pCurrent) {
                pAfter = pCurrent;
                pCurrent = pCurrent->pNext;
            }
        
            NODE *newNode = m_pNodePool->Alloc();
            newNode->pPrev = pCurrent->pPrev;
            newNode->pElement = pElement;
            newNode->pNext = pCurrent;
        
            pCurrent->pPrev = newNode;
            pAfter->pNext = newNode;
        
            ++m_nCount;
        }
        return 0;
    }
    
    // Insert element after index
    int InsertpAfter(T pElement, int index)
    {
        if( index == m_nCount - 1 ) AddTail(pElement);
        else
		{
            NODE *pAfter = 0;
            NODE *pCurrent = m_pHead;
            while( 0 < index-- && pCurrent )
			{
                pAfter = pCurrent;
                pCurrent = pCurrent->pNext;
            }
            
            NODE *newNode = m_pNodePool->Alloc();
            newNode->pPrev = pCurrent;
            newNode->pElement = pElement;
            newNode->pNext = pCurrent->pNext;
            
            pAfter->pPrev = newNode;
            pCurrent->pNext = newNode;
            
            ++m_nCount;
        }
        return 0;
    }

  
    inline POS GetFirstPos() { return (POS)m_pHead; }
    inline POS GetLastPos() { return (POS)m_pTail; }
    
    // Pos to index
    int GetPosIndex(POS pos)
    {
        NODE *pCurrent = m_pHead;
        for( int i = 0 ; pCurrent ; ++i )
        {
            if( pos == (POS)pCurrent )
				return i;
            pCurrent = pCurrent->pNext;
        }
        return -1;
    }
    
    // Index to pos
    POS GetIndexPos(int index)
    {
        NODE *pCurrent = m_pHead;
        while( 0 < index-- && pCurrent )
            pCurrent = pCurrent->pNext;
        
        return (POS)pCurrent;
    }
	// taiyo test add
	T GetAtPos(POS& pos)
	{
		T pElement;
        if (pos)
        {
            pElement = ((NODE*)pos)->pElement;
        }
        else 
		{
            pElement = 0;
        }
        return pElement;
	}
    
    // Get previous position of node
    T GetPrevPos(POS& pos)
    {
        T pElement;
        if (pos)
        {
            pElement = ((NODE*)pos)->pElement;
            pos = (POS)((NODE*)pos)->pPrev;
        }
        else
		{
            pElement = 0;
            pos = 0;
        }
        return pElement;
    }
    
    // Get next position of node
    T GetNextPos(POS& pos)
    {
        T pElement;
        if (pos)
        {
            pElement = ((NODE*)pos)->pElement;
            pos = (POS)((NODE*)pos)->pNext;
        }
        else
		{
            pElement = 0;
            pos = 0;
        }
        return pElement;
    }
    
    // Delete position of node
    T DeletePos(POS& pos)
    {
        T pElement = 0;
        if (m_pHead == (NODE*)pos)
        {
            pElement = DeleteHead();
            pos = (POS)m_pHead;
        }
        else if (m_pTail == (NODE*)pos)
        {
            pElement = DeleteTail();
            pos = (POS)m_pTail;
        }
        else
		{
            NODE *pAfter = m_pHead;
            NODE *pCurrent = m_pHead->pNext;
            while (pCurrent != (NODE*)pos)
            {
                pAfter = pCurrent;
                pCurrent = pCurrent->pNext;
            }
            pCurrent->pNext->pPrev = pAfter;
            pAfter->pNext = pCurrent->pNext;
            pos = (POS)pAfter;
            
            pElement = pCurrent->pElement;
            
            m_pNodePool->Free(pCurrent);
            --m_nCount;
        }
        return pElement;
    }
    
    
    // Get total number of node
    inline int GetCount() { return m_nCount; }
    
    // Get node by index number : 0base
    T GetAt(int index)
    {
        NODE *pCurrent = m_pHead;
        while (0 < index-- && pCurrent)
            pCurrent = pCurrent->pNext;
        
        return pCurrent->pElement;
    }
    
    // Delete at
    T DeleteAt(int index)
    {
        if (index == 0) return DeleteHead();
        else if( index == m_nCount - 1 ) return DeleteTail();
        else
		{
            NODE *pAfter = m_pHead;
            NODE *pCurrent = m_pHead->pNext;
            while( index-- )
            {
                pAfter = pCurrent;
                pCurrent = pCurrent->pNext;
            }
            pCurrent->pNext->pPrev = pAfter;
            pAfter->pNext = pCurrent->pNext;
            
            T pElement = pCurrent->pElement;
            
            m_pNodePool->Free(pCurrent);
            --m_nCount;
            
            return pElement;
        }
        return 0;
    }
    
    // Delete all node
    void DeleteAll()
    {
        NODE *pCurrent = m_pHead;
        NODE *pAfter = 0;
        while( pCurrent )
        {
            pAfter = pCurrent->pNext;
            m_pNodePool->Free(pCurrent);
            pCurrent = pAfter;
        }
        m_pHead = m_pTail = 0;
        m_nCount = 0;
    }

};


} /// namespace util

#endif // __LIST_H__
