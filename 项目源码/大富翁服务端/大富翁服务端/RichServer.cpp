#include "RichServer.h"
RichServer *RichServer::m_pServer = nullptr;




RichServer::RichServer()
{
	memset(m_ResponseTime, 0, sizeof(m_ResponseTime));
	m_TotalEvent = 0;
	// 创建监听套节字
	SOCKET sListen = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	sockaddr_in sin;
	sin.sin_family = AF_INET;
	sin.sin_port = htons(6238);
	sin.sin_addr.S_un.S_addr = INADDR_ANY;
	if (::bind(sListen, (sockaddr*)&sin, sizeof(sin)) == SOCKET_ERROR)
	{
		MessageBoxA(NULL, "绑定失败", "错误", MB_OK);
		exit(EXIT_FAILURE);
	}
	::listen(sListen, 200);

	// 创建事件对象，并关联到新的套节字
	WSAEVENT event = ::WSACreateEvent();
	::WSAEventSelect(sListen, event, FD_ACCEPT | FD_CLOSE);
	// 添加到表中
	m_EventArray[m_TotalEvent] = event;
	m_SockArray[m_TotalEvent] = sListen;
	m_TotalEvent++;
}

RichServer::~RichServer()
{
	while (m_TotalEvent--)
		::closesocket(m_SockArray[m_TotalEvent]);
}

RichServer* RichServer::CreateRichServer()
{
	if (!m_pServer)
		m_pServer = new RichServer;
	return m_pServer;
}

void RichServer::CloseRichServer()
{
	if (m_pServer)
	{
		delete m_pServer;
		m_pServer = nullptr;
	}
}

void RichServer::run()
{
	while (TRUE)
	{
		//// 清理离线用户
		//for (int i = 1; i < m_TotalEvent; i++)
		//	if (m_ResponseTime[i] > 0 && clock() - m_ResponseTime[i] > 30000)
		//	{
		//		printf("一个玩家因为掉线而被T出\n");
		//		::closesocket(m_SockArray[i]);
		//      ::WSACloseEvent(m_EventArray[i]);
		//		for (int j = i; j < m_TotalEvent - 1; j++)
		//		{
		//			m_EventArray[j] = m_EventArray[j + 1];
		//			m_SockArray[j] = m_SockArray[j + 1];
		//			m_Addr[j] = m_Addr[j + 1];
		//			m_ResponseTime[j] = m_ResponseTime[j + 1];
		//		}
		//		m_TotalEvent--;
		//		break;
		//	}
		// 对所有事件只等待100ms（只要有一个触发）
		int nIndex = ::WSAWaitForMultipleEvents(m_TotalEvent, m_EventArray, FALSE, 100UL, FALSE);
		// 对每个事件调用WSAWaitForMultipleEvents函数，以便确定它的状态
		nIndex = nIndex - WSA_WAIT_EVENT_0;
		for (int i = nIndex; i<m_TotalEvent; i++)
		{
			// 检验该事件对象是否触发
			nIndex = ::WSAWaitForMultipleEvents(1, &m_EventArray[i], TRUE, 100UL, FALSE);
			if (nIndex == WSA_WAIT_FAILED || nIndex == WSA_WAIT_TIMEOUT)
			{
				continue;
			}
			else
			{
				// 获取到来的通知消息，WSAEnumNetworkEvents函数会自动重置受信事件
				WSANETWORKEVENTS netWorkEvent;
				::WSAEnumNetworkEvents(m_SockArray[i], m_EventArray[i], &netWorkEvent);
				if (netWorkEvent.lNetworkEvents & FD_ACCEPT)				// 处理FD_ACCEPT通知消息
				{
					if (netWorkEvent.iErrorCode[FD_ACCEPT_BIT] == 0)
					{
						if (m_TotalEvent > WSA_MAXIMUM_WAIT_EVENTS)
						{
							printf(" Too many connections! \n");
							continue;
						}
						int n = sizeof(sockaddr_in);
						SOCKET sNew = ::accept(m_SockArray[i], (sockaddr*)&m_Addr[m_TotalEvent], &n);
						WSAEVENT WSAevent = ::WSACreateEvent();
						::WSAEventSelect(sNew, WSAevent, FD_READ | FD_CLOSE | FD_WRITE);
						// 添加到表中
						m_EventArray[m_TotalEvent] = WSAevent;
						m_SockArray[m_TotalEvent] = sNew;
						m_ResponseTime[m_TotalEvent] = clock();

						// 如果是房主，接受创房间的信息（服务器只允许一个房主）
						while (::recv(m_SockArray[m_TotalEvent], reinterpret_cast<char *>(m_buffer), sizeof(m_buffer), 0) == -1)
							continue;
						if (m_buffer[0] == 1 && m_buffer[1] != 0)
						{
							if (m_TotalEvent == 1)
							{
								// 确定玩家数并创建ID数组
								m_Players = m_buffer[1];
								m_Golds = m_buffer[2];
								m_Slivers = m_buffer[3];
								m_LeftID.resize(m_Players);
								for (int i = 0; i < m_Players; ++i)
									m_LeftID[i] = i + 1;
							}
							else
							{
								// 第二个房主滚开！
								printf("这里不允许第二个房主，直接T出。\n");
								::closesocket(m_SockArray[m_TotalEvent]);
								::WSACloseEvent(m_EventArray[m_TotalEvent]);
								continue;
							}
						}




						// 分配ID给玩家，若房间满人，直接T出。
						if (m_LeftID.empty())
						{
							printf("房间满人，直接T出。\n");
							::closesocket(m_SockArray[m_TotalEvent]);
							::WSACloseEvent(m_EventArray[m_TotalEvent]);
							continue;
						}
						else
						{
							int pos = rand() % m_LeftID.size();
							memset(m_buffer, 0, sizeof(m_buffer));
							m_buffer[0] = 1;
							m_buffer[1] = m_Players;
							m_buffer[2] = m_Golds;
							m_buffer[3] = m_Slivers;
							m_buffer[4] = m_LeftID[pos];
							printf("新玩家加入，分配出ID号%d\n", m_LeftID[pos]);
							m_LeftID.erase(m_LeftID.begin() + pos, m_LeftID.begin() + pos + 1);
							::send(m_SockArray[m_TotalEvent], reinterpret_cast<char *>(m_buffer), 64, 0);
							// 分配完所有ID后
							if (m_LeftID.empty())
							{
								for (int i = 1; i <= m_Players; ++i)
								{
									int buffer[16] = { 2 };
									::send(m_SockArray[i], reinterpret_cast<char *>(buffer), 64, 0);
								}
							}
							
						}
						// 发送基本消息
						m_TotalEvent++;
					}
				}
				else if (netWorkEvent.lNetworkEvents & FD_READ)			// 处理FD_READ通知消息
				{
					if (netWorkEvent.iErrorCode[FD_READ_BIT] == 0)
					{
						memset(m_buffer, 0, sizeof(m_buffer));
						int nRecv = ::recv(m_SockArray[i], reinterpret_cast<char *>(m_buffer), 64, 0);
						if (nRecv > 0)
						{
							// 收到前4字节的int为3，4或5
							if (m_buffer[0] == 3 || m_buffer[0] == 4 || m_buffer[0] == 5)
							{
								// 给所有玩家发送消息
								for (int pos = 1; pos <= m_Players; ++pos)
									::send(m_SockArray[pos], reinterpret_cast<char *>(m_buffer), 64, 0);
							}
							// 更新响应时间
							m_ResponseTime[i] = clock();
						}

					}
				}
				else if (netWorkEvent.lNetworkEvents & FD_CLOSE)		// 处理FD_CLOSE通知消息
				{
					if (netWorkEvent.iErrorCode[FD_CLOSE_BIT] == 0)
					{
						printf("一个玩家关闭了游戏\n");
						::closesocket(m_SockArray[i]);
						::WSACloseEvent(m_EventArray[i]);
						for (int j = i; j < m_TotalEvent - 1; j++)
						{
							m_EventArray[j] = m_EventArray[j + 1];
							m_SockArray[j] = m_SockArray[j + 1];
							m_Addr[j] = m_Addr[j + 1];
							m_ResponseTime[j] = m_ResponseTime[j + 1];
						}
						m_TotalEvent--;
					}
				}
				else if (netWorkEvent.lNetworkEvents & FD_WRITE)		// 处理FD_WRITE通知消息
				{

				}

			}
		}


	}
}


