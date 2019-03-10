#include "MessageManager.h"

namespace Rich {

	MessageManager* MessageManager::m_pMessageManager = nullptr;

	void MessageManager::Initialize(LPCSTR ipFileName, Rich::PlayerManager* playerManager, Rich::StreetManager* streetManager)
	{
		std::ifstream fin("ipAddr.txt");
		if (!fin)
		{
			MessageBox(nullptr, L"�Ҳ����ļ�ipAddr.txt��", L"����", MB_OK);
			PostQuitMessage(0);
		}
		// ��ȡIP��ַ
		char ipAddr[100];
		fin >> ipAddr >> ipAddr;
		m_Socket = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (m_Socket == INVALID_SOCKET)
		{
			MessageBox(nullptr, L"�Ҳ����ļ�ipAddr.txt��", L"����", MB_OK);
			PostQuitMessage(0);
		}
		// �׽������ó�ʱ
		int nNetTimeout = 0;
		setsockopt(m_Socket, SOL_SOCKET, SO_RCVTIMEO, (char *)&nNetTimeout, sizeof(int));
		// ����Э����ͷ����IP��ַ
		m_ServAddr.sin_family = AF_INET;
		m_ServAddr.sin_port = htons(6238);
		m_ServAddr.sin_addr.S_un.S_addr = inet_addr(ipAddr);

		// ��ȡ���������ָ��
		if (!m_pPlayerManager && playerManager)
		{
			m_pPlayerManager = playerManager;
		}
		// ��ȡ����������ָ��
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
			MessageBox(nullptr, L"����ʧ�ܣ�", L"����", MB_OK);
			PostQuitMessage(0);
		}
		int buffer[16] = { 1, numOfPlayers, startGold, startSliver };
		::send(m_Socket, reinterpret_cast<const char *>(&buffer), sizeof(buffer), 0);
	}

	void MessageManager::PlayerConnect()
	{
		if (::connect(m_Socket, (sockaddr*)&m_ServAddr, sizeof(m_ServAddr)) == -1)
		{
			MessageBox(nullptr, L"����ʧ�ܣ�", L"����", MB_OK);
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
			// �յ���Ϣ��ʼ����
			if (nRecv > 0)
			{
				m_Recived = true;
				// ����յ����뷿�����Ϣ����ʼ������
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
				// ����յ���Ϸ��ʼ����Ϣ������״̬
				else if (m_Buffer[0] == 2)
				{
					m_State = 1;	// ��Ϸ���Կ�ʼ
				}
				// ����������¼������ö�Ӧ����  
				else if (m_Buffer[0] == 3)
				{
					// 0����ɫ�ƶ�
					if (m_Buffer[1] == 0)
						m_pPlayerManager->Move(m_Buffer[2]);
					// 1����ɫת��
					else if (m_Buffer[1] == 1)
					{
						int revDir = -m_pPlayerManager->GetDirection();	// �õ�������
						POINT index = m_pStreetManager->BlockNextGet(m_pPlayerManager->GetBlockIndex(), revDir); // �õ���һ��������
						XMFLOAT3 pos = m_pStreetManager->BlockTransFloat3Get(index); // �õؿ�λ��
						m_pPlayerManager->TurnBack(pos); // ����ת�˷����...
					}
					// 2������·��
					else if (m_Buffer[1] == 2)
					{
						int revDir = -m_pPlayerManager->GetDirection();	// �õ�������
						POINT index = m_pStreetManager->BlockNextGet(m_pPlayerManager->GetBlockIndex(), revDir); // �õ���һ��������
						m_pStreetManager->BarrierSet(index, true);   // Ӧ�������ú�·����
					}
					// 3�����/��ʧ��Ǯ
					else if (m_Buffer[1] == 3)
						m_pPlayerManager->AddGold(m_Buffer[2], m_Buffer[3]);
					// 4����ʧ��ǰ��ҵ�ָ������
					else if (m_Buffer[1] == 4)
						m_pPlayerManager->ThrowCard(m_Buffer[2], m_Buffer[3]);
					// 5����ǰ��һ�ȡһ�ſ�
					else if (m_Buffer[1] == 5)
						m_pPlayerManager->ObtainCard(m_Buffer[2], static_cast<Rich::CardType>(m_Buffer[3]));
					// 6: ����/��ʧ����
					else if (m_Buffer[1] == 6)
						m_pPlayerManager->AddRate(m_Buffer[2], m_Buffer[3]);
					// 7���������п���
					else if (m_Buffer[1] == 7)
						m_pPlayerManager->SellAllCard();
					// 8�����/��ʧ��ȯ
					else if (m_Buffer[1] == 8)
						m_pPlayerManager->AddSilver(m_Buffer[2]);
					// 9����ƶ��
					else if (m_Buffer[1] == 9)
						m_pPlayerManager->AverageGold(m_Buffer[2]);
					// 10����������
					else if (m_Buffer[1] == 10)
						m_pPlayerManager->RobInterests();
					// 11: ������Ϣ
					else if (m_Buffer[1] == 11)
						m_pPlayerManager->SetPlayerInterests(m_Buffer[2], m_Buffer[3]);
				}
				// ����ǵ�ͼ�¼������ö�Ӧ����
				else if (m_Buffer[0] == 4)
				{
					// 0����ǰ������
					if (m_Buffer[1] == 0)
						m_pStreetManager->HutongDestroy(m_Buffer[2]);
					// 1���������޸ĵؼ�����
					else if (m_Buffer[1] == 1)
						m_pStreetManager->HutongValueChange(m_Buffer[2], m_Buffer[3]);
					// 2�������ַ��ݵȼ��ı�
					else if (m_Buffer[1] == 2)
						m_pStreetManager->HutongLevelChange(m_Buffer[2], m_Buffer[3]);
					// 3�����ݵȼ�����
					else if (m_Buffer[1] == 3)
						m_pStreetManager->HouseLevelSet(POINT{ m_Buffer[2], m_Buffer[3] }, m_Buffer[4]);
					// 4�����ݵȼ��ı�
					else if (m_Buffer[1] == 4)
						m_pStreetManager->HouseLevelAdd(POINT{ m_Buffer[2], m_Buffer[3] }, m_Buffer[4]);
					// 5����������Ȩ�ı�
					else if (m_Buffer[1] == 5)
						m_pStreetManager->HouseOwnerSet(POINT{ m_Buffer[2], m_Buffer[3] }, m_Buffer[4]);
					// 6�����ݻ���
					else if (m_Buffer[1] == 6)
						m_pStreetManager->HouseDestroy(POINT{ m_Buffer[2], m_Buffer[3] });
					// 7����������
					else if (m_Buffer[1] == 7)
						m_pStreetManager->HouseLevelMax(POINT{ m_Buffer[2], m_Buffer[3] });
					// 8���������͸ı�
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
