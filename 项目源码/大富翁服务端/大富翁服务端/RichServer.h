#pragma once
#include <Winsock2.h>
#include <cstdio>
#include <ctime>
#include <vector>
#include <queue>
#include <cstdlib>

class RichServer
{
private:
	SOCKET					m_sListen;			// �����׽���
	WSAEVENT				m_EventArray[24];	// WSA�¼�����
	SOCKET					m_SockArray[24];	// �ͻ����׽�������
	sockaddr_in 			m_Addr[24];			// �û���Ϣ��ʱ��
	clock_t					m_ResponseTime[24];	// ��Ӧʱ��
	int						m_TotalEvent;		// �¼�����
	static RichServer	   *m_pServer;
	int                     m_buffer[16];		// ��Ϣ���ͻ�����
	
	std::vector<int>        m_LeftID;			// ʣ��ɷ���ID
	int						m_Players;			// �����Ŀ
	int                     m_Golds;			// �����
	int						m_Slivers;			// ��ȯ��
	
	RichServer();
	~RichServer();
public:
	static RichServer* CreateRichServer();
	static void CloseRichServer();
	void run();
};