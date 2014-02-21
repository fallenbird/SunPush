#include "UtilityCommon.h"
#include "Global.h"
#include <tchar.h>


char * buffer2048()
{
	static char buffer[2048] = { '\0' };
	return &buffer[0];
}



string & ReplaceAllDistinct(string & str, const string & old_value, const string & new_value) 
{   
	if (old_value.empty())
		return str;

	if (old_value == new_value)
		return str;

    for( string::size_type pos(0); pos != string::npos; pos += new_value.length())
	{   
        if( (pos = str.find(old_value, pos)) != string::npos)  
		{
            str.replace(pos, old_value.length(), new_value);
		}
        else
		{
			break;   
		}
    }

    return str;
}


