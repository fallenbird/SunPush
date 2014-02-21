#ifndef N_STRING_H
#define N_STRING_H
//------------------------------------------------------------------------------
/**
    @스트링 잚깎 

    @author
    - RadonLabs GmbH 

    @since
    - 2005.6.13
    @remarks
    - 지완 추가 
*/
//------------------------------------------------------------------------------

#ifndef _NSTRING_H_
#define _NSTRING_H_


#include <ctype.h>
#include <string.h>
#include "Narray.h"


class nString
{
public:
    /// constructor
    nString();
    /// constructor 1
    nString(const char* str);
    /// copy constructor
    nString(const nString& rhs);
    /// set to int val
    nString(int intVal);
    /// set to float val
    nString(float floatVal);
    /// destructor
    ~nString();
    /// = operator
    nString& operator=(const nString& rhs);
    /// = operator with string
    nString& operator=(const char* rhs);
    /// += operator with char*
    nString& operator+=(const char* rhs);
    /// += operator with string
    nString& operator+=(const nString& rhs);
    /// Is `a' equal to `b'?
    friend bool operator == (const nString& a, const nString& b);
    /// Is `a' inequal to `b'?
    friend bool operator != (const nString& a, const nString& b);
    /// Subscript operator (read only).
    const char operator[](int i) const;
    /// Subscript operator (writeable).
    char& operator[](int i);
    /// set as char ptr, with explicit length
    void Set(const char* ptr, int length);
    /// set as char ptr
    void Set(const char* str);
    /// set as int value
    void SetInt(int val);
    /// set as float value
    void SetFloat(float val);
    /// get string as char ptr
    const char* Get() const;
    /// return content as integer
    int AsInt() const;
    /// return content as float
    float AsFloat() const;
    /// return length of string
    int Length() const;
    /// clear the string
    void Clear();
    /// return true if string object is empty
    bool IsEmpty() const;
    /// append character pointer
    void Append(const char* str);
    /// append string
    void Append(const nString& str);
    /// append a range of characters
    void AppendRange(const char* str, uint numChars);
    /// append int value
    void AppendInt(int val);
    /// append float value
    void AppendFloat(float val);
    /// convert string to lower case
	void ToLower();
    /// convert string to upper case
	void ToUpper();
    /// get first token (this will destroy the string)
    const char* GetFirstToken(const char* whiteSpace);
    /// get next token (this will destroy the string)
    const char* GetNextToken(const char* whiteSpace);
    /// tokenize string into a provided nString array
    int Tokenize(const char* whiteSpace, nArray<nString>& tokens) const;
    /// extract substring
    nString ExtractRange(int from, int to) const;
    /// terminate string at first occurence of character in set
    void Strip(const char* charSet);
    /// Index of first appearance of `v' starting from index `startIndex'.
    int IndexOf(const nString& v, int startIndex) const;
    /// return index of character in string
    int FindChar(unsigned char c, int startIndex) const;
    /// terminate string at given index
    void TerminateAtIndex(int index);
    /// strip slash at end of path, if exists
    void StripTrailingSlash();
    /// delete characters from charset at left side of string
    nString TrimLeft(const char* charSet) const;
    /// delete characters from charset at right side of string
    nString TrimRight(const char* charSet) const;
    /// trim characters from charset at both sides of string
    nString Trim(const char* charSet) const;
    /// substitute every occurance of a string with another string
    nString Substitute(const char* str, const char* substStr) const;
    /// convert string inplace from UTF-8 to 8-bit ANSI
    void UTF8toANSI();
    /// convert ANSI to UTF-8 in place
    void ANSItoUTF8();
    /// get pointer to extension (without the dot)
    const char* GetExtension() const;
    /// check if extension matches (no dot in the extension!)
    bool CheckExtension(const char* ext) const;
    /// convert backslashes to slashes
    void ConvertBackslashes();
    /// remove extension
    void StripExtension();
    /// extract the part after the last directory separator
    nString ExtractFileName() const;
    /// extract the last directory of the path
    nString ExtractLastDirName() const;
    /// extract the part before the last directory separator
    nString ExtractDirName() const;
    /// extract path until last slash
    nString ExtractToLastSlash() const;
    /// check if this string matches the given pattern
   // bool MatchPattern(const nString& pattern) const;
    /// 숫자 인지 검사 
    bool IsNumberic();


protected:
    /// copy contents
    void Copy(const nString& src);
    /// delete contents
    void Delete();
    /// get pointer to last directory separator
    char* GetLastSlash() const;

