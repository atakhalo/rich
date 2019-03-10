#ifndef _INITSOCK_H_
#define _INITSOCK_H_
#include <Winsock2.h>
#pragma comment(lib, "WS2_32")	// 链接到WS2_32.lib
class Initsock
{
private:
	static Initsock * m_pInitsock;

	Initsock()
	{
		// 初始化WS2_32.dll
		WSADATA wsaData;
		WORD sockVersion = MAKEWORD(2, 2);
		if (::WSAStartup(sockVersion, &wsaData) != 0)
		{
			MessageBox(NULL, L"Winsock初始化失败！", L"错误", MB_OK);
			exit(0);
		}
	}
	~Initsock()
	{
		::WSACleanup();
	}

public:
	static Initsock * GetInitsock();
	static void Release();
	
};

#endif