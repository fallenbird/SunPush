#pragma once

const int KEY_A1 = 0x5a;
const int KEY_B1 = 0X24;
const int KEY_C1 = 0X11;
const int KEY_FST1 = 0Xfe;
const int KEY_A2 = 0Xa1;
const int KEY_B2 = 0X88;
const int KEY_C2 = 0X12;
const int KEY_FST2 = 0Xfc;


class MessageEncrypt
{
public:
	MessageEncrypt();
	virtual ~MessageEncrypt();

	void	Encrypt(unsigned char * bufMsg, int nLen, bool bMove = true,
		int resv = 4422,int resv2 = 2244,int resv3 = 42,int resv4 = 4422,int resv5 = 4452,int resv6 = 42,int resv7 = 0xff342);
	void	Decrypt(unsigned char * bufMsg, int nLen, bool bMove = true,
		int resv = 4422,int resv2 = 2244,int resv3 = 42,int resv4 = 4422,int resv5 = 4452,int resv6 = 42,int resv7 = 0xff342);

protected:

	void	InitKey(unsigned char a1, unsigned char b1, unsigned char c1, unsigned char fst1, unsigned char a2, unsigned char b2, unsigned char c2, unsigned char fst2);
	unsigned char	rotate_left_char(unsigned char nData, unsigned long nCount);
	unsigned char	rotate_right_char(unsigned char nData, unsigned long nCount);

	void inline push_stack1();
	void inline push_stack2();
	void inline push_stack3();

	void inline pop_stack1();
	void inline pop_stack2();
	void inline pop_stack3();

	int		m_nPos1;
	int		m_nPos2;
	unsigned char m_bufEncrypt1[256];
	unsigned char m_bufEncrypt2[256];

};