    enum
    {
        LOCALSTRINGSIZE = 14,
    };
    char* string;
    char localString[LOCALSTRINGSIZE];
    ushort strLen;
};

//------------------------------------------------------------------------------
/** global
*/
/*
bool 
n_strmatch(const char* str, const char* pat)
{
    char c2;

    while (true) 
    {
        if (*pat == 0) 
        {
            if (*str == 0) return true;
            else           return false;
        }
        if ((*str == 0) && (*pat != '*')) return false;
        if (*pat=='*') 
        {
            pat++;
            if (*pat==0) return true;
            while (true) 
            {
                if (n_strmatch(str, pat)) return true;
                if (*str==0) return false;
                str++;
            }
        }
        if (*pat=='?') goto match;
        if (*pat=='[') 
        {
            pat++;
            while (true) 
            {
                if ((*pat==']') || (*pat==0)) return false;
                if (*pat==*str) break;
                if (pat[1] == '-') 
                {
                    c2 = pat[2];
                    if (c2==0) return false;
                    if ((*pat<=*str) && (c2>=*str)) break;
                    if ((*pat>=*str) && (c2<=*str)) break;
                    pat+=2;
                }
                pat++;
            }
            while (*pat!=']') 
            {
                if (*pat==0) 
                {
                    pat--;
                    break;
                }
                pat++;
            }
            goto match;
        }

        if (*pat=='\\') 
        {
            pat++;
            if (*pat==0) return false;
        }
        if (*pat!=*str) return false;

match:
        pat++;
        str++;
    }
}
*/
//------------------------------------------------------------------------------
/**
*/
inline
nString::nString() :
    string(0),
    strLen(0)
{
    this->localString[0] = 0;
}

//------------------------------------------------------------------------------
/**
*/
inline
void
nString::Delete()
{
    if (this->string)
    {
        free((void*) this->string);
        this->string = 0;
    }
    this->localString[0] = 0;
}

//------------------------------------------------------------------------------
/**
*/
inline
nString::~nString()
{
    this->Delete();
}

//------------------------------------------------------------------------------
/**
*/
inline
void
nString::Set(const char* str, int length)
{
    this->Delete();
    if (str)
    {
        this->strLen = safe_cast<ushort>(length);
        char* ptr = this->localString;
        if (strLen >= LOCALSTRINGSIZE)
        {
            ptr = (char*) malloc(strLen + 1);
            this->string = ptr;
        }
        else
        {
            ptr = this->localString;
        }
        int i;
        for (i = 0; i < strLen; i++)
        {
            *ptr++ = *str++;
        }
        *ptr = 0;
    }
}

//------------------------------------------------------------------------------
/**
*/
inline
void
nString::Set(const char* str)
{
    int len = 0;
    if (str)
    {
        len = strlen(str);
    }
    this->Set(str, len);
}

//------------------------------------------------------------------------------
/**
*/
inline
void
nString::SetInt(int val)
{
    char buf[128];
    _snprintf(buf, sizeof(buf), "%d", val);
    this->Set(buf);
}

//------------------------------------------------------------------------------
/**
*/
inline
void
nString::SetFloat(float val)
{
    char buf[128];
    _snprintf(buf, sizeof(buf), "%.6f", val);
    this->Set(buf);
}

//------------------------------------------------------------------------------
/**
*/
inline
void
nString::Copy(const nString& src)
{
    ASSERT(0 == this->string);
    const char* str = src.Get();
    if (str)
    {
        this->Set(str);
    }
}

//------------------------------------------------------------------------------
/**
*/
inline
nString::nString(const char* str) :
    string(0),
    strLen(0)
{
    this->localString[0] = 0;
    this->Set(str);
}

