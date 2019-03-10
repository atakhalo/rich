//***************************************************************************************
//	���֣�			������������� * Դ�ļ�
//	���ߣ�			¬��͢
//  �������ڣ�		2017-8-24
//  ����������ڣ�	2017-8-26
//***************************************************************************************

#include "PlayerManager.h"

namespace Rich
{
	PLAYER::PLAYER() :name(std::string("(None)")), state(), ID(PLAYER_ID_DEFAULT),
		gold(0), silver(0), posBefore(XMFLOAT3(0, 0, 0)), posNow(XMFLOAT3(0, 0, 0)),
		posAfter(XMFLOAT3(0, 0, 0)), dir(0), leftStep(0), moneyRate(0.0f),
		interests(0.0f)
	{
		blockIndex.x = 0; blockIndex.y = 0;
		cards.clear();
	}

	/*---------------------------------------------------------------------------------*/

	//  ��������ṹ����Ϣ
	void PlayerManager::Update(float dt)
	{
		//  ������������һ֡
		for (PLAYER player : m_Players)
		{
			Moving(player.ID, dt);
		}

		//  ����������������
		std::vector<XMFLOAT4X4> matrix;
		XMFLOAT4X4 M;	
		for (PLAYER player : m_Players)
		{
			matrix.clear();
			M = CaculateWorldMatrix(player.ID);
			matrix.push_back(M);
			m_SPlayer[player.name].UpdateWorldMatrixes(matrix);
		}
	}

	//  ��������
	void PlayerManager::Draw()
	{
		for (PLAYER player : m_Players)
		{
			m_SPlayer[player.name].DrawInstances();
		}
	}

	//  ��ʼ����Ϸ���
	void PlayerManager::InitPlayer(int playerNum, int gold, int silver, int myID)
	{
		//  ������Ӧ����������
		std::string name[4]{"����","����","ˮ��","ԭ��"};

		//  ��Դ����
		std::string resourceName[4]{ "red","yellow","blue","green" };
		int ID[4] = { PLAYER_ID_ONE,PLAYER_ID_TWO ,PLAYER_ID_THREE ,PLAYER_ID_FOUR };
		POINT blockIndex[4] = {
			{1,2},
			{5,4},
			{14,1},
			{19,2}
		};
		PLAYER newPlayer;

		for (int i = 0; i < playerNum; ++i)
		{
			newPlayer.ID = ID[i];
			newPlayer.name = name[i];
			newPlayer.gold = gold;
			newPlayer.silver = silver;
			newPlayer.dir = 1;
			newPlayer.blockIndex = blockIndex[i];

			//  �������С�������ʼ��Ƭ
			//newPlayer.cards.clear();
			//newPlayer.cards.push_back(CardType::CARD_STAY);

			m_Players.push_back(newPlayer);

			std::string obj = std::string("Object\\") + resourceName[i] + std::string(".obj");
			std::string png = std::string("Texture\\") + resourceName[i] + std::string(".png");
			std::string mtl = std::string("Material\\") + resourceName[i] + std::string(".mtl");

			m_SPlayer[name[i]].InitModel(obj.c_str(), png.c_str(), mtl.c_str());
		}

		//  ��¼�ҵ�ID��
		m_MyPlayerID = myID;
	}

	//  �л�����ٿ�Ȩ
	void PlayerManager::SwitchPlayer()
	{
		int index = m_CurrPlyaerIndex;
		index = (index + 1) % m_Players.size();
		m_CurrPlyaerIndex = index;
	}

	//  ���㵱ǰ���������Ϣ����ϼ���
	void PlayerManager::CalculateCurrPlayerInterests()
	{
		int i = m_CurrPlyaerIndex;
		int gold = m_Players[i].gold;
		int rate = m_Players[i].interests;

		//  ������Ϣֵ = ��ǰ�ܽ�Ǯ * ��ǰ����
		int interests = gold * rate;

		m_Players[i].interests += interests;
	}

	//  ��ǰ�����������ȡǮ������Ϣת��Ϊ��Ǯ����ϼ���
	void PlayerManager::ChangeInterestsToMoney()
	{
		//  �����Ϣ
		int interests = GetPlayerInterests();
		SetPlayerInterests(0);

		//  ���ӽ�Ǯ
		AddGold(interests);
	}

