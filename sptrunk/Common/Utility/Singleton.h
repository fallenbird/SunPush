// Singleton.h: interface for the Singleton class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __SINGLETON_H__
#define __SINGLETON_H__

#include <assert.h>

namespace util
{
template <typename T>
class Singleton
{
protected:
	Singleton()
	{
		assert( !ms_pInstance );
		ms_pInstance = static_cast< T* >( this );
	}

	~Singleton()
	{
		ms_pInstance = 0;
	}
public:
	static T * Instance()
	{
		if (0 == ms_pInstance)
		{
			ms_pInstance = new T;
		}

		return ms_pInstance;
	}

	static void DestroyInstance()
	{
		if (ms_pInstance)
		{		
			delete ms_pInstance;
			ms_pInstance = 0;
		}
	}

private:
	static T *		ms_pInstance;
};


template< class T > T * Singleton<T>::ms_pInstance = 0;
}


#endif 