//------------------------------------------------------------------------------
/**
*/
inline
nString::nString(const nString& rhs) :
    string(0),
    strLen(0)
{
    this->localString[0] = 0;
    this->Copy(rhs);
}

//------------------------------------------------------------------------------
/**
*/
inline
nString::nString(int intVal) :
    string(0),
    strLen(0)
{
    this->SetInt(intVal);
}

//------------------------------------------------------------------------------
/**
*/
inline
nString::nString(float floatVal) :
    string(0),
    strLen(0)
{
    this->SetFloat(floatVal);
}

//------------------------------------------------------------------------------
/**
*/
inline
const char*
nString::Get() const
{
    if (this->string)
    {
        return this->string;
    }
    else if (this->localString[0])
    {
        return this->localString;
    }
    else
    {
        return "";
    }
}

//------------------------------------------------------------------------------
/**
*/
inline
nString&
nString::operator=(const nString& rhs)
{
    if (&rhs != this)
    {
        this->Delete();
        this->Copy(rhs);
    }
    return *this;
}

//------------------------------------------------------------------------------
/**
*/
inline
nString&
nString::operator=(const char* rhs)
{
    this->Set(rhs);
    return *this;
}

//------------------------------------------------------------------------------
/**
*/
inline
void
nString::AppendRange(const char* str, uint numChars)
{
    ASSERT(NULL != str);
    if (numChars > 0)
    {
        ushort rlen = safe_cast<ushort>(numChars);
        ushort tlen = this->strLen + rlen;
        if (this->string)
        {
            char* ptr = (char*) malloc(tlen + 1);
            strcpy(ptr, this->string);
            strncat(ptr, str, numChars);
            free((void*) this->string);
            this->string = ptr;
            this->strLen = tlen;
        }
        else if (this->localString[0])
        {
            if (tlen >= LOCALSTRINGSIZE)
            {
                char* ptr = (char*) malloc(tlen + 1);
                strcpy(ptr, this->localString);
                strncat(ptr, str, numChars);
                this->localString[0] = 0;
                this->string = ptr;
            }
            else
            {
                strncat(this->localString, str, numChars);
            }
            this->strLen = tlen;
        }
        else
        {
            this->Set(str, numChars);
        }
    }
}

//------------------------------------------------------------------------------
/**
*/
inline
void
nString::Append(const char* str)
{
    CHECK_PTR(str);
    ushort rlen = safe_cast<ushort>( strlen(str) );
    this->AppendRange(str, rlen);
}

//------------------------------------------------------------------------------
/**
*/
inline
void
nString::Append(const nString& str)
{
    this->Append(str.Get());
}

//------------------------------------------------------------------------------
/**
*/
inline
void
nString::AppendInt(int val)
{
    nString str;
    str.SetInt(val);
    this->Append(str);
}

//------------------------------------------------------------------------------
/**
*/
inline
void
nString::AppendFloat(float val)
{
    nString str;
    str.SetFloat(val);
    this->Append(str);
}

//------------------------------------------------------------------------------
/**
*/
inline
nString&
nString::operator+=(const char* rhs)
{
    this->Append(rhs);
    return *this;
}

//------------------------------------------------------------------------------
/**
*/
inline
nString&
nString::operator+=(const nString& rhs)
{
    this->Append(rhs.Get());
    return *this;
}

//------------------------------------------------------------------------------
/**
*/
inline
bool
operator == (const nString& a, const nString& b)
{
    return strcmp(a.Get(), b.Get()) == 0;
}

//------------------------------------------------------------------------------
/**
*/
inline
bool
operator != (const nString& a, const nString& b)
{
    return strcmp(a.Get(), b.Get()) != 0;
}

//------------------------------------------------------------------------------
/**
*/
inline
const char
nString::operator[](int i) const
{
    ASSERT((0 <= i) && (i <= (strLen - 1)));
    if (this->string != 0)
    {
        return this->string[i];
    }
    else
    {
        return this->localString[i];
    }
}

//------------------------------------------------------------------------------
/**
*/
inline
char&
nString::operator[](int i)
{
    ASSERT((0 <= i) && (i <= (strLen - 1)));
    if (this->string != 0)
    {
        return this->string[i];
    }
    else
    {
        return this->localString[i];
    }
}

//------------------------------------------------------------------------------
/**
*/
inline
int
nString::Length() const
{
    return this->strLen;
}

//------------------------------------------------------------------------------
/**
*/
inline
void
nString::Clear()
{
    this->Delete();
}

//------------------------------------------------------------------------------
/**
*/
inline
bool
nString::IsEmpty() const
{
    if (this->string && (this->string[0] != 0))
    {
        return false;
    }
    else if (this->localString[0] != 0)
    {
        return false;
    }
    return true;
}

//------------------------------------------------------------------------------
/**
*/
inline
void
nString::ToLower()
{
    char* str = (char*) (this->string ? this->string : this->localString);
    if (str)
    {
        char c;
        char* ptr = (char*) str;
        while ((c = *ptr))
        {
            *ptr++ = safe_cast<char>(tolower(c));
        }
    }
}

//------------------------------------------------------------------------------
/**
*/
inline
void
nString::ToUpper()
{
    char* str = (char*) (this->string ? this->string : this->localString);
    if (str)
    {
        char c;
        char* ptr = (char*) str;
        while ((c = *ptr))
        {
            *ptr++ = safe_cast<char>(toupper(c));
        }
    }
}

//------------------------------------------------------------------------------
/**
*/
static
inline
nString
operator+(const nString& s0, const nString& s1)
{
    nString newString = s0;
    newString.Append(s1.Get());
    return newString;
}

//------------------------------------------------------------------------------
/**
    Get the first token in the string, delimited by one or more of the
    character in 'whiteSpace'. This simply invokes the strtok() function
    internally, and will destroy the contained string. After calling
    GetFirstToken(), invoke GetNextToken() until 0 returns.

    ATTENTION: if somebody else calls strtok() while GetFirstToken()/
    GetNextToken() is underway, everything will break apart!
    Check out the Tokenize() method for a better alternative.

    @param  whiteSpace  string containing white space characters
*/
inline
const char*
nString::GetFirstToken(const char* whiteSpace)
{
    CHECKNULL_PTR(whiteSpace);
    return strtok((char*) this->Get(), whiteSpace);
}

//------------------------------------------------------------------------------
/**
    Get the next token in the string. Call this after a GetFirstToken()
    or GetNextToken(). Returns 0 if no more tokens in string. This method
    will destroy the original string.

    ATTENTION: if somebody else calls strtok() while GetFirstToken()/
    GetNextToken() is underway, everything will break apart!
    Check out the Tokenize() method for a better alternative.

    @param  whiteSpace  string containing whitespace characters
*/
inline
const char*
nString::GetNextToken(const char* whiteSpace)
{
    CHECKNULL_PTR(whiteSpace);
    return strtok(0, whiteSpace);
}

//------------------------------------------------------------------------------
/**
    Tokenize the string into a provided nString array. Returns the number
    of tokens. This method is recommended over GetFirstToken()/GetNextToken(),
    since it is atomic. This nString object will not be destroyed
    (as is the case with GetFirstToken()/GetNextToken().

    @param  whiteSpace      [in] a string containing the whitespace characters
    @param  tokens          [out] nArray<nString> where tokens will be appended
    @return                 number of tokens found
*/
inline
int
nString::Tokenize(const char* whiteSpace, nArray<nString>& tokens) const
{
    int numTokens = 0;

    // create a temporary string, which will be destroyed during the operation
    nString str(*this);
    char* ptr = (char*) str.Get();
    const char* token;
    while (0 != (token = strtok(ptr, whiteSpace)))
    {
        tokens.Append(nString(token));
        ptr = 0;
        numTokens++;
    }
    return numTokens;
}

//------------------------------------------------------------------------------
/**
    Extract sub string.
*/
inline
nString
nString::ExtractRange(int from, int numChars) const
{
    ASSERT(from <= this->strLen);
    ASSERT((from + numChars) <= this->strLen);
    const char* str = this->Get();
    nString newString;
    newString.Set(&(str[from]), numChars);
    return newString;
}

//------------------------------------------------------------------------------
/**
    Terminates the string at the first occurance of one of the characters
    in charSet.
*/
inline
void
nString::Strip(const char* charSet)
{
    CHECK_PTR(charSet);
    char* str = (char*) this->Get();
    char* ptr = strpbrk(str, charSet);
    if (ptr)
    {
        *ptr = 0;
    }
}

//------------------------------------------------------------------------------
/**
    @result Index or -1 if not found.
*/
inline
int
nString::IndexOf(const nString& v, int startIndex) const
{
    ASSERT(0 <= startIndex && startIndex <= Length() - 1);
    ASSERT(!v.IsEmpty());

    for (int i = startIndex; i < Length(); i++)
    {
        if (Length() - i < v.Length())
        {
            break;
        }

        if (strncmp(&(Get()[i]), v.Get(), v.Length()) == 0)
        {
            return i;
        }
    }

    return -1;
}

//------------------------------------------------------------------------------
/**
    Return index of character in string, or -1 if not found.
*/
inline
int
nString::FindChar(unsigned char c, int startIndex) const
{
    if (this->Length() > 0)
    {
        ASSERT(startIndex < this->Length());
        const char* ptr = strchr(this->Get() + startIndex, c);
        if (ptr)
        {
            return ptr - this->Get();
        }
    }
    return -1;
}

//------------------------------------------------------------------------------
/**
    Terminates the string at the given index.
*/
inline
void
nString::TerminateAtIndex(int index)
{
    ASSERT(index < this->Length());
    char* ptr = (char*) this->Get();
    ptr[index] = 0;
}

//------------------------------------------------------------------------------
/**
    Strips last slash, if the path name ends on a slash.
*/
inline
void
nString::StripTrailingSlash()
{
    if (this->strLen > 0)
    {
        int pos = strLen - 1;
        char* str = (char*) this->Get();
        if ((str[pos] == '/') || (str[pos] == '\\'))
        {
            str[pos] = 0;
        }
    }
}

//------------------------------------------------------------------------------
/**
    Returns a new string which is this string, stripped on the left side 
    by all characters in the char set.
*/
inline
nString
nString::TrimLeft(const char* charSet) const
{
    ASSERT(NULL != charSet);
    if (this->IsEmpty())
    {
        return *this;
    }

    int charSetLen = strlen(charSet);
    int thisIndex = 0;
    bool stopped = false;
    while (!stopped && (thisIndex < this->strLen))
    {
        int charSetIndex;
        bool match = false;
        for (charSetIndex = 0; charSetIndex < charSetLen; charSetIndex++)
        {
            if ((*this)[thisIndex] == charSet[charSetIndex])
            {
                // a match
                match = true;
                break;
            }
        }
        if (!match)
        {
            // stop if no match
            stopped = true;
        }
        else
        {
            // a match, advance to next character
            ++thisIndex;
        }
    }
    nString trimmedString(&(this->Get()[thisIndex]));
    return trimmedString;
}

//------------------------------------------------------------------------------
/**
    Returns a new string, which is this string, stripped on the right side 
    by all characters in the char set.
*/
inline
nString
nString::TrimRight(const char* charSet) const
{
    ASSERT(NULL != charSet);
    if (this->IsEmpty())
    {
        return *this;
    }

    int charSetLen = strlen(charSet);
    int thisIndex = this->strLen - 1;
    bool stopped = false;
    while (!stopped && (thisIndex < this->strLen))
    {
        int charSetIndex;
        bool match = false;
        for (charSetIndex = 0; charSetIndex < charSetLen; charSetIndex++)
        {
            if ((*this)[thisIndex] == charSet[charSetIndex])
            {
                // a match
                match = true;
                break;
            }
        }
        if (!match)
        {
            // stop if no match
            stopped = true;
        }
        else
        {
            // a match, advance to next character
            --thisIndex;
        }
    }
    nString trimmedString;
    trimmedString.Set(this->Get(), thisIndex + 1);
    return trimmedString;
}

//------------------------------------------------------------------------------
/**
    Trim both sides of a string.
*/
inline
nString
nString::Trim(const char* charSet) const
{
    return this->TrimLeft(charSet).TrimRight(charSet);
}

//------------------------------------------------------------------------------
/**
    Substitute every occurance of origStr with substStr.
*/
inline
nString
nString::Substitute(const char* matchStr, const char* substStr) const
{
    ASSERT(matchStr && substStr);

    const char* ptr = this->Get();
    int matchStrLen = strlen(matchStr);
    nString dest;

    // walk original string for occurances of str
    const char* occur;
    while ((occur = strstr(ptr, matchStr)))
    {
        // append string fragment until match
        dest.AppendRange(ptr, occur - ptr);

        // append replacement string
        dest.Append(substStr);

        // adjust source pointer
        ptr = occur + matchStrLen;
    }
    dest.Append(ptr);
    return dest;
}

//------------------------------------------------------------------------------
/**
    Returns content as integer.
*/
inline
int
nString::AsInt() const
{
    const char* ptr = this->Get();
    return atoi(ptr);
}

//------------------------------------------------------------------------------
/**
    Returns content as float.
*/
inline
float
nString::AsFloat() const
{
    const char* ptr = this->Get();
    return float(atof(ptr));
}

//------------------------------------------------------------------------------
/**
    This converts an UTF-8 string to 8-bit-ANSI. Note that only characters
    in the range 0 .. 255 are converted, all other characters will be converted
    to a question mark.

    For conversion rules see http://www.cl.cam.ac.uk/~mgk25/unicode.html#utf-8
*/
inline
void
nString::UTF8toANSI()
{
    uchar* src = (uchar*) this->Get();
    uchar* dst = src;
    uchar c;
    while ((c = *src++))
    {
        if (c >= 0x80)
        {
            if ((c & 0xE0) == 0xC0)
            {
                // a 2 byte sequence with 11 bits of information
                ushort wide = ((c & 0x1F) << 6) | (*src++ & 0x3F);
                if (wide > 0xff)
                {
                    c = '?';
                }
                else
                {
                    c = (uchar) wide;
                }
            }
            else if ((c & 0xF0) == 0xE0)
            {
                // a 3 byte sequence with 16 bits of information
                c = '?';
                src += 2;
            }
            else if ((c & 0xF8) == 0xF0)
            {
                // a 4 byte sequence with 21 bits of information
                c = '?';
                src += 3;
            }
            else if ((c & 0xFC) == 0xF8)
            {
                // a 5 byte sequence with 26 bits of information
                c = '?';
                src += 4;
            }
            else if ((c & 0xFE) == 0xFC)
            {
                // a 6 byte sequence with 31 bits of information
                c = '?';
                src += 5;
            }
        }
        *dst++ = c;
    }
    *dst = 0;
}
//------------------------------------------------------------------------------
/**
    Convert contained ANSI string to UTF-8 in place.
*/
inline
void
nString::ANSItoUTF8()
{
    ASSERT(!this->IsEmpty());
    int bufSize = this->Length() * 2 + 1;
    char* buffer = new char[bufSize];
    char* dstPtr = buffer;
    const char* srcPtr = this->Get();
    unsigned char c;
    while ((c = *srcPtr++))
    {
        // note: this only covers the 2 cases that the character
        // is between 0 and 127 and between 128 and 255
        if (c < 128)
        {
            *dstPtr++ = c;
        }
        else
        {
            *dstPtr++ = 192 + (c / 64);
            *dstPtr++ = 128 + (c % 64);
        }
    }
    *dstPtr = 0;
    this->Set(buffer);
    delete[] buffer;
}

//------------------------------------------------------------------------------
/**
    Converts backslashes to slashes.
*/
inline
void
nString::ConvertBackslashes()
{
    char* ptr = (char*) this->Get();
    int i;
    for (i = 0; i <= this->strLen; i++)
    {
        if (ptr[i] == '\\')
        {
            ptr[i] = '/';
        }
    }
}

//------------------------------------------------------------------------------
/**
    @return     pointer to extension (without the dot), or 0
*/
inline
const char*
nString::GetExtension() const
{
    const char* str = this->Get();
    const char* ext = strrchr(str, '.');
    if (ext)
    {
        ext++;
        if (ext[0] != 0)
        {
            return ext;
        }
    }
    return 0;
}

//------------------------------------------------------------------------------
/**
    Returns true if file extension matches.

    @param  ext     extension string (without the dot)
    @return         true if extension matches
*/
inline
bool
nString::CheckExtension(const char* ext) const
{
    CHECKF_PTR(ext);
    const char* extStr = this->GetExtension();
	if (0 == extStr)
    {
		return false;
    }
    return (0 == (strcmp(ext, extStr)));
}

//------------------------------------------------------------------------------
/**
    Remove the file extension.
*/
inline
void
nString::StripExtension()
{
    char* ext = (char*) this->GetExtension();
    if (ext)
    {
        ext[-1] = 0;
    }
}

//------------------------------------------------------------------------------
/**
    Get a pointer to the last directory separator.
*/
inline
char*
nString::GetLastSlash() const
{
    char* s = (char*) this->Get();
    char* lastSlash = strrchr(s, '/');
    if (0 == lastSlash) lastSlash = strrchr(s, '\\');
    if (0 == lastSlash) lastSlash = strrchr(s, ':');
    return lastSlash;
}

//------------------------------------------------------------------------------
/**
    Return a nString object containing the part after the last
    path separator.
*/
inline
nString
nString::ExtractFileName() const
{
    nString pathString;
    char* lastSlash = this->GetLastSlash();
    if (lastSlash)
    {
        pathString = &(lastSlash[1]);
    }
    else
    {
        pathString = this->Get();
    }
    return pathString;
}

//------------------------------------------------------------------------------
/**
    해당 문자열이 숫자로  구성 되었는지 검사 한다-완
*/
inline 
bool 
nString::IsNumberic()
{
    int len=this->Length();
    nString str=this->Get();

    for(int i=0;i<len;i++)
    {
        if(isdigit(str[i])==0)
        {
            return false;
        }
    }

    return true;
}
//------------------------------------------------------------------------------
/**
    Return a nString object containing the last directory of the path, i.e.
    a category.

    - 17-Feb-04     floh    fixed a bug when the path ended with a slash
*/
inline
nString
nString::ExtractLastDirName() const
{
    nString pathString(*this);
    char* lastSlash = pathString.GetLastSlash();
    
    // special case if path ends with a slash
    if (lastSlash)
    {
        if (0 == lastSlash[1])
        {
            *lastSlash = 0;
            lastSlash = pathString.GetLastSlash();
        }

        char* secLastSlash = 0;
        if (0 != lastSlash)
        {
            *lastSlash = 0; // cut filename
            secLastSlash = pathString.GetLastSlash();
            if (secLastSlash)
            {
                *secLastSlash = 0;
                return nString(secLastSlash+1);
            }
        }
    }
    return "";
}

//------------------------------------------------------------------------------
/**
    Return a nString object containing the part before the last
    directory separator.
    
    NOTE: I left my fix in that returns the last slash (or colon), this was 
    necessary to tell if a dirname is a normal directory or an assign.     

    - 17-Feb-04     floh    fixed a bug when the path ended with a slash
*/
inline
nString
nString::ExtractDirName() const
{
    nString pathString(*this);
    char* lastSlash = pathString.GetLastSlash();

    // special case if path ends with a slash
    if (lastSlash)
    {
        if (0 == lastSlash[1])
        {
            *lastSlash = 0;
            lastSlash = pathString.GetLastSlash();
        }
        if (lastSlash)
        {
            *++lastSlash = 0;
        }
    }
    return pathString;
}

//------------------------------------------------------------------------------
/**
    Return a path string object which contains of the complete path
    up to the last slash. Returns an empty string if there is no
    slash in the path.
*/
inline
nString
nString::ExtractToLastSlash() const
{
    nString pathString(*this);
    char* lastSlash = pathString.GetLastSlash();
    if (lastSlash)
    {
        lastSlash[1] = 0;
    }
    else
    {
        pathString = "";
    }
    return pathString;
}

//------------------------------------------------------------------------------
/**
*/
/*
inline
bool
nString::MatchPattern(const nString& pattern) const
{
    return n_strmatch(this->Get(), pattern.Get());
}
*/
//------------------------------------------------------------------------------
#endif