	//  ��ָͣ�������ƶ�
	void PlayerManager::StopPlayer(int ID)
	{
		int i = FindPlayerByID(ID);
		m_Players[i].leftStep = 0;
	}

	//  ���õ�ǰ���λ��
	void PlayerManager::SetPlayerPosition(XMFLOAT3 pos)
	{
		int i = m_CurrPlyaerIndex;

		m_Players[i].posBefore = pos;
		m_Players[i].posNow = pos;
	}

	//  ���õ�ǰ���������Ϣ
	void PlayerManager::SetPlayerInterests(int interests)
	{
		int i = m_CurrPlyaerIndex;
		m_Players[i].interests = interests;
	}

	//  ����ָ�����λ��
	void PlayerManager::SetPlayerPosition(int ID, XMFLOAT3 pos)
	{
		int i = FindPlayerByID(ID);
		m_Players[i].posBefore = pos;
		m_Players[i].posNow = pos;
	}

	//  ����ָ����ҵ�������Ϣ
	void PlayerManager::SetPlayerInterests(int ID, int interests)
	{
		int i = FindPlayerByID(ID);
		m_Players[i].interests = interests;
	}

	//  �ı䵱ǰ�����Ǯ
	void PlayerManager::AddGold(int money)
	{
		int i = m_CurrPlyaerIndex;
		m_Players[i].gold += money;
	}

	//  �ı�ָ�������Ǯ
	void PlayerManager::AddGold(int ID, int money)
	{
		int i = FindPlayerByID(ID);
		m_Players[i].gold += money;
	}

	//  �ı䵱ǰ�����ȯ
	void PlayerManager::AddSilver(int silver)
	{
		int i = m_CurrPlyaerIndex;
		m_Players[i].silver += silver;
	}

	//  �ı�ָ�������ȯ
	void PlayerManager::AddSilver(int ID, int silver)
	{
		int i = FindPlayerByID(ID);
		m_Players[i].silver += silver;
	}

	//  ���õ�ǰ�����·����
	void PlayerManager::SetBlockIndex(POINT point)
	{
		int i = m_CurrPlyaerIndex;
		m_Players[i].blockIndex = point;
	}

	//  ����ָ�������·����
	void PlayerManager::SetBlockIndex(int ID, POINT point)
	{
		int i = FindPlayerByID(ID);
		m_Players[i].blockIndex = point;
	}

	//  ��ǰ����ת��
	void PlayerManager::TurnBack(XMFLOAT3 backStep)
	{
		int i = m_CurrPlyaerIndex;

		//  ���߷���ı�
		m_Players[i].dir *= -1;
		//  Ŀ��ı�
		m_Players[i].posAfter = backStep;
		//  ����ı�
		XMVECTOR before = XMLoadFloat3(&m_Players[i].posBefore);
		XMVECTOR after = XMLoadFloat3(&m_Players[i].posAfter);
		XMVECTOR dirMove = after - before;
		XMStoreFloat3(&m_Players[i].dirMove, dirMove);
	}
	
	//  ָ������ת��
	void PlayerManager::TurnBack(int ID, XMFLOAT3 backStep)
	{
		int i = FindPlayerByID(ID);

		//  ���߷���ı�
		m_Players[i].dir *= -1;
		//  Ŀ��ı�
		m_Players[i].posAfter = backStep;
		//  ����ı�
		XMVECTOR before = XMLoadFloat3(&m_Players[i].posBefore);
		XMVECTOR after = XMLoadFloat3(&m_Players[i].posAfter);
		XMVECTOR dirMove = after - before;
		XMStoreFloat3(&m_Players[i].dirMove, dirMove);
	}

	//  �ı䵱ǰ��������(rateΪǧ�ֱȣ�1 ��ʾ 0.1%)[����������0 - 2%]
	void PlayerManager::AddRate(int rate)
	{
		int i = m_CurrPlyaerIndex;
		float nowRate = m_Players[i].moneyRate;

		nowRate = MathHelper::Clamp(nowRate + rate * 0.001f, 0.0f, 2.0f);
		m_Players[i].moneyRate = nowRate;
	}

	//  �ı�ָ����������(rateΪǧ�ֱȣ�1 ��ʾ 0.1%)[����������0 - 2%]
	void PlayerManager::AddRate(int ID, int rate)
	{
		int i = FindPlayerByID(ID);

		float nowRate = m_Players[i].moneyRate;

		nowRate = MathHelper::Clamp(nowRate + rate * 0.001f, 0.0f, 2.0f);
		m_Players[i].moneyRate = nowRate;
	}

	//  ָ�����ﶪ���ض��Ŀ�
	CardType PlayerManager::ThrowCard(int ID, int cardIndex)
	{
		int i = FindPlayerByID(ID);

		//  û�п���
		if (m_Players[i].cards.size() <= 0)
			return CardType::CARD_NONE;

		//  ����������Χ
		if(cardIndex < 0 || cardIndex >= m_Players[i].cards.size())
			return CardType::CARD_NONE;

		//  ����һ�ſ���
		CardType card = m_Players[i].cards[cardIndex];
		m_Players[i].cards.erase(m_Players[i].cards.begin() + cardIndex);
		return card;
	}

	//  ��ǰ���ﶪ����һ�ſ�
	CardType PlayerManager::ThrowCard()
	{
		int i = m_CurrPlyaerIndex;

		//  û�п���
		if(m_Players[i].cards.size() <= 0)
			return CardType::CARD_NONE;

		//  ����һ�ſ���
		CardType card = m_Players[i].cards[0];
		m_Players[i].cards.erase(m_Players[i].cards.begin());
		return card;
	}

	//  ָ�����ﶪ����һ�ſ�
	CardType PlayerManager::ThrowCard(int ID)
	{
		int i = FindPlayerByID(ID);

		//  û�п���
		if (m_Players[i].cards.size() <= 0)
			return CardType::CARD_NONE;

		//  ����һ�ſ���
		CardType card = m_Players[i].cards[0];
		m_Players[i].cards.erase(m_Players[i].cards.begin());
		return card;
	}

	//  ��ǰ������һ�ſ�
	void PlayerManager::ObtainCard(CardType card)
	{
		int i = m_CurrPlyaerIndex;

		//  ������Ƭ��ֱ���������� <= ��������-1
		while(m_Players[i].cards.size() > PLAYER_CARD_MAX - 1)
			ThrowCard();

		//  ��ӿ�Ƭ
		m_Players[i].cards.push_back(card);
	}

	//  ��ȡ��ǰ�������ID
	std::vector<int> PlayerManager::GetAlivePlayersID() const
	{
		std::vector<int> IDs;
		for (PLAYER player : m_Players)
		{
			IDs.push_back(player.ID);
		}
		return IDs;
	}

	//  ָ��������һ�ſ�
	void PlayerManager::ObtainCard(int ID, CardType card)
	{
		int i = FindPlayerByID(ID);

		//  ������Ƭ��ֱ���������� <= ��������-1
		while (m_Players[i].cards.size() > PLAYER_CARD_MAX - 1)
			ThrowCard(ID);

		//  ��ӿ�Ƭ
		m_Players[i].cards.push_back(card);
	}

	//  ��ȡ��ǰ����Ľֵ�����
	POINT PlayerManager::GetBlockIndex() const
	{
		int i = m_CurrPlyaerIndex;
		return m_Players[i].blockIndex;
	}

	//  ��ȡ��ǰ�����ID
	int PlayerManager::GetCurrPlayerID() const
	{
		int i = m_CurrPlyaerIndex;
		return m_Players[i].ID;
	}

	//  ��ȡָ������ĵ�·����
	POINT PlayerManager::GetBlockIndex(int ID) const
	{
		int i = FindPlayerByID(ID);
		return m_Players[i].blockIndex;
	}

	//  ��ȡ��ǰ������ƶ�����(������)
	int PlayerManager::GetDirection() const
	{
		int i = m_CurrPlyaerIndex;
		return m_Players[i].dir;
	}

	//  ��ȡָ��������ƶ�������������
	int PlayerManager::GetDirection(int ID) const
	{
		int i = FindPlayerByID(ID);
		return m_Players[i].dir;
	}

	//  ��ȡ��ǰ�������ʵλ��
	XMFLOAT3 PlayerManager::GetPlayerRealPosition() const
	{
		int i = m_CurrPlyaerIndex;
		return m_Players[i].posNow;
	}

	//  ��ȡ��ǰ�����������Ϣ
	int PlayerManager::GetPlayerInterests() const
	{
		int i = m_CurrPlyaerIndex;
		return m_Players[i].interests;
	}

	//  ��ȡ��ǰ����ӵ�еĿ�Ƭ��ö�٣�
	const std::vector<CardType>& PlayerManager::GetPlayerCardType() const
	{
		int i = m_CurrPlyaerIndex;
		return m_Players[i].cards;
	}

	//  ��ȡ�ͻ�����ҵ�ID��
	int PlayerManager::GetMyPlayerID() const
	{
		return m_MyPlayerID;
	}

	//  ��ȡ�ͻ�����ҵĽ�Ǯ
	int PlayerManager::GetMyPlayerGold() const
	{
		int i = FindPlayerByID(m_MyPlayerID);
		return m_Players[i].gold;
	}

	//  ��ȡ�ͻ�����ҵĵ�ȯ
	int PlayerManager::GetMyPlayerSilver() const
	{
		int i = FindPlayerByID(m_MyPlayerID);
		return m_Players[i].silver;
	}

	//  ��ȡ�ͻ�����ҵĿ���
	const std::vector<CardType> & PlayerManager::GetMyPlayerCardType() const
	{
		int i = FindPlayerByID(m_MyPlayerID);
		return m_Players[i].cards;
	}

	//  ��ȡ�ͻ�����ҵ�����
	const std::string & PlayerManager::GetMyPlayerName() const
	{
		int i = FindPlayerByID(m_MyPlayerID);
		return m_Players[i].name;
	}

	//  ��ȡָ���������ʵλ��
	XMFLOAT3 PlayerManager::GetPlayerRealPosition(int ID) const
	{
		int i = FindPlayerByID(ID);
		return m_Players[i].posNow;
	}

	//  ��ȡָ�������������Ϣ
	int PlayerManager::GetPlayerInterests(int ID) const
	{
		int i = FindPlayerByID(ID);
		return m_Players[i].interests;
	}

	//  ��ȡָ������ӵ�еĿ�Ƭ��ö�٣�
	const std::vector<CardType>& PlayerManager::GetPlayerCardType(int ID) const
	{
		int i = FindPlayerByID(ID);
		return m_Players[i].cards;
	}

	//  ���ò�������������ģʽ
	void PlayerManager::Move(int step)
	{
		int i = m_CurrPlyaerIndex;

		//  ���Ӳ���
		m_Players[i].leftStep += step;
		//  ��������ģʽ
		m_Players[i].state.isNeedMove = true;
		m_Players[i].state.isMoving = true;
		//  ������Ҫ��������ģʽ
		m_Players[i].state.isNeedToActivateAfterMove = true;
	}

	//  ��ָ���������ò��������������߽������¼�
	void PlayerManager::JustMove(int ID, int step)
	{
		int i = FindPlayerByID(ID);

		//  ���Ӳ���
		m_Players[i].leftStep += step;
		//  ��������ģʽ
		m_Players[i].state.isNeedMove = true;
		m_Players[i].state.isMoving = true;
	}

	//  ���õ�ǰ���һ���ƶ���Ŀ��λ�úͳ���
	void PlayerManager::SetTargetPosition(XMFLOAT3 pos)
	{
		int i = m_CurrPlyaerIndex;

		//  ����Ŀ��λ��
		m_Players[i].posAfter = pos;

		//  ��ȡ��λ��������
		XMVECTOR after = XMLoadFloat3(&m_Players[i].posAfter);
		XMVECTOR before = XMLoadFloat3(&m_Players[i].posBefore);
		XMVECTOR dirMove = XMVector3Normalize(after - before);
		XMStoreFloat3(&m_Players[i].dirMove, dirMove);

		//  ���������ƶ�״̬
		m_Players[i].state.isMoving = true;
	}

	//  ����ָ�����һ���ƶ���Ŀ��λ�úͳ���
	void PlayerManager::SetTargetPosition(int ID, XMFLOAT3 pos)
	{
		int i = FindPlayerByID(ID);

		//  ����Ŀ��λ��
		m_Players[i].posAfter = pos;

		//  ��ȡ��λ��������
		XMVECTOR after = XMLoadFloat3(&m_Players[i].posAfter);
		XMVECTOR before = XMLoadFloat3(&m_Players[i].posBefore);
		XMVECTOR dirMove = XMVector3Normalize(after - before);
		XMStoreFloat3(&m_Players[i].dirMove, dirMove);

		//  ���������ƶ�״̬
		m_Players[i].state.isMoving = true;
	}

	//  �Զ����������ƶ�
	void PlayerManager::Moving(int ID, float dt)
	{
		int i = FindPlayerByID(ID);

		//  �������ƶ�״̬
		if (m_Players[i].state.isNeedMove == false)
			return;

		//  û�в���
		if (m_Players[i].leftStep <= 0)
		{
			//  ���߶������
			m_Players[i].state.isMoving = false;
			m_Players[i].state.isNeedMove = false;

			//  ��Ҫ���ߺ󼤻�
			if (m_Players[i].state.isNeedToActivateAfterMove == true)
			{
				m_Players[i].state.isActivatingAfterMove = true;
				m_Players[i].state.isNeedToActivateAfterMove = false;
			}
		}

		//  ���в���
		else
		{
			//  �ж�Ŀ��λ���Ƿ���ǰ������ǰ->Ŀ������ �� ���������ķ�����ͬ��
			XMVECTOR posNow = XMLoadFloat3(&m_Players[i].posNow);
			XMVECTOR posAfter = XMLoadFloat3(&m_Players[i].posAfter);
			XMVECTOR nowToAfter = posAfter - posNow;
			XMVECTOR dir = XMLoadFloat3(&m_Players[i].dirMove);

			XMVECTOR dotResult = XMVector3Dot(nowToAfter, dir);

			//  �ӽ�Ŀ�ĵ�
			if(XMVectorGetX(dotResult) < 0.0f)
			{
				//  ���һ����ƶ�
				m_Players[i].posNow = m_Players[i].posAfter;
				m_Players[i].posBefore = m_Players[i].posAfter;
				m_Players[i].state.isMoving = false;
				--m_Players[i].leftStep;
			}

			//  ����ǰ��Ŀ�ĵص�·��
			else
			{
				//  �Ժ�ֱ���ú�������1.0f
				float speed = PLAYER_MOVE_SPEED;
				XMVECTOR posNow = XMLoadFloat3(&m_Players[i].posNow);
				XMVECTOR dir = XMLoadFloat3(&m_Players[i].dirMove);
				posNow = posNow + dir * speed * dt;
				XMStoreFloat3(&m_Players[i].posNow, posNow);

				//  ��������ǰ��״̬
				m_Players[i].state.isMoving = true;
			}
		}
	}

	//  �жϵ�ǰ�����Ƿ���Ҫ������һ��������
	bool PlayerManager::IsUpdateNextStep() const
	{
		int i = m_CurrPlyaerIndex;

		//  ������Ҫ�����߲����Ѿ�����һ��
		if (m_Players[i].state.isNeedMove == true)
			if (m_Players[i].state.isMoving == false)
				return true;
			
		return false;
	}

	//  �ж�ָ�������Ƿ���Ҫ������һ��������
	bool PlayerManager::IsUpdateNextStep(int ID) const
	{
		int i = FindPlayerByID(ID);

		//  ������Ҫ�����߲����Ѿ�����һ��
		if (m_Players[i].state.isNeedMove == true)
			if (m_Players[i].state.isMoving == false)
				return true;

		return false;
	}

	//  �жϵ�ǰ�����Ƿ���Ҫ�������ߺ󼤻��¼������緵��Ϊtrue���ͻ��Զ���λ����һ��ֻ����false��
	bool PlayerManager::IsNeedDealWithEventAfterMove()
	{
		int i = m_CurrPlyaerIndex;

		if (m_Players[i].state.isActivatingAfterMove == true)
		{
			m_Players[i].state.isActivatingAfterMove = false;
			return true;
		}

		return false;
	}

	//  �ж�ָ�������Ƿ���Ҫ�������ߺ󼤻��¼������緵��Ϊtrue���ͻ��Զ���λ����һ��ֻ����false��
	bool PlayerManager::IsNeedDealWithEventAfterMove(int ID)
	{
		int i = FindPlayerByID(ID);

		if (m_Players[i].state.isActivatingAfterMove == true)
		{
			m_Players[i].state.isActivatingAfterMove = false;
			return true;
		}

		return false;
	}

	//  �жϵ�ǰ�Ƿ����������վ��ͬһ�񣨲���Ϊ���ƶ�������ID��
	int PlayerManager::IsStandOnSamePosition(int movePlayerID) const
	{
		//  ���ƶ������
		int i = FindPlayerByID(movePlayerID);

		if (m_Players[i].leftStep > 0)
			return PLAYER_ID_DEFAULT;

		for (PLAYER otherPlayer : m_Players)
		{
			//  �ų��Լ�
			if (m_Players[i].ID == otherPlayer.ID)
				continue;
			if (m_Players[i].blockIndex.x == otherPlayer.blockIndex.x)
				if (m_Players[i].blockIndex.y == otherPlayer.blockIndex.y)
				{
					return otherPlayer.ID;
				}
		}

		return PLAYER_ID_DEFAULT;
	}

	//  ��ǰ�����ĳһλ���ƽ����Ǯ
	void PlayerManager::AverageGold(int targetID)
	{
		int iMy = m_CurrPlyaerIndex;
		int iTarget = FindPlayerByID(targetID);

		//  ����ƽ��ֵ
		int first = m_Players[iMy].gold;
		int second = m_Players[iTarget].gold;
		int average = (first + second) / 2;

		//  �����Ǯ
		m_Players[iMy].gold = average;
		m_Players[iTarget].gold = average;
	}

	//  ��ǰ��һ�ȡ���������Ϣ�������Լ���
	void PlayerManager::RobInterests()
	{
		int i = m_CurrPlyaerIndex;
		float totalGold = 0.0f;

		//  ��ȡ���е���Ϣ
		for (PLAYER player : m_Players)
		{
			totalGold += player.interests;
			player.interests = 0.0f;
		}

		//  ����Ϣ���赱ǰ���
		m_Players[i].gold += static_cast<int>(totalGold);
	}

	//  ������ǰ��������5�ſ���
	void PlayerManager::SellAllCard()
	{
		int i = m_CurrPlyaerIndex;
		int totalSilver = 0;

		//  �������п��Ƶĵㄻ��ֵ
		for (CardType type : m_Players[i].cards)
		{
			CARD card = RichCardManager::GetInstance()->GetCardByEnum(type);
			totalSilver += card.silver;
		}

		//  �������еĿ�Ƭ
		while (m_Players[i].cards.size() > 0)
			ThrowCard();

		//  �ѱ����Ľ�Ǯ���ӵ���ǰ����
		m_Players[i].gold += totalSilver * EXCHANGE_SILVER_TO_GOLD;		
	}

	//  �����ָ��������������
	XMFLOAT4X4 PlayerManager::CaculateWorldMatrix(int ID)
	{
		//  ��ȡָ������������Ϣ
		int i = FindPlayerByID(ID);

		//  ��ȡƽ�ƾ���
		XMMATRIX T = XMMatrixTranslation(
			m_Players[i].posNow.x,
			m_Players[i].posNow.y,
			m_Players[i].posNow.z);

		//  ��ȡ��ת����
		XMFLOAT3 fLook = m_Players[i].dirMove;
		float angle = 0.0f;
		if (::abs(fLook.z) > 0.1f)
		{
			if (fLook.z > 0)
				angle = 0.0f;
			else
				angle = MathHelper::Pi;
		}
		else
		{
			if (fLook.x > 0)
				angle = MathHelper::Pi / 2.0f;
			else
				angle = -MathHelper::Pi / 2.0f;
		}
		XMMATRIX R = XMMatrixRotationY(angle);

		//  ��ȡ�������
		XMMATRIX S = XMMatrixScaling(PLAYER_MODEL_SCALE, PLAYER_MODEL_SCALE, PLAYER_MODEL_SCALE);

		XMFLOAT4X4 matrix;
		XMStoreFloat4x4(&matrix, S*R*T);
		return matrix;

	}

	//  ͨ��ID��Ѱ�Ҷ�Ӧ�������������е�������ʧ�ܷ���-1��
	int PlayerManager::FindPlayerByID(int ID) const
	{
		//  ͨ��IDѰ�����������е�����
		for (int i = 0; i < m_Players.size(); ++i)
		{
			if (m_Players[i].ID == ID)
				return i;
		}

		//  ����-1����Ѱ��ʧ��
		return -1;
		
	}

} //  ���������ռ����

