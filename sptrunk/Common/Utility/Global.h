#ifndef __GLOBAL_H__
#define __GLOBAL_H__

//=============================================================================================================================
// Global header files
//	- 傍侩栏肺 荤侩 且 荐 乐绰 全局 函数甸狼 葛烙
//=============================================================================================================================
#pragma once
#include <string>
#include <vector>
#include <typeinfo.h>
#include "UtilityCommon.h"
#include <AdvanceLib/GlobalDefine.h>

using namespace std;
struct WzVector;

#define BIT_CHECK(x,b)		(((x) & (b)) ? true:false)
#define BIT_REMOVE(x,b)		((x) = ((x) & ~(b)))
#define BIT_ADD(x,b)		((x) = ((x) | (b)))	
#define RANGE_CHECK(x,a,b)		(((x>=a ) & (x<=b )) ? true:false)

enum TimeVariables
{
	TIME_SECOND = 1,
	TIME_MINUTE = TIME_SECOND * 60,
	TIME_HOUR   = TIME_MINUTE * 60,
	TIME_DAY	= TIME_HOUR * 24,
	TIME_MONTH	= TIME_DAY * 30,
	TIME_YEAR	= TIME_MONTH * 12,
};

enum MsTimeVariables
{
	MSTIME_SECOND	= 1000,
	MSTIME_MINUTE	= MSTIME_SECOND * 60,
	MSTIME_HOUR		= MSTIME_MINUTE * 60,
	MSTIME_DAY		= MSTIME_HOUR * 24,
};

// 是否相隔一天(秒1, 秒2)
inline bool	Overnight(time_t t1, time_t t2)
{
	return (0 != t1 / TIME_DAY - t2 / TIME_DAY);
}

/***********不支持多线程调用************/
// 2048字节大的字符数组
char * buffer2048();
std::string ToString(const WzVector& vec);


// 完全替换,不会对替换后的字符串再做替换处理
string & ReplaceAllDistinct(string & str, const string & old_value, const string & new_value);

template< typename Vector > inline void RandomRangeVector( Vector * pVec, INT iRangeMax, INT iRangeMin= 0 )
{
	++iRangeMax;
	pVec->x = pVec->x + (random(0,1)==0?-1.0f:1.0f)*(float)dRandom( iRangeMin, iRangeMax );
	pVec->y = pVec->y + (random(0,1)==1?-1.0f:1.0f)*(float)dRandom( iRangeMin, iRangeMax );
}

template<class Type>
inline Type _abs( Type value )
{
	return ( (value) < 0 ? -(value) : (value) );
}

template<class Type>
inline const Type & _max( Type & x, Type & y )
{
	return ( (x) < (y) ? (y) : (x) );
}

template<class Type>
inline const Type & _min( Type & x, Type & y )
{
	return ( (x) > (y) ? (y) : (x) );
}

template<class Type>
inline void _swap( Type & s1, Type & s2 )
{
	s1 ^= s2 ^= s1 ^= s2;
}
template<class Type>
inline Type _round( Type x )
{
	return ((x>0) ? floor(x+0.5f) : ceil(x-0.5f));
}

// 分离字符串到vector
template<typename T> size_t SplitString(vector<T> & vec, const char * szSrc, const char * delim)
{
	if (NULL == szSrc || stricmp(szSrc, "") == 0 || NULL == delim || stricmp(delim, "") == 0)
		return 0;

	string str(szSrc);
	char * tokens	= const_cast<char *>(str.c_str());
	char * param	= strtok(tokens, delim);

	if (NULL == param) return 0;

	typedef T (*cast_fun)(const char*);
	cast_fun	cf = NULL;

	if (typeid(T) == typeid(char)	|| typeid(T) == typeid(unsigned char)	||
		typeid(T) == typeid(short)	|| typeid(T) == typeid(unsigned short)	||
		typeid(T) == typeid(int)	|| typeid(T) == typeid(unsigned int))
	{
		cf	=	(cast_fun)&atoi;
	}
	else if (typeid(T) == typeid(double) || typeid(T) == typeid(float))
	{
		cf = (cast_fun)&atof;
	}
	else if(typeid(T) == typeid(long) || typeid(T) == typeid(unsigned long))
	{
		cf = (cast_fun)&atol;
	}
	else if(typeid(T) == typeid(long long) || typeid(T) == typeid(unsigned long long))
	{
		cf = (cast_fun)&_atoi64;
	}

	while(NULL != param)
	{
		if (NULL == cf)
			vec.push_back(T(param));
		else
			vec.push_back(cf(param));
		param	=	strtok(NULL, delim);
	}

	return vec.size();
}

#endif // __GLOBAL_H__
