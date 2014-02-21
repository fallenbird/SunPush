// WzDataType.cpp: Webzen common data type definitions
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <windows.h>
#include <assert.h>
#include "WzDataType.h"



WzVector* VectorNormalize( WzVector *pwvOut, WzVector *pwvIn)
{
	float fLength = VectorLength( pwvIn);
	if ( fLength > 0.0f)
	{
		pwvOut->x = pwvIn->x / fLength;
		pwvOut->y = pwvIn->y / fLength;
		pwvOut->z = pwvIn->z / fLength;
	}
	else
	{
		*pwvOut = *pwvIn;
	}

	return ( pwvOut);
}
