#ifndef _PUSH_SERVER_H_
#define _PUSH_SERVER_H_


class PushServer
{
public:
	PushServer();
	~PushServer();

public:
	bool InitServer();
	void RunServer();
	void Release();
	
private:
};

#endif

