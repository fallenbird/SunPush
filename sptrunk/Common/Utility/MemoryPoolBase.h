// MemoryPoolBase.h: interface for the CMemoryPoolBase class.
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
// Be Done
// - MemoryPool template class of inherited-method and factory-method
// - Multi-thread safety
// - Buffer overflow write check
// To do
// - Analysis of amount used for band
//-------------------------------------------------------------------
// example)
//
//  class Money1 : public CMemoryPoolBase<Money1> { ... };
//
//  void main() 
//  {
//    Money1::Initialize(sizeof(Money1), 3, 2);
//
//    Money1 * pmoney1 = new Money1;
//    delete pmoney1;
//
//    Money1::Release();
//  }
//
//////////////////////////////////////////////////////////////////////
#ifndef __MEMORYPOOLBAES_H__
#define __MEMORYPOOLBAES_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MemBand.h"

namespace util
{


template<class Type>
class CMemoryPoolBase : public CMemBand<Type>
{
public:
	static void Initialize(DWORD dwObjSize, DWORD dwPoolBasicSize, DWORD dwPoolExtendSize)	
	{
		//#ifdef _PSDK_INSTALLED
		//	InitializeCriticalSectionAndSpinCount(&m_cs, 2000);
		//#else
			InitializeCriticalSection(&m_cs);
		//#endif 
		InitBand( dwObjSize, dwPoolBasicSize, dwPoolExtendSize );
	}
	static void Release()
	{
		//CMemBand::FreeBand();
		FreeBand();
		DeleteCriticalSection(&m_cs);
	}
	
	void * operator new( size_t size )
	{
		Lock();
		void * pn = (void *)AllocObject();
		Unlock();
		return pn;
	}
	void operator delete( void * p )
	{
		if(!p) return;
		Lock();
#ifdef _DEBUG
		BOOL rt = FreeObject((void *)p);
		TASSERT(rt);
#else
		FreeNode((void *)p);
#endif
		Unlock();
	}
protected:
	static void Lock() 		{ EnterCriticalSection(&m_cs); }
	static void Unlock() 	{ LeaveCriticalSection(&m_cs); }
	static CRITICAL_SECTION	m_cs;
};

template<class Type> CRITICAL_SECTION CMemoryPoolBase<Type>::m_cs;




} /// namespace util


#endif //__MEMORYPOOLBAES_H__