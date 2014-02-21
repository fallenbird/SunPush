#ifndef __SOLAR_FILE_READER_H__
#define __SOLAR_FILE_READER_H__

#include "UtilityCommon.h"


enum eTOKEN_TYPE
{
	TOKEN_ANYTYPE			= -1,
	TOKEN_ERROR				= 0,
	TOKEN_END,
	TOKEN_NUMBER,
	TOKEN_STRING,
	TOKEN_EXTRA,
	TOKEN_SLASH				= '/',
	TOKEN_ASTERISK			= '*',

	// white-space
	TOKEN_SPACE				= ' ',
	TOKEN_NEWLINE			= '\n',
	TOKEN_CARRIAGERETURN	= '\r', 
	TOKEN_TAB				= '\t',
};

enum eSEPERATOR_MODE
{
	SEPERATOR_ERROR						= 0x00,
	SEPERATOR_WHITESPACE				= 0x01,
	SEPERATOR_QUOTATIONMARKFORSTR		= 0x02,
	SEPERATOR_ONLY_NEWLINE				= 0x03,
	SEPERATOR_WHITESPACE_ONLY_STRING	= 0x04,
	SEPERATOR_TAB						= 0x05,
};

namespace util 
{
typedef const char * (*LanFindCallback)(const char *);

class SolarFileReader
{
	enum 
	{
		MAX_TOKEN_BUFFER_SIZE			= 1024,
	};
public:
	SolarFileReader(LanFindCallback funcCB = NULL);
	~SolarFileReader();

	BOOL OpenFile( const char * pszFileName, eSEPERATOR_MODE mode = SEPERATOR_WHITESPACE );
	BOOL				OpenBuffer( BYTE * pMemoryBuffer, DWORD dwBufferSize, eSEPERATOR_MODE mode = SEPERATOR_WHITESPACE );
	void				CloseBuffer();
	void				CloseFile();

	eTOKEN_TYPE			GetNextTokenType( eTOKEN_TYPE desireTokenType = TOKEN_ANYTYPE );

	char *				GetTokenString() { return m_pszTokenString;	}
	double				GetTokenNumber() { return m_dTokenNumber;	}
	DWORD				GetTokenDword()
	{
		return DWORD(GetTokenNumber());
	}

	WORD				GetTokenWord()
	{ 
		return (WORD)(GetTokenNumber());
	}

	BYTE				GetTokenByte()
	{ 
		return (BYTE)(GetTokenNumber());
	}

	ULONGLONG			GetTokenULLong()
	{ 
		return (ULONGLONG)(GetTokenNumber() );
	}

	SHORT				GetTokenShort()
	{ 
		return (SHORT)(GetTokenNumber() );
	}

	float				GetTokenFloat()
	{
		return (float)(GetTokenNumber() );
	}

	int					GetTokenInt()
	{ 
		return int( GetTokenNumber() );
	}

	bool GetTokenBool()
	{
		const float EPSINON = 0.00001f; 
		if (  ( GetTokenNumber() >= - EPSINON ) && ( GetTokenNumber() <= EPSINON ) )
		{
			return false;
		}
		return true;
	}

	long GetTokenLong()
	{ 
		return long( GetTokenNumber() );
	}

	ULONG64				GetTokenUlong64()
	{
		return ULONG64(GetTokenNumber() );
	}

	BOOL				IsEOF();
	char *				GetNextString( eTOKEN_TYPE desireTokenType = TOKEN_STRING );
	double				GetNextNumber( eTOKEN_TYPE desireTokenType = TOKEN_NUMBER );
	void				Reset();
private:
	void				_clear();
	char				_getchar( DWORD * pPos = NULL );
	char				_getcurchar( DWORD * pPos = NULL );
	void				_rewind( int i , DWORD * pPos = NULL );
	int					_isspace( char c );
	int					_isnewline( char c );
	BOOL				_isnumber( char c );
	void				_parsercell(std::string & str);
	char				GetValidData();// 简化冗余并修正原代码的错误 [8/5/2009 13:02:36 CHANCY]
	eTOKEN_TYPE			_getNextTokenType_whitespace( eTOKEN_TYPE desireTokenType );
	eTOKEN_TYPE			_getNextTokenType_quotationForStr( eTOKEN_TYPE desireTokenType );
	eTOKEN_TYPE			_getNextTokenType_newline( eTOKEN_TYPE desireTokenType );
	eTOKEN_TYPE			_getNextTokenType_whitespace_onlystring( eTOKEN_TYPE desireTokenType );
	eTOKEN_TYPE			_getNextTokenType_tab(eTOKEN_TYPE desireTokenType);

	double				m_dTokenNumber;
	char				m_pszTokenString[MAX_TOKEN_BUFFER_SIZE];
	DWORD				m_dwBufferPos;
	DWORD				m_dwBufferSize;
	char *				m_pszBuffer;
	LanFindCallback		m_funcCB;
	eSEPERATOR_MODE		m_Mode;
	char				m_szFileName[MAX_PATH] ;//  [2009-7-28 10:27:57  ]
};


} /// namespace util


//=======================================================================================================

class BaseParser
{
public :

	BaseParser();
	virtual ~BaseParser();
	void SetPackFileName( const char * pszPackFileName );

private :
	BOOL OpenFile( util::SolarFileReader & sr, const char * pszFileName, eSEPERATOR_MODE mode );
	char	m_szPackFileName[MAX_PATH];
};

#endif // __SOLAR_FILE_READER_H__