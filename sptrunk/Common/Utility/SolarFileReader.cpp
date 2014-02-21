#include "UtilityCommon.h"
#include <assert.h>
#include ".\solarfilereader.h"
#include "Global.h"
#define	GETVALIDDATA	eTOKEN_TYPE getTokenType = TOKEN_ERROR;\
						char c = GetValidData();\
						if (TOKEN_END == c)\
						return TOKEN_END;\
						char * pbp = &m_pszTokenString[0];


namespace util
{

SolarFileReader::SolarFileReader(LanFindCallback funcCB/* = NULL*/)
	:	m_pszBuffer ( NULL ),
		m_dTokenNumber ( 0.0 ),
		m_dwBufferSize ( 0 ),
		m_dwBufferPos ( 0 ),
		m_funcCB(funcCB),
		m_Mode ( SEPERATOR_ERROR )
{
	memset(m_szFileName,0,_countof(m_szFileName));
}

SolarFileReader::~SolarFileReader()
{
	assert( m_pszBuffer == NULL );
}
void SolarFileReader::_clear()
{
	m_dTokenNumber = 0.0;
	m_pszTokenString[0] = NULL;
}
void SolarFileReader::CloseFile()
{
	CloseBuffer();
}
void SolarFileReader::CloseBuffer()
{
	if ( m_pszBuffer )
	{
		delete [] m_pszBuffer;
		m_pszBuffer = NULL;
	}
	m_dwBufferSize	= 0;
	m_dwBufferPos	= 0;
}
void SolarFileReader::Reset()
{
	m_dwBufferPos	= 0;
	_clear();
}
BOOL SolarFileReader::OpenBuffer( BYTE * pBuffer, DWORD dwBufferSize, eSEPERATOR_MODE mode )
{
	if( NULL == m_pszBuffer )
	{
		return FALSE;
	}
	m_dwBufferSize	= dwBufferSize;
	m_pszBuffer = new char [dwBufferSize];
	if( !m_pszBuffer )
	{
		return FALSE;
	}
	memcpy( m_pszBuffer, pBuffer, dwBufferSize );
	m_Mode = mode;
	m_dwBufferPos = 0;
	return TRUE;
}

BOOL SolarFileReader::OpenFile( const char * pszFileName, eSEPERATOR_MODE mode /*= SEPERATOR_WHITESPACE */ )
{
	//if( NULL == m_pszBuffer )
	//{
	//	return FALSE;
	//}
	//strcpy_s( m_szFileName , MAX_PATH, pszFileName ) ;
	//m_Mode = mode;

	//HANDLE hFile = ::CreateFile( pszFileName, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	//if ( INVALID_HANDLE_VALUE == hFile )
	//{
	//	return FALSE;
	//}

	//m_dwBufferSize = GetFileSize( hFile, NULL );
	//if ( m_dwBufferSize < 0 )
	//	return FALSE;
	//
	//m_pszBuffer		= new char [m_dwBufferSize];
	//m_dwBufferPos	= 0;

	//DWORD dwNumber	= 0;
	//BOOL rt = ::ReadFile( hFile, m_pszBuffer, m_dwBufferSize, &dwNumber, NULL );
	//if( !rt ) return FALSE;

	//::CloseHandle( hFile );

	return FALSE;
}

char SolarFileReader::_getchar( DWORD * pPos )
{
	if( pPos == NULL )
		pPos = &m_dwBufferPos;

	if( *pPos < m_dwBufferSize )
		return m_pszBuffer[(*pPos)++];
	else
	{
		++(*pPos);
		return TOKEN_END;
	}
}
char SolarFileReader::_getcurchar( DWORD * pPos )
{
	if( pPos == NULL )
		pPos = &m_dwBufferPos;

	return (char)( *pPos < m_dwBufferSize ? m_pszBuffer[*pPos] : TOKEN_END );
}

void SolarFileReader::_rewind( int i, DWORD * pPos )
{
	if( pPos == NULL )
	{
		pPos = &m_dwBufferPos;
	}
	assert( *pPos - i >= 0 );
	*pPos -= i;
}

int SolarFileReader::_isspace( char c )
{
	if( c == TOKEN_SPACE || c == TOKEN_NEWLINE || c == TOKEN_CARRIAGERETURN || c == TOKEN_TAB )
		return 1;
	return 0;
}

int SolarFileReader::_isnewline( char c )
{
	if( c == TOKEN_NEWLINE || c == TOKEN_CARRIAGERETURN )
		return 1;
	return 0;
}


void SolarFileReader::_parsercell(std::string & str)
{
	if (!m_funcCB) return;

	// need find a better way
	string::size_type pos = 0;

	while (pos != string::npos)
	{
		pos = str.find("$L:", pos);

		if (pos != string::npos)
		{
			string::size_type pos_str = pos + 3;
			string::size_type pos_end = str.find_first_of("$", pos_str);

			if (pos_end == string::npos)
				return;

			string replacestr = str.substr(pos, pos_end - pos + 1);
			string findstr = str.substr(pos + 3, pos_end - pos - 3);

			findstr = m_funcCB(findstr.c_str());
			ReplaceAllDistinct(str, replacestr, findstr);
			pos = 0;
		}
	}
}

char SolarFileReader::GetValidData()
{
	// 提取重复代码 [8/5/2009 13:04:38 CHANCY]
	char c = 0, nc = 0;
	BOOL bInfiniteLoop = TRUE;
	while( bInfiniteLoop )
	{
		c = _getchar();
		nc = _getcurchar();

		if( TOKEN_SLASH == c && TOKEN_SLASH == nc )
		{
			while( ( c = _getchar() ) != TOKEN_END && c != TOKEN_NEWLINE );
		}
		else if( TOKEN_SLASH == c && TOKEN_ASTERISK  == nc )
		{
			c = _getchar();// * 公矫困秦
			while ( ( c = _getchar() ) != TOKEN_END && !( c == TOKEN_ASTERISK && _getcurchar() == TOKEN_SLASH ));
			c = _getchar();// * 公矫困秦
			c = _getchar();// / 公矫困秦
		}

		// 文件 退出 眉农
		if( c == TOKEN_END ) 
			return TOKEN_END;

		if( 0 == _isspace( c ) )
			break;
	}

	return c;

}

eTOKEN_TYPE	SolarFileReader::_getNextTokenType_whitespace( eTOKEN_TYPE desireTokenType )
{
	desireTokenType;
	GETVALIDDATA
	switch( c )
	{
	case TOKEN_END: break;
		// 箭磊
	case '0':	case '1':	case '2':	case '3':	case '4':
	case '5':	case '6':	case '7':	case '8':	case '9':
	case '.':	case '-':	case '+':
		{
			*(pbp++) = c;
			while( ( c = _getchar() ) != TOKEN_END && ( isdigit( c ) != 0 || c == '.' || c == 'f') )
			{
				*(pbp++) = c;
			}
			*pbp = NULL;
			_rewind(1);
			m_dTokenNumber	= atof( m_pszTokenString );
			getTokenType	= TOKEN_NUMBER;
		}	
		break;
	default: //< 巩磊凯
		{
			*(pbp++) = c;
			while( ( c = _getchar() ) != TOKEN_END && _isspace( c ) == 0 )
	
			{
				*(pbp++) = c;
			}
			*pbp = NULL;
			_rewind(1);
			getTokenType	= TOKEN_STRING;
		}
	}

#ifdef _DEBUG
	if( desireTokenType != TOKEN_ANYTYPE && getTokenType != desireTokenType )
	{
		assert( !"从配置文件读到的数据类型不正常,可能是字符串以数字开头，或者夹杂半角空格和逗号" );
	}
#endif

	return getTokenType;
}


eTOKEN_TYPE	 SolarFileReader::_getNextTokenType_quotationForStr( eTOKEN_TYPE desireTokenType )
{
	desireTokenType;
	GETVALIDDATA

	switch( c )
	{
	case TOKEN_END: break;
	// 箭磊
	case '0':	case '1':	case '2':	case '3':	case '4':
	case '5':	case '6':	case '7':	case '8':	case '9':
	case '.':	case '-':	case '+':
		{
			*(pbp++) = c;
			while( ( c = _getchar() ) != TOKEN_END && ( isdigit( c ) != 0 || c == '.' || c == 'f') )
			{
				*(pbp++) = c;
			}
			*pbp = NULL;
			_rewind(1);
			m_dTokenNumber	= atof( m_pszTokenString );
			getTokenType	= TOKEN_NUMBER;
		}	
		break;
	case '"': //< 巩磊凯
		{
			while( ( c = _getchar() ) != TOKEN_END && ( c != '"' ) )
			{
				if( c == '\\') c = _getchar();
				*(pbp++) = c;
			}
			*pbp = NULL;
			getTokenType	= TOKEN_STRING;
		}
		break;
	default: //< 屈侥俊 嘎瘤 臼绰 巩磊甸
		{
			*(pbp++) = c;
			*pbp = NULL;
			getTokenType	= TOKEN_EXTRA;
		}
	}

#ifdef _DEBUG
	if( desireTokenType != TOKEN_ANYTYPE && getTokenType != desireTokenType )
	{
		assert( !"你要的 TokenType 不相同. : 数据可能已损坏" );
	}
#endif

	return getTokenType;
}

eTOKEN_TYPE	SolarFileReader::_getNextTokenType_newline( eTOKEN_TYPE desireTokenType )
{
	desireTokenType;
	GETVALIDDATA
	switch( c )
	{
	case TOKEN_END: break;
	default: // 箭磊, 巩磊, 傍归俊 包拌绝捞 无条件 new line 父阑 盒府磊肺 牢侥
		{
			*(pbp++) = c;
			while( ( c = _getchar() ) != TOKEN_END && _isnewline( c ) == 0 )
	
			{
				*(pbp++) = c;
			}
			*pbp = NULL;
			_rewind(1);
			getTokenType	= TOKEN_STRING;
		}
	}

#ifdef _DEBUG
	if( desireTokenType != TOKEN_ANYTYPE && getTokenType != desireTokenType )
	{
		assert( !"与你要的 TokenType 不相同. : 数据可能已损坏" );
	}
#endif

	return getTokenType;
}

eTOKEN_TYPE	SolarFileReader::_getNextTokenType_whitespace_onlystring( eTOKEN_TYPE desireTokenType )
{
	desireTokenType;
	GETVALIDDATA

	switch( c )
	{
	case TOKEN_END: break;
	default: // 坷肺瘤 巩磊凯肺父 牢侥
		{
			*(pbp++) = c;
			while( ( c = _getchar() ) != TOKEN_END && _isspace( c ) == 0 )
	
			{
				*(pbp++) = c;
			}
			*pbp = NULL;
			_rewind(1);
			getTokenType	= TOKEN_STRING;
		}
	}

#ifdef _DEBUG
	if( desireTokenType != TOKEN_ANYTYPE && getTokenType != desireTokenType )
	{
		assert( !"读到的类型与想获取的类型不一致" );
	}
#endif

	return getTokenType;
}

eTOKEN_TYPE	SolarFileReader::_getNextTokenType_tab(eTOKEN_TYPE desireTokenType)
{
	GETVALIDDATA
	static std::string str;
	//switch( c )
	//{
	//case TOKEN_END: break;
	//	// 箭磊
	//case '0':	case '1':	case '2':	case '3':	case '4':
	//case '5':	case '6':	case '7':	case '8':	case '9':
	//case '.':	case '-':	case '+':
	//	{
	//		*(pbp++) = c;
	//		while( ( c = _getchar() ) != TOKEN_END && ( isdigit( c ) != 0 || c == '.' || c == 'f') )
	//		{
	//			*(pbp++) = c;
	//		}
	//		*pbp = NULL;
	//		_rewind(1);
	//		m_dTokenNumber	= atof( m_pszTokenString );
	//		getTokenType	= TOKEN_NUMBER;
	//	}	
	//	break;
	//default: //< 巩磊凯
	if (TOKEN_END != c)
	{
		str.push_back(c);
		bool flag = ('\"' == c) ? true : false;

		while( ( c = _getchar() ) != TOKEN_END && _isnewline( c ) == 0 && TOKEN_TAB != c)
		{
			if ('\"' != c)
			{
				if (flag)
				{
					flag = false;
					/**(--pbp) = c;
					++pbp;*/
					str.back() = c;
				}
				else
				{
					//*(pbp++) = c;
					str.push_back(c);
				}
			}
			else
			{
				if (!flag)
				{
					flag = true;
				}

				//*(pbp++) = c;
				str.push_back(c);
			}
		}

		if (flag)
			//*(--pbp) = '\0';
			str.pop_back();
		//else
			//*pbp = '\0';

		_rewind(1);
		m_dTokenNumber	= atof(str.c_str());

		if (str.empty()		||
			"0"	==	str		|| 
			"NULL" == str	|| 
			" " == str)
			m_pszTokenString[0] = '\0';
		else
		{
			_parsercell(str);
			strncpy(pbp, str.c_str(), 1024);
		}

		getTokenType	= TOKEN_STRING;
	}
	//}
	str.clear();
	return getTokenType;
}

eTOKEN_TYPE SolarFileReader::GetNextTokenType( eTOKEN_TYPE desireTokenType )
{
	_clear();

	switch( m_Mode )
	{
	case SEPERATOR_WHITESPACE:
		return _getNextTokenType_whitespace( desireTokenType );
	case SEPERATOR_QUOTATIONMARKFORSTR:
		return _getNextTokenType_quotationForStr( desireTokenType );
	case SEPERATOR_ONLY_NEWLINE:
		return _getNextTokenType_newline( desireTokenType );
	case SEPERATOR_WHITESPACE_ONLY_STRING :
		return _getNextTokenType_whitespace_onlystring( desireTokenType );
	case SEPERATOR_TAB:
		return _getNextTokenType_tab(desireTokenType);
	}
	return TOKEN_ERROR;
}

BOOL SolarFileReader::IsEOF()
{
	DWORD m_CurPos = m_dwBufferPos;
	char c = GetValidData();
	m_dwBufferPos = m_CurPos;
	if(TOKEN_END == c)
		return TRUE;
	return FALSE;
}

char * SolarFileReader::GetNextString( eTOKEN_TYPE desireTokenType )
{
	GetNextTokenType( desireTokenType );
	return GetTokenString();
}

double SolarFileReader::GetNextNumber( eTOKEN_TYPE desireTokenType )
{
	GetNextTokenType( desireTokenType );
	return GetTokenNumber();
}

}


//=====================================================================================================

BaseParser::BaseParser()
{
	memset( m_szPackFileName, 0, MAX_PATH );
}

BaseParser::~BaseParser()
{
}

void BaseParser::SetPackFileName( const char * pszPackFileName )
{
	if( !pszPackFileName )
	{
		return;
	}
	strcpy_s( m_szPackFileName, MAX_PATH, pszPackFileName );
}

BOOL BaseParser::OpenFile( util::SolarFileReader & sr, const char * pszFileName, eSEPERATOR_MODE mode )
{
	BOOL bResult  = sr.OpenFile( pszFileName, mode );
	if( !bResult )
	{
		assert( !"File Open Error" );
	}

	return bResult;
}
