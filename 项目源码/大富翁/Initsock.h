#ifndef _INITSOCK_H_
#define _INITSOCK_H_
#include <Winsock2.h>
#pragma comment(lib, "WS2_32")	// ���ӵ�WS2_32.lib
class Initsock
{
private:
	static Initsock * m_pInitsock;

	Initsock()
	{
		// ��ʼ��WS2_32.dll
		WSADATA wsaData;
		WORD sockVersion = MAKEWORD(2, 2);
		if (::WSAStartup(sockVersion, &wsaData) != 0)
		{
			MessageBox(NULL, L"Winsock��ʼ��ʧ�ܣ�", L"����", MB_OK);
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