#pragma once
#include "SolarAssert.h"
#include "../AdvanceLib/Debug/sym_engine.h"

#include <iostream>

template<class T>
inline bool _lognotptr(T e, const TCHAR* szFileName, UINT iLine,const TCHAR *szFmt,...)
{
	MustNotBePtr( e );
	if (!(e))
	{
		static TCHAR szLog[512]= _T("");
		va_list args; 
		va_start(args, szFmt); 
		_vstprintf_s( szLog, _countof(szLog), szFmt , args ); 
		va_end(args); 
		//log_assert(szLog ,szFileName , iLine );
		_solarAssertBox( szLog , iLine, szFileName ,_T(__DATE__), _T(__TIME__)); 
		return false;
	}
	return true;
}


template<class T>
inline bool _logptr(T e, const TCHAR* szFileName, UINT iLine,const TCHAR *szFmt,...)
{
	//sym_engine::stack_trace(std::cout, 2, "%f(%l) : %m at %s\n");

	MustBePtr( e );
	if (!(e))
	{
		static TCHAR szLog[5120]=_T("");
		va_list args; 
		va_start(args, szFmt); 
		_vstprintf_s( szLog, _countof(szLog), szFmt , args ); 
		va_end(args); 
		//log_assert(szLog ,szFileName , iLine );
		_solarAssertBox( szLog , iLine, szFileName ,_T(__DATE__), _T(__TIME__)); 
		return false;
	}
	return true;
}

#ifdef _UNICODE
#define __FILET__		__FILEW__
#else
#define __FILET__		__FILE__
#endif

#ifndef ASSERT
#define ASSERT(e)									_lognotptr(e,__FILET__,__LINE__,_T(#e));
#endif

#define ASSERT_MSG(e,STR)							_lognotptr(e,__FILET__,__LINE__,STR);
#define ASSERT_MSG1(e,STR,parm1)					_lognotptr(e,__FILET__,__LINE__,STR,parm1);
#define ASSERT_MSG2(e,STR,parm1,parm2)				_lognotptr(e,__FILET__,__LINE__,STR,parm1,parm2);

#define CHECK( e )									if(!_lognotptr(e,__FILET__,__LINE__,_T(#e))) return;
#define CHECK_MSG(e,STR)							if(!_lognotptr(e,__FILET__,__LINE__,STR)) return;
#define CHECK_MSG1(e,STR,parm1)						if(!_lognotptr(e,__FILET__,__LINE__,STR,parm1)) return;
#define CHECK_MSG2(e,STR,parm1,parm2)				if(!_lognotptr(e,__FILET__,__LINE__,STR,parm1,parm2)) return;
#define CHECK_PTR( e )								if(!_logptr(e,__FILET__,__LINE__,_T(#e))) return;
#define CHECK_PTR_MSG( e , STR )					if(!_logptr(e,__FILET__,__LINE__,STR)) return;
#define CHECK_PTR_MSG1(e,STR,parm1)					if(!_logptr(e,__FILET__,__LINE__,STR,parm1)) return;
#define CHECK_PTR_MSG2(e,STR,parm1,parm2)			if(!_logptr(e,__FILET__,__LINE__,STR,parm1,parm2)) return;


#define CHECKF( e )									if(!_lognotptr(e,__FILET__,__LINE__,_T(#e))) return false;
#define CHECKF_MSG(e,STR)							if(!_lognotptr(e,__FILET__,__LINE__,STR)) return false;
#define CHECKF_MSG1(e,STR,parm1)					if(!_lognotptr(e,__FILET__,__LINE__,STR,parm1)) return false;
#define CHECKF_MSG2(e,STR,parm1,parm2)				if(!_lognotptr(e,__FILET__,__LINE__,STR,parm1,parm2)) return false;
#define CHECKF_PTR( e )								if(!_logptr(e,__FILET__,__LINE__,_T(#e))) return false;
#define CHECKF_PTR_MSG( e , STR )					if(!_logptr(e,__FILET__,__LINE__,STR)) return false;
#define CHECKF_PTR_MSG1(e,STR,parm1)				if(!_logptr(e,__FILET__,__LINE__,STR,parm1)) return false;
#define CHECKF_PTR_MSG2(e,STR,parm1,parm2)			if(!_logptr(e,__FILET__,__LINE__,STR,parm1,parm2)) return false;


#define CHECKNULL( e )								if(!_lognotptr(e,__FILET__,__LINE__,_T(#e))) return NULL;
#define CHECKNULL_MSG(e,STR)						if(!_lognotptr(e,__FILET__,__LINE__,STR)) return NULL;
#define CHECKNULL_MSG1(e,STR,parm1)					if(!_lognotptr(e,__FILET__,__LINE__,STR,parm1)) return NULL;
#define CHECKNULL_MSG2(e,STR,parm1,parm2)			if(!_lognotptr(e,__FILET__,__LINE__,STR,parm1,parm2)) return NULL;
#define CHECKNULL_PTR( e )							if(!_logptr(e,__FILET__,__LINE__,#e)) return NULL;
#define CHECKNULL_PTR_MSG( e , STR )				if(!_logptr(e,__FILET__,__LINE__,STR)) return NULL;
#define CHECKNULL_PTR_MSG1(e,STR,parm1)				if(!_logptr(e,__FILET__,__LINE__,STR,parm1)) return NULL;
#define CHECKNULL_PTR_MSG2(e,STR,parm1,parm2)		if(!_logptr(e,__FILET__,__LINE__,STR,parm1,parm2)) return NULL;


#define CHECKHANDLE( e )							if(!_lognotptr(e,__FILET__,__LINE__,#e)) return INVALID_HANDLE_VALUE;
#define CHECKHANDLE_MSG(e,STR)						if(!_lognotptr(e,__FILET__,__LINE__,STR)) return INVALID_HANDLE_VALUE;
#define CHECKHANDLE_MSG1(e,STR,parm1)				if(!_lognotptr(e,__FILET__,__LINE__,STR,parm1)) return INVALID_HANDLE_VALUE;
#define CHECKHANDLE_MSG2(e,STR,parm1,parm2)			if(!_lognotptr(e,__FILET__,__LINE__,STR,parm1,parm2)) return INVALID_HANDLE_VALUE;
#define CHECKHANDLE_PTR( e )						if(!_logptr(e,__FILET__,__LINE__,#e)) return INVALID_HANDLE_VALUE;
#define CHECKHANDLE_PTR_MSG( e , STR )				if(!_logptr(e,__FILET__,__LINE__,STR)) return INVALID_HANDLE_VALUE;
#define CHECKHANDLE_PTR_MSG1(e,STR,parm1)			if(!_logptr(e,__FILET__,__LINE__,STR,parm1)) return INVALID_HANDLE_VALUE;
#define CHECKHANDLE_PTR_MSG2(e,STR,parm1,parm2)		if(!_logptr(e,__FILET__,__LINE__,STR,parm1,parm2)) return INVALID_HANDLE_VALUE;


#define CHECKZERO( e )								if(!_lognotptr(e,__FILET__,__LINE__,#e)) return 0;
#define CHECKZERO_MSG(e,STR)						if(!_lognotptr(e,__FILET__,__LINE__,STR)) return 0;
#define CHECKZERO_MSG1(e,STR,parm1)					if(!_lognotptr(e,__FILET__,__LINE__,STR,parm1)) return 0;
#define CHECKZERO_MSG2(e,STR,parm1,parm2)			if(!_lognotptr(e,__FILET__,__LINE__,STR,parm1,parm2)) return 0;
#define CHECKZERO_PTR( e )							if(!_logptr(e,__FILET__,__LINE__,#e)) return 0;
#define CHECKZERO_PTR_MSG( e , STR )				if(!_logptr(e,__FILET__,__LINE__,STR)) return 0;
#define CHECKZERO_PTR_MSG1(e,STR,parm1)				if(!_logptr(e,__FILET__,__LINE__,STR,parm1)) return 0;
#define CHECKZERO_PTR_MSG2(e,STR,parm1,parm2)		if(!_logptr(e,__FILET__,__LINE__,STR,parm1,parm2)) return 0;


#define IF_OK( e )					if(_lognotptr(e,__FILET__,__LINE__,#e))
#define IF_OK_PTR( e )				if(_logptr(e,__FILET__,__LINE__,#e))
#define IF_NOT( e )					if(!_lognotptr(e,__FILET__,__LINE__,#e))
#define IF_NOT_PTR( e )				if(!_logptr(e,__FILET__,__LINE__,#e))
#define CHECK_RC( e , ret )			if(!_lognotptr(e,__FILET__,__LINE__,#e)) return ret;
#define CHECKRC_PTR( e , ret )		if(!_logptr(e,__FILE__,__LINE__,#e)) return ret;
#define SHOW_ASSERT_STR( STR )		_solarAssertBox( STR , __LINE__, __FILE__ ,__DATE__, __TIME__); 
#define WRITE_LOG_STR(STR)			log_assert(STR ,__FILE__ , __LINE__ );


#define DEF_INVALID_ASSIGN_OP( CLASS ) void operator=(CLASS &){SHOW_ASSERT_STR( "ERR" );}
#define DEF_INVALID_COPY_CTOR( CLASS ) CLASS(const CLASS &){SHOW_ASSERT_STR( "ERR" );}
#define DEF_INVALID_ASSIGN_COPY_CTOR( CLASS ) DEF_INVALID_ASSIGN_OP(CLASS) DEF_INVALID_COPY_CTOR( CLASS )


// 编译约束，在编译器报错，模拟编译期ASSERT [2009-7-22 14:42:19 YuHaiyun]

#define STATIC_ASSERT( e ) \
{\
	static int arr[(e) ? 1 : -1 ] = {0};\
};

#define GLOBAL_STATIC_ASSERT( e )	void GlobalStaticAssert(int (&arr)[(e) ? 1 : -1 ]) ;



template<class TYPE>
inline void MUST_BE_POD( const TYPE &refArg )
{
	typedef union tagU_TEMP_POD
	{
		int nTempPod;
		TYPE objPod ;
	}U_TEMP_POD;
	MustNotBePtr( refArg );
}

template<class TYPE,int ARR_LEN>
inline void MUST_BE_POD_ARR( TYPE (&refArg)[ARR_LEN] )
{
	MUST_BE_POD(refArg[0]);
	MustNotBePtr( refArg[0] );
}

template<class TYPE,int ARR_LEN>
inline void MUST_BE_PTR_ARR( TYPE (&refArg)[ARR_LEN] )
{
	MustBePtr( refArg[0] );
}

#define GLOBAL_MUST_BE_POD( TYPE ) \
{\
	typedef union tagU_TEMP_POD\
{\
	int nTempPod;\
	TYPE objPod ;\
}U_TEMP_POD;\
}

template<class TYPE>
inline void ZeroPod( TYPE &refOut )
{
	MUST_BE_POD( refOut ) ;
	ZeroMemory( &refOut , sizeof(refOut) ) ;
}

template<class TYPE,int ARR_LEN>
inline void ZeroPodArr( TYPE (&refOut)[ARR_LEN] )
{
	MUST_BE_POD_ARR( refOut ) ;
	ZeroMemory( refOut , sizeof(refOut) ) ;
}

template<class TYPE,int ARR_LEN>
inline void ZeroPtrArr( TYPE (&refOut)[ARR_LEN] )
{
	MUST_BE_PTR_ARR( refOut ) ;
	ZeroMemory( refOut , _sizeof(refOut) ) ;
}

template<class TYPE>	inline void MustNotBePtr( TYPE* )	{	STATIC_ASSERT( false );}// 禁止初始化指针，编译报错 [2009-12-10 12:43:10 YuHaiyun]
template<class TYPE>	inline void MustNotBePtr( TYPE )	{}
template<class TYPE>	inline void MustBePtr( TYPE* )	{}
template<class TYPE>	inline void MustBePtr( bool )	{STATIC_ASSERT( false );}
template<class TYPE>	inline void MustBePtr( TYPE& )	{STATIC_ASSERT( false );}// 禁止初始化指针，编译报错 [2009-12-10 12:43:10 YuHaiyun]



// template<class T>
// bool IsSigned( const T &refParam=0)
// {
// 	refParam;
// 	return (T)(~0) < 0;
//}

template<class T>
void MustBeSigned()
{
	STATIC_ASSERT( (T)(~0) < 0 );
}

template<class T>
void MustBeSigned( T &)
{
	//STATIC_ASSERT( (T)(~0) < 0 );
	MustBeSigned<T>();
}

template<class T>
void MustBeUnsigned()
{
	STATIC_ASSERT( (T)(~0) > 0 );
}

template<class T>
void MustBeUnsigned(T &)
{
	MustBeUnsigned<T>();
}

template<class T1,class T2>
struct IsSameType{};

template<class T1>
struct IsSameType<T1,T1>{ enum{m_eIsSame=false};};

template<class T1,class T2>
void MustBeSameType()
{
	IsSameType<T1,T2>::m_eIsSame;
}

template<class T1,class T2>
void MustBeSameType( T2 & /*ref*/)
{
	MustBeSameType<T1,T2>();
}


// 安全字符串拷贝函数
#include <tchar.h>
template<int ARR_LEN>
inline void SAFE_STR_CPY( TCHAR (&dst)[ARR_LEN] , const TCHAR* src)
{
	ASSERT( src && dst );
	if( src&&dst )
	{
		lstrcpyn( (dst) , (src) , _countof(dst) ) ; 
	}
}



//  删除容器中的指针[9/18/2009 GaoShengjie]
template<class T>
inline void SafeDeletePtr(T pPointer)
{
	SAFE_DELETE(pPointer);
}

#include <algorithm>
template <class T>
inline void SafeDeletePtrContainer(T&refContainer)
{
	std::for_each(refContainer.begin(),refContainer.end(),SafeDeletePtr<T::value_type>);
	refContainer.clear();

}
template <class T,int ARR_LEN>
void CopyPodArr ( T (&refOut)[ARR_LEN] , const T (&refIn)[ARR_LEN])
{
	MUST_BE_POD_ARR(refOut);
	MUST_BE_POD_ARR(refIn);
	STATIC_ASSERT( 0 < ARR_LEN );
	CHECK( refOut != refIn );// 禁止自己复制给自己 YuHaiyun [2010-5-24 10:05:36]
	CHECK( abs(refOut-refIn)>= _sizeof(refOut) );// 防止复制区域之间出现重叠的部分,此行把64位指针强转为INT类型，必须禁用编译选项“检查64位的可移植性” YuHaiyun [2010-5-24 10:11:22]
	memcpy(&refOut,&refIn,_sizeof(refOut) );
}

template<class T>
inline T GetMaxValue()
{
	STATIC_ASSERT( false );
}

template<>inline WORD	GetMaxValue(){return USHRT_MAX;}
template<>inline SHORT	GetMaxValue(){return SHRT_MAX;}
template<>inline INT	GetMaxValue(){return INT_MAX;}
template<>inline BYTE	GetMaxValue(){return UCHAR_MAX;}
template<>inline CHAR	GetMaxValue(){return CHAR_MAX;}
template<>inline DWORD	GetMaxValue(){return ULONG_MAX;}
template<>inline float	GetMaxValue(){return FLT_MAX;}

template<class T>
struct MaxValue
{
	static const T MAX_VALUE;
};
template<>
const WORD MaxValue<WORD>::MAX_VALUE =USHRT_MAX;


template<class T_TO,class T_FROM>inline T_TO& safe_cast_ref( T_FROM& /*refFrom */ ){	STATIC_ASSERT( FALSE );}
template<class T_TO,class T_FROM>inline T_TO* safe_cast_ptr( T_FROM* /*refFrom */ ){	STATIC_ASSERT( FALSE );}
