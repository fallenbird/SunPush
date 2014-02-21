#pragma once
#include <string>
#include <fstream>
#include <stdio.h>
#include <assert.h>
/*using namespace std;*/

#include <stdio.h>
#include <stdarg.h>
inline bool SafeSprintf( char *szDst , size_t dwDstBufLen , const char *szFmt,...)
{
	if( NULL == szDst || 0 == dwDstBufLen )
		return false ;

	va_list args; 
	va_start(args, szFmt); 
//#if _MSC_VER < 1500
	_vsnprintf( szDst, dwDstBufLen, szFmt , args ); 
// #else
// 	_vsnprintf_s( szDst, 512, dwDstBufLen, szFmt , args ); 
// #endif
	va_end(args); 

	szDst[dwDstBufLen-1] = '\0' ;//  [2009-7-17 15:54:25 YuHaiyun]
	return true ;
}

inline void log_assert(const char* szExp,const char* szFileName,long lLine)
{
	//if (NULL == szExp)
	//{
	//	return;
	//}

	//char szFileFullPath[MAX_PATH] = "" ;
	//GetModuleFileName(NULL, szFileFullPath, MAX_PATH);

	//std::string strFile("");
	//for (size_t i=strlen(szFileFullPath)-1 ; i>=0 ; i-- )
	//{
	//	assert( i+1 < MAX_PATH ) ;
	//	if ( szFileFullPath[i] == '\\' && i+1 < MAX_PATH )
	//	{
	//		strFile.append("Log/");
	//		strFile.append((szFileFullPath + i + 1));
	//		break;
	//	}
	//}

	//SYSTEMTIME timeLocal;
	//GetLocalTime( &timeLocal );

	//char szCurTime[MAX_PATH] = "" ;
	//SafeSprintf( szCurTime , MAX_PATH , "%d_%d_%d" ,
	//	timeLocal.wYear, timeLocal.wMonth , timeLocal.wDay) ;

	//strFile.append("_");
	//strFile.append(szCurTime);
	//strFile.append(".log");
	//std::ofstream fileOut(strFile.c_str(),std::ios::app);
	//if( ! fileOut.is_open() )
	//{
	//	assert( ! "日志文件打开失败" ) ;
	//	return ;
	//}

	//fileOut << "[" 
	//	<< timeLocal.wYear	<< "_"
	//	<< timeLocal.wMonth	<< "_"
	//	<< timeLocal.wDay	<< " "
	//	<< timeLocal.wHour	<< ":"
	//	<< timeLocal.wMinute<< ":"
	//	<< timeLocal.wSecond<< "] ["
	//	<< szFileName<< "] [第"<<lLine<<"行] ["<<szExp<<"]"
	//	<< std::endl ;

	//fileOut.close();
}