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
	SOCKET					m_sListen;			// 监听套接字
	WSAEVENT				m_EventArray[24];	// WSA事件数组
	SOCKET					m_SockArray[24];	// 客户端套接字数组
	sockaddr_in 			m_Addr[24];			// 用户信息和时间
	clock_t					m_ResponseTime[24];	// 响应时间
	int						m_TotalEvent;		// 事件总数
	static RichServer	   *m_pServer;
	int                     m_buffer[16];		// 消息发送缓冲区
	
	std::vector<int>        m_LeftID;			// 剩余可分配ID
	int						m_Players;			// 玩家数目
	int                     m_Golds;			// 金币数
	int						m_Slivers;			// 点券数
	
	RichServer();
	~RichServer();
public:
	static RichServer* CreateRichServer();
	static void CloseRichServer();
	void run();
};