// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once

#include <stdio.h>
#include <tchar.h>



// TODO: 在此处引用程序需要的其他头文件
typedef void *HANDLE;
typedef int                 BOOL;
typedef unsigned char       BYTE;
typedef float               FLOAT;
typedef char                CHAR;
typedef signed char         INT8;
typedef unsigned char       UCHAR;
typedef unsigned char       UINT8;
typedef unsigned char       BYTE;
typedef short               SHORT;
typedef signed short        INT16;
typedef unsigned short      USHORT;
typedef unsigned short      UINT16;
typedef unsigned short      WORD;
typedef int                 INT;
typedef signed int          INT32;
typedef unsigned int        UINT;
typedef unsigned int        UINT32;
typedef long                LONG;
typedef unsigned long       ULONG;
typedef unsigned long       DWORD;
typedef __int64             LONGLONG;
typedef __int64             LONG64;
typedef signed __int64      INT64;
typedef unsigned __int64    ULONGLONG;
typedef unsigned __int64    DWORDLONG;
typedef unsigned __int64    ULONG64;
typedef unsigned __int64    DWORD64;
typedef unsigned __int64    UINT64;



//////////////////////////////////////////////////////////////////////////
//								boost扩展标准库包含

#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/strand.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/thread.hpp>
#include <boost/pool/object_pool.hpp>
#include <boost/noncopyable.hpp>
#include <boost/smart_ptr.hpp>


#define ASSERT(exp) ((void) 0)



#ifdef _DEBUG
#pragma comment( lib, "Advancelib_vc10d.lib" )
#else
#pragma comment( lib, "Advancelib_vc10.lib" )
#endif





