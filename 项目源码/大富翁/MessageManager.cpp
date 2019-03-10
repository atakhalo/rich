#include "MessageManager.h"

namespace Rich {

	MessageManager* MessageManager::m_pMessageManager = nullptr;

	void MessageManager::Initialize(LPCSTR ipFileName, Rich::PlayerManager* playerManager, Rich::StreetManager* streetManager)
	{
		std::ifstream fin("ipAddr.txt");
		if (!fin)
		{
			MessageBox(nullptr, L"找不到文件ipAddr.txt！", L"错误", MB_OK);
			PostQuitMessage(0);
		}
		// 读取IP地址
		char ipAddr[100];
		fin >> ipAddr >> ipAddr;
		m_Socket = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (m_Socket == INVALID_SOCKET)
		{
			MessageBox(nullptr, L"找不到文件ipAddr.txt！", L"错误", MB_OK);
			PostQuitMessage(0);
		}
		// 套接字设置超时
		int nNetTimeout = 0;
		setsockopt(m_Socket, SOL_SOCKET, SO_RCVTIMEO, (char *)&nNetTimeout, sizeof(int));
		// 配置协议族和服务端IP地址
		m_ServAddr.sin_family = AF_INET;
		m_ServAddr.sin_port = htons(6238);
		m_ServAddr.sin_addr.S_un.S_addr = inet_addr(ipAddr);

		// 获取人物管理类指针
		if (!m_pPlayerManager && playerManager)
		{
			m_pPlayerManager = playerManager;
		}
		// 获取街区管理类指针
		if (!m_pStreetManager && streetManager)
		{
			m_pStreetManager = streetManager;
		}
		m_tUpdate = std::thread(&MessageManager::Update, this);
	}

	void MessageManager::HostConnect(int numOfPlayers, int startGold, int startSliver)
	{
		if (::connect(m_Socket, (sockaddr*)&m_ServAddr, sizeof(m_ServAddr)) == -1)
		{
			MessageBox(nullptr, L"连接失败！", L"错误", MB_OK);
			PostQuitMessage(0);
		}
		int buffer[16] = { 1, numOfPlayers, startGold, startSliver };
		::send(m_Socket, reinterpret_cast<const char *>(&buffer), sizeof(buffer), 0);
	}

	void MessageManager::PlayerConnect()
	{
		if (::connect(m_Socket, (sockaddr*)&m_ServAddr, sizeof(m_ServAddr)) == -1)
		{
			MessageBox(nullptr, L"连接失败！", L"错误", MB_OK);
			PostQuitMessage(0);
		}
		int buffer[16] = { 1 };
		::send(m_Socket, reinterpret_cast<const char *>(&buffer), sizeof(buffer), 0);
	}

	void MessageManager::Send(int *message)
	{
		::send(m_Socket, reinterpret_cast<const char *>(message), 64, 0);
	}

	void MessageManager::Update()
	{
		while (1)
		{
			memset(m_Buffer, 0, sizeof(m_Buffer));
			int nRecv = ::recv(m_Socket, reinterpret_cast<char *>(&m_Buffer), sizeof(m_Buffer), MSG_WAITALL);
			// 收到消息开始处理
			if (nRecv > 0)
			{
				m_Recived = true;
				// 如果收到加入房间的信息，初始化房间
				if (m_Buffer[0] == 1)
				{
					m_pPlayerManager->InitPlayer(m_Buffer[1], m_Buffer[2], m_Buffer[3], m_Buffer[4]);
					for (int i = 1; i <= m_Buffer[1]; i++)
					{
						POINT blockIndex = m_pPlayerManager->GetBlockIndex(i);
						XMFLOAT3 pos = m_pStreetManager->BlockTransFloat3Get(blockIndex);
						m_pPlayerManager->SetPlayerPosition(i, pos);

						POINT nextPosIndex = m_pStreetManager->BlockNextGet(
							m_pPlayerManager->GetBlockIndex(i),
							m_pPlayerManager->GetDirection(i));
						XMFLOAT3 nextPos = m_pStreetManager->BlockTransFloat3Get(nextPosIndex);
						m_pPlayerManager->SetTargetPosition(i, nextPos);
					}

				}
				// 如果收到游戏开始的信息，更新状态
				else if (m_Buffer[0] == 2)
				{
					m_State = 1;	// 游戏可以开始
				}
				// 如果是人物事件，调用对应函数  
				else if (m_Buffer[0] == 3)
				{
					// 0：角色移动
					if (m_Buffer[1] == 0)
						m_pPlayerManager->Move(m_Buffer[2]);
					// 1：角色转向
					else if (m_Buffer[1] == 1)
					{
						int revDir = -m_pPlayerManager->GetDirection();	// 拿到反方向
						POINT index = m_pStreetManager->BlockNextGet(m_pPlayerManager->GetBlockIndex(), revDir); // 拿到上一街区索引
						XMFLOAT3 pos = m_pStreetManager->BlockTransFloat3Get(index); // 该地块位置
						m_pPlayerManager->TurnBack(pos); // 终于转了方向吧...
					}
					// 2：设置路障
					else if (m_Buffer[1] == 2)
					{
						int revDir = -m_pPlayerManager->GetDirection();	// 拿到反方向
						POINT index = m_pStreetManager->BlockNextGet(m_pPlayerManager->GetBlockIndex(), revDir); // 拿到上一街区索引
						m_pStreetManager->BarrierSet(index, true);   // 应该是设置好路障了
					}
					// 3：获得/损失金钱
					else if (m_Buffer[1] == 3)
						m_pPlayerManager->AddGold(m_Buffer[2], m_Buffer[3]);
					// 4：丢失当前玩家的指定卡牌
					else if (m_Buffer[1] == 4)
						m_pPlayerManager->ThrowCard(m_Buffer[2], m_Buffer[3]);
					// 5：当前玩家获取一张卡
					else if (m_Buffer[1] == 5)
						m_pPlayerManager->ObtainCard(m_Buffer[2], static_cast<Rich::CardType>(m_Buffer[3]));
					// 6: 利率/损失利率
					else if (m_Buffer[1] == 6)
						m_pPlayerManager->AddRate(m_Buffer[2], m_Buffer[3]);
					// 7：变卖所有卡牌
					else if (m_Buffer[1] == 7)
						m_pPlayerManager->SellAllCard();
					// 8：获得/损失点券
					else if (m_Buffer[1] == 8)
						m_pPlayerManager->AddSilver(m_Buffer[2]);
					// 9：均贫卡
					else if (m_Buffer[1] == 9)
						m_pPlayerManager->AverageGold(m_Buffer[2]);
					// 10：抢劫利率
					else if (m_Buffer[1] == 10)
						m_pPlayerManager->RobInterests();
					// 11: 设置利息
					else if (m_Buffer[1] == 11)
						m_pPlayerManager->SetPlayerInterests(m_Buffer[2], m_Buffer[3]);
				}
				// 如果是地图事件，调用对应函数
				else if (m_Buffer[0] == 4)
				{
					// 0：拆当前整条街
					if (m_Buffer[1] == 0)
						m_pStreetManager->HutongDestroy(m_Buffer[2]);
					// 1：整条街修改地价因子
					else if (m_Buffer[1] == 1)
						m_pStreetManager->HutongValueChange(m_Buffer[2], m_Buffer[3]);
					// 2：整条街房屋等级改变
					else if (m_Buffer[1] == 2)
						m_pStreetManager->HutongLevelChange(m_Buffer[2], m_Buffer[3]);
					// 3：房屋等级重置
					else if (m_Buffer[1] == 3)
						m_pStreetManager->HouseLevelSet(POINT{ m_Buffer[2], m_Buffer[3] }, m_Buffer[4]);
					// 4：房屋等级改变
					else if (m_Buffer[1] == 4)
						m_pStreetManager->HouseLevelAdd(POINT{ m_Buffer[2], m_Buffer[3] }, m_Buffer[4]);
					// 5：房屋所有权改变
					else if (m_Buffer[1] == 5)
						m_pStreetManager->HouseOwnerSet(POINT{ m_Buffer[2], m_Buffer[3] }, m_Buffer[4]);
					// 6：房屋毁灭
					else if (m_Buffer[1] == 6)
						m_pStreetManager->HouseDestroy(POINT{ m_Buffer[2], m_Buffer[3] });
					// 7：房屋升满
					else if (m_Buffer[1] == 7)
						m_pStreetManager->HouseLevelMax(POINT{ m_Buffer[2], m_Buffer[3] });
					// 8：房屋类型改变
					else if (m_Buffer[1] == 8)
						m_pStreetManager->HouseTypeSet(POINT{ m_Buffer[2], m_Buffer[3] }, 
							static_cast<Rich::HOUSETYPE>(m_Buffer[4]));
				}
				else if (m_Buffer[0] == 5)
					m_pPlayerManager->SwitchPlayer();
			}
			else
				m_Recived = false;
		}
	}

	int MessageManager::GetState() const
	{
		return m_State;
	}

	void MessageManager::Release()
	{
		if (m_pMessageManager)
		{
			delete m_pMessageManager;
			m_pMessageManager = nullptr;
		}
	}

	MessageManager::MessageManager()
		: m_pPlayerManager(nullptr), m_pStreetManager(nullptr), m_State(0), m_Recived(0)
	{

	}

	MessageManager::~MessageManager()
	{
	}

	MessageManager * MessageManager::GetMessageManager()
	{
		if (!m_pMessageManager)
			m_pMessageManager = new MessageManager;
		return m_pMessageManager;
	}

}
