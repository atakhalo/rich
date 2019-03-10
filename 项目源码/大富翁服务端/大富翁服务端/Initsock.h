#pragma once
#include <WinSock2.h>
#pragma comment(lib, "WS2_32")	// 链接到WS2_32.lib
class InitSock
{
private:
	static InitSock *initSock;

	InitSock()
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
	~InitSock()
	{
		::WSACleanup();
	}

public:
	static InitSock * CreateInitSock()
	{
		if (!initSock)
			initSock = new InitSock;
		return initSock;
	}

	static void CloseInitSock()
	{
		if (initSock)
			delete initSock;
		initSock = nullptr;
	}
};
