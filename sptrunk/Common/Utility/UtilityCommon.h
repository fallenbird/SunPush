#ifndef	_UTILITY_COMMON_H_
#define	_UTILITY_COMMON_H_

#include <memory>
#include <assert.h>

#define DISPMSG						printf
#define XY_INLINE inline
#define DEBUG_CODE(c)				c

#define COMPILER_MICROSOFT 0
#define COMPILER_GNU	   1
#define COMPILER_BORLAND   2

#ifdef _MSC_VER
	#define COMPILER COMPILER_MICROSOFT
#elif defined( __BORLANDC__ )
	#define COMPILER COMPILER_BORLAND
#elif defined( __GNUC__ )
	#define COMPILER COMPILER_GNU
#else
	#pragma error "FATAL ERROR: Unknown compiler."
#endif




#define random(min, max)        ((rand()%(int)(((max) + 1)-(min)))+ (min)) 
#define _RAND_MAX 0x7fff
#define dRandom(min, max)		((((double)rand()/(((double)(_RAND_MAX)+(double)(1)))) * (max-min))+(min))


//#if COMPILER != COMPILER_GNU
	//typedef signed __int64	int64;
	//typedef signed __int32	int32;
	//typedef signed __int16	int16;
	//typedef signed __int8	int8;
	//typedef unsigned __int64 uint64;
	//typedef unsigned __int32 uint32;
	//typedef unsigned __int16 uint16;
	//typedef unsigned __int8	 uint8;
//#else
	//typedef int64_t			int64;
	//typedef int32_t			int32;
	//typedef int16_t			int16;
	//typedef int8_t			int8;
	//typedef uint64_t		uint64;
	//typedef uint32_t		uint32;
	//typedef uint16_t		uint16;
	//typedef uint8_t			uint8;
//#endif

typedef long long				int64;
typedef int						int32;
typedef short					int16;
typedef signed char 			int8;
typedef unsigned long long		uint64;
typedef unsigned int			uint32;
typedef unsigned short			uint16;
typedef unsigned char			uint8;
	


#ifndef _SXF_TYPE_DEFINE
#define _SXF_TYPE_DEFINE
	typedef void *LPVOID;
	typedef void *HANDLE;
	typedef int						BOOL;
	typedef unsigned char			BYTE;
	typedef float					FLOAT;
	typedef char					CHAR;
	typedef signed char				INT8;
	typedef unsigned char			UCHAR;
	typedef unsigned char			UINT8;
	typedef unsigned char			BYTE;
	typedef short					SHORT;
	typedef signed short			INT16;
	typedef unsigned short			USHORT;
	typedef unsigned short			UINT16;
	typedef unsigned short			WORD;
	typedef int						INT;
	typedef signed int				INT32;
	typedef unsigned int			UINT;
	typedef unsigned int			UINT32;
	typedef long					LONG;
	typedef unsigned long			ULONG;
	typedef unsigned long			DWORD;
	typedef int64					LONGLONG;
	typedef int64					LONG64;
	typedef signed long long		INT64;
	typedef unsigned long long		ULONGLONG;
	typedef unsigned long long		DWORDLONG;
	typedef unsigned long long		ULONG64;
	typedef unsigned long long		DWORD64;
	typedef unsigned long long 		UINT64;
	#define FALSE 0
	#define TRUE 1
	#define MAX_PATH 260
#endif 


#include <set>
#include <list>
#include <string>
#include <map>
#include <queue>
#include <sstream>
#include <algorithm>

using namespace std;


#endif