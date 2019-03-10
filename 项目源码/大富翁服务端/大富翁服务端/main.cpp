#include "RichServer.h"
#include "Initsock.h"
void printIP();

int main()
{
	srand((unsigned)time(0));

	InitSock * initSock = InitSock::CreateInitSock();
	RichServer * pServer = RichServer::CreateRichServer();
	printIP();
	pServer->run();
	pServer->CloseRichServer();
	initSock->CloseInitSock();
}

void printIP()
{
	char szHost[256];
	// ȡ�ñ�����������
	::gethostname(szHost, 256);
	// ͨ���������õ���ַ��Ϣ
	hostent *pHost = ::gethostbyname(szHost);
	// ��ӡ������IP��ַ
	in_addr addr;
	for (int i = 0; ; i++)
	{
		char *p = pHost->h_addr_list[i];
		if (p == NULL)
			break;

		memcpy(&addr.S_un.S_addr, p, pHost->h_length);
		char *szIp = ::inet_ntoa(addr);
		printf("IP��ַ��%s \n", szIp);
	}
}


