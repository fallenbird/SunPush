#include "../CommonHeader.h"
#include "MessageEncrypt.h"

MessageEncrypt::MessageEncrypt()
{
	m_nPos1 = 0;
	m_nPos2 = 0;
	InitKey(KEY_A1, KEY_B1, KEY_C1, KEY_FST1, KEY_A2, KEY_B2, KEY_C2, KEY_FST2);
}

MessageEncrypt::~MessageEncrypt()
{
}

void MessageEncrypt::InitKey(UINT8 a1, UINT8 b1, UINT8 c1, UINT8 fst1, UINT8 a2, UINT8 b2, UINT8 c2, UINT8 fst2)
{
	unsigned char	nCode = fst1;
	int i = 0;
	for(i = 0; i < 256; i++)
	{
		m_bufEncrypt1[i] = nCode;
		nCode = (a1*nCode*nCode + b1*nCode + c1) % 256;
	}

	nCode = fst2;
	for( i = 0; i < 256; i++)
	{
		m_bufEncrypt2[i] = nCode;
		nCode = (a2*nCode*nCode + b2*nCode + c2) % 256;
	}

	push_stack1();
	push_stack2();
	push_stack3();

}

UINT8 MessageEncrypt::rotate_left_char(unsigned char nData, unsigned long nCount)
{
	nCount %= 8;
	unsigned long lData = nData << nCount;
	unsigned long lData2 = nData >> (8-nCount);
	return static_cast<unsigned char>((lData2 | lData) & 0xff);
}


UINT8 MessageEncrypt::rotate_right_char(unsigned char nData, unsigned long nCount)
{
	nCount %= 8;
	unsigned lData = nData >> nCount;
	unsigned lData2 = nData << (8-nCount);
	return static_cast<unsigned char>((lData | lData2) & 0xff);
}


void MessageEncrypt::Encrypt(unsigned char * bufMsg, int nLen, bool bMove, int resv,int resv2,int resv3,int resv4,int resv5,int resv6,int resv7)
{
	try{
		int		nOldPos1 = m_nPos1;
		int		nOldPos2 = m_nPos2;
		for(int i = 0; i < nLen; i++)
		{
			bufMsg[i] ^= m_bufEncrypt1[m_nPos1];
			bufMsg[i] ^= m_bufEncrypt1[m_nPos1] * 0x33f;
			bufMsg[i] ^= m_bufEncrypt2[m_nPos2];
			bufMsg[i] ^= m_bufEncrypt1[m_nPos1] * m_bufEncrypt2[m_nPos2];
			bufMsg[i] ^= m_bufEncrypt1[m_nPos1] * m_bufEncrypt2[m_nPos2] / 0x23df;
			bufMsg[i] ^= m_bufEncrypt1[m_nPos1] + m_bufEncrypt2[m_nPos2] +0x34fee;
			bufMsg[i] ^= m_nPos1;
			bufMsg[i] ^= m_nPos2;	
			if( m_nPos2 & 0x31  )
			{
				bufMsg[i] ^= m_bufEncrypt1[m_nPos1] * 0x3423f;
				bufMsg[i] ^= m_nPos1*2344;
				bufMsg[i] ^= m_bufEncrypt1[m_nPos1] * m_bufEncrypt2[m_nPos2];
				bufMsg[i] ^= m_bufEncrypt1[m_nPos1] * m_bufEncrypt2[m_nPos2] / 0x233fedf * m_nPos2;
				bufMsg[i] ^= m_bufEncrypt1[m_nPos1] + m_bufEncrypt2[m_nPos2] +0x343fee;
				bufMsg[i] ^= m_nPos1;
				bufMsg[i] ^= m_nPos2;	
				//bufMsg[i] = rotate_right_char(bufMsg[i],m_nPos1);
			}

			if( m_nPos1 & 0x3  )
			{
				bufMsg[i] ^= m_bufEncrypt1[m_nPos1] * 0x3422f;
				bufMsg[i] ^= m_nPos1+m_nPos2+0x234a;
				bufMsg[i] ^= m_bufEncrypt1[m_nPos1] * m_bufEncrypt2[m_nPos2]*2;
				bufMsg[i] ^= m_bufEncrypt1[m_nPos1] * m_bufEncrypt2[m_nPos2] / 0x233fedf * m_nPos2;
				bufMsg[i] ^= m_bufEncrypt1[m_nPos1] + m_bufEncrypt2[m_nPos2] + 0x343fee;
				bufMsg[i] ^= m_nPos2;	
				//bufMsg[i] = rotate_left_char(bufMsg[i],m_nPos2);
			}

			if( (m_nPos1 & 0x13) == 0 )
			{
				//bufMsg[i] = rotate_left_char(bufMsg[i],m_bufEncrypt1[m_nPos1]);
				bufMsg[i] ^= m_bufEncrypt1[m_nPos1] * 0x3422f;
				bufMsg[i] ^= m_nPos1+33;
				bufMsg[i] ^= m_bufEncrypt1[m_nPos1] * m_bufEncrypt2[m_nPos2]*2;
				bufMsg[i] ^= m_bufEncrypt1[m_nPos1] * m_bufEncrypt2[m_nPos2] / 0x233fedf * m_nPos2;
				bufMsg[i] ^= m_bufEncrypt1[m_nPos1] + m_bufEncrypt2[m_nPos2] + 0x343fee;
				bufMsg[i] ^= m_nPos2;	
			}
			if( m_nPos1 * m_bufEncrypt2[m_nPos2] % 0x9 == 1 )
			{
				bufMsg[i] ^= m_bufEncrypt1[m_nPos1] * 0x342f;
				bufMsg[i] ^= m_nPos1+31;
				bufMsg[i] ^= m_bufEncrypt1[m_nPos1] * m_bufEncrypt2[m_nPos2]*2;
				bufMsg[i] ^= m_bufEncrypt1[m_nPos1] * m_bufEncrypt2[m_nPos2] / 0x233fedf * m_nPos1;
				bufMsg[i] ^= m_bufEncrypt1[m_nPos1] + m_bufEncrypt2[m_nPos2] + 0x343fee;
				bufMsg[i] ^= m_nPos2;	
				//bufMsg[i] = rotate_right_char(bufMsg[i],m_nPos1*3);

			}

			//bufMsg[i] = rotate_right_char(bufMsg[i],m_bufEncrypt1[m_nPos1] + m_bufEncrypt2[m_nPos2] );
			bufMsg[i] = rotate_left_char(bufMsg[i],m_bufEncrypt1[m_nPos1] + m_bufEncrypt2[m_nPos2] +0x34fee);

			if(++m_nPos1 >= 256)
			{
				m_nPos1 = 0;
				if(++m_nPos2 >= 256)
					m_nPos2 = 0;
			}
			assert(m_nPos1 >=0 && m_nPos1 < 256);
			assert(m_nPos2 >=0 && m_nPos2 < 256);
		}

		if(!bMove)
		{
			m_nPos1 = nOldPos1;
			m_nPos2 = nOldPos2;
		}
	}catch(...){ 
		//LOGCATCH("Encrypt fail."); 
	}
}

void MessageEncrypt::Decrypt(unsigned char * bufMsg, int nLen, bool bMove,int resv,int resv2,int resv3,int resv4,int resv5,int resv6,int resv7)
{
	try{
		int		nOldPos1 = m_nPos1;
		int		nOldPos2 = m_nPos2;
		for(int i = 0; i < nLen; i++)
		{
			bufMsg[i] = rotate_right_char(bufMsg[i],m_bufEncrypt1[m_nPos1] + m_bufEncrypt2[m_nPos2] +0x34fee);
			bufMsg[i] ^= m_bufEncrypt1[m_nPos1];
			bufMsg[i] ^= m_bufEncrypt1[m_nPos1] * 0x33f;
			bufMsg[i] ^= m_bufEncrypt2[m_nPos2];
			bufMsg[i] ^= m_bufEncrypt1[m_nPos1] * m_bufEncrypt2[m_nPos2];
			bufMsg[i] ^= m_bufEncrypt1[m_nPos1] * m_bufEncrypt2[m_nPos2] / 0x23df;
			bufMsg[i] ^= m_bufEncrypt1[m_nPos1] + m_bufEncrypt2[m_nPos2] +0x34fee;
			bufMsg[i] ^= m_nPos1;
			bufMsg[i] ^= m_nPos2;	
			if( m_nPos2 & 0x31  )
			{
				bufMsg[i] ^= m_bufEncrypt1[m_nPos1] * 0x3423f;
				bufMsg[i] ^= m_nPos1*2344;
				bufMsg[i] ^= m_bufEncrypt1[m_nPos1] * m_bufEncrypt2[m_nPos2];
				bufMsg[i] ^= m_bufEncrypt1[m_nPos1] * m_bufEncrypt2[m_nPos2] / 0x233fedf * m_nPos2;
				bufMsg[i] ^= m_bufEncrypt1[m_nPos1] + m_bufEncrypt2[m_nPos2] +0x343fee;
				bufMsg[i] ^= m_nPos1;
				bufMsg[i] ^= m_nPos2;	
				//bufMsg[i] = rotate_left_char(bufMsg[i],m_nPos1);
			}

			if( m_nPos1 & 0x3  )
			{
				bufMsg[i] ^= m_bufEncrypt1[m_nPos1] * 0x3422f;
				bufMsg[i] ^= m_nPos1+m_nPos2+0x234a;
				bufMsg[i] ^= m_bufEncrypt1[m_nPos1] * m_bufEncrypt2[m_nPos2]*2;
				bufMsg[i] ^= m_bufEncrypt1[m_nPos1] * m_bufEncrypt2[m_nPos2] / 0x233fedf * m_nPos2;
				bufMsg[i] ^= m_bufEncrypt1[m_nPos1] + m_bufEncrypt2[m_nPos2] + 0x343fee;
				bufMsg[i] ^= m_nPos2;	
				//bufMsg[i] = rotate_right_char(bufMsg[i],m_nPos2);
			}

			if( (m_nPos1 & 0x13) == 0 )
			{
				//bufMsg[i] = rotate_right_char(bufMsg[i],m_bufEncrypt1[m_nPos1]);
				bufMsg[i] ^= m_bufEncrypt1[m_nPos1] * 0x3422f;
				bufMsg[i] ^= m_nPos1+33;
				bufMsg[i] ^= m_bufEncrypt1[m_nPos1] * m_bufEncrypt2[m_nPos2]*2;
				bufMsg[i] ^= m_bufEncrypt1[m_nPos1] * m_bufEncrypt2[m_nPos2] / 0x233fedf * m_nPos2;
				bufMsg[i] ^= m_bufEncrypt1[m_nPos1] + m_bufEncrypt2[m_nPos2] + 0x343fee;
				bufMsg[i] ^= m_nPos2;	
			}
			if( m_nPos1 * m_bufEncrypt2[m_nPos2] % 0x9 == 1 )
			{
				bufMsg[i] ^= m_bufEncrypt1[m_nPos1] * 0x342f;
				bufMsg[i] ^= m_nPos1+31;
				bufMsg[i] ^= m_bufEncrypt1[m_nPos1] * m_bufEncrypt2[m_nPos2]*2;
				bufMsg[i] ^= m_bufEncrypt1[m_nPos1] * m_bufEncrypt2[m_nPos2] / 0x233fedf * m_nPos1;
				bufMsg[i] ^= m_bufEncrypt1[m_nPos1] + m_bufEncrypt2[m_nPos2] + 0x343fee;
				bufMsg[i] ^= m_nPos2;	
				//bufMsg[i] = rotate_left_char(bufMsg[i],m_nPos1*3);

			}

			//bufMsg[i] = rotate_left_char(bufMsg[i],m_bufEncrypt1[m_nPos1] + m_bufEncrypt2[m_nPos2] );

			if(++m_nPos1 >= 256)
			{
				m_nPos1 = 0;
				if(++m_nPos2 >= 256)
					m_nPos2 = 0;
			}
			assert(m_nPos1 >=0 && m_nPos1 < 256);
			assert(m_nPos2 >=0 && m_nPos2 < 256);
		}

		if(!bMove)
		{
			m_nPos1 = nOldPos1;
			m_nPos2 = nOldPos2;
		}
	}catch(...){ 
		//LOGCATCH("Encrypt fail."); 
	}

}


void MessageEncrypt::push_stack1()
{
	for(int i = 0; i < 10; i++ )
	{
		m_bufEncrypt1[i*4+1] ^= m_bufEncrypt2[i*4+3];
	}
}
void MessageEncrypt::push_stack2()
{
	for(int i = 0; i < 5; i++ )
	{
		m_bufEncrypt1[i*4 + 50] ^= m_bufEncrypt2[i*4+2+50];
	}
}
void MessageEncrypt::push_stack3()
{
	for(int i = 0; i < 5; i++ )
	{
		m_bufEncrypt1[i*4 + 50] ^= m_bufEncrypt2[i*4+2+50];
	}
}

void MessageEncrypt::pop_stack1()
{
	push_stack1();
}
void MessageEncrypt::pop_stack2()
{
	push_stack2();
}

void MessageEncrypt::pop_stack3()
{
	push_stack3();
}