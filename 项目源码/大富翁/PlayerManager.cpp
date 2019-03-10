//***************************************************************************************
//	名字：			大富翁人物管理类 * 源文件
//	作者：			卢俊廷
//  创建日期：		2017-8-24
//  最近更新日期：	2017-8-26
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

	//  更新人物结构体信息
	void PlayerManager::Update(float dt)
	{
		//  所有人物行走一帧
		for (PLAYER player : m_Players)
		{
			Moving(player.ID, dt);
		}

		//  更新人物的世界矩阵
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

	//  绘制人物
	void PlayerManager::Draw()
	{
		for (PLAYER player : m_Players)
		{
			m_SPlayer[player.name].DrawInstances();
		}
	}

	//  初始化游戏玩家
	void PlayerManager::InitPlayer(int playerNum, int gold, int silver, int myID)
	{
		//  载入相应数量的人物
		std::string name[4]{"火灵","土灵","水灵","原谅"};

		//  资源名字
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

			//  【测试中】——初始卡片
			//newPlayer.cards.clear();
			//newPlayer.cards.push_back(CardType::CARD_STAY);

			m_Players.push_back(newPlayer);

			std::string obj = std::string("Object\\") + resourceName[i] + std::string(".obj");
			std::string png = std::string("Texture\\") + resourceName[i] + std::string(".png");
			std::string mtl = std::string("Material\\") + resourceName[i] + std::string(".mtl");

			m_SPlayer[name[i]].InitModel(obj.c_str(), png.c_str(), mtl.c_str());
		}

		//  记录我的ID号
		m_MyPlayerID = myID;
	}

	//  切换人物操控权
	void PlayerManager::SwitchPlayer()
	{
		int index = m_CurrPlyaerIndex;
		index = (index + 1) % m_Players.size();
		m_CurrPlyaerIndex = index;
	}

	//  结算当前玩家银行利息【组合技】
	void PlayerManager::CalculateCurrPlayerInterests()
	{
		int i = m_CurrPlyaerIndex;
		int gold = m_Players[i].gold;
		int rate = m_Players[i].interests;

		//  增加利息值 = 当前总金钱 * 当前利率
		int interests = gold * rate;

		m_Players[i].interests += interests;
	}

	//  当前人物进入银行取钱，把利息转变为金钱【组合技】
	void PlayerManager::ChangeInterestsToMoney()
	{
		//  清空利息
		int interests = GetPlayerInterests();
		SetPlayerInterests(0);

		//  增加金钱
		AddGold(interests);
	}

	//  暂停指定人物移动
	void PlayerManager::StopPlayer(int ID)
	{
		int i = FindPlayerByID(ID);
		m_Players[i].leftStep = 0;
	}

	//  设置当前玩家位置
	void PlayerManager::SetPlayerPosition(XMFLOAT3 pos)
	{
		int i = m_CurrPlyaerIndex;

		m_Players[i].posBefore = pos;
		m_Players[i].posNow = pos;
	}

	//  设置当前玩家银行利息
	void PlayerManager::SetPlayerInterests(int interests)
	{
		int i = m_CurrPlyaerIndex;
		m_Players[i].interests = interests;
	}

	//  设置指定玩家位置
	void PlayerManager::SetPlayerPosition(int ID, XMFLOAT3 pos)
	{
		int i = FindPlayerByID(ID);
		m_Players[i].posBefore = pos;
		m_Players[i].posNow = pos;
	}

	//  设置指定玩家的银行利息
	void PlayerManager::SetPlayerInterests(int ID, int interests)
	{
		int i = FindPlayerByID(ID);
		m_Players[i].interests = interests;
	}

	//  改变当前人物金钱
	void PlayerManager::AddGold(int money)
	{
		int i = m_CurrPlyaerIndex;
		m_Players[i].gold += money;
	}

	//  改变指定人物金钱
	void PlayerManager::AddGold(int ID, int money)
	{
		int i = FindPlayerByID(ID);
		m_Players[i].gold += money;
	}

	//  改变当前人物点券
	void PlayerManager::AddSilver(int silver)
	{
		int i = m_CurrPlyaerIndex;
		m_Players[i].silver += silver;
	}

	//  改变指定人物点券
	void PlayerManager::AddSilver(int ID, int silver)
	{
		int i = FindPlayerByID(ID);
		m_Players[i].silver += silver;
	}

	//  设置当前人物道路索引
	void PlayerManager::SetBlockIndex(POINT point)
	{
		int i = m_CurrPlyaerIndex;
		m_Players[i].blockIndex = point;
	}

	//  设置指定人物道路索引
	void PlayerManager::SetBlockIndex(int ID, POINT point)
	{
		int i = FindPlayerByID(ID);
		m_Players[i].blockIndex = point;
	}

	//  当前人物转向
	void PlayerManager::TurnBack(XMFLOAT3 backStep)
	{
		int i = m_CurrPlyaerIndex;

		//  行走方向改变
		m_Players[i].dir *= -1;
		//  目标改变
		m_Players[i].posAfter = backStep;
		//  朝向改变
		XMVECTOR before = XMLoadFloat3(&m_Players[i].posBefore);
		XMVECTOR after = XMLoadFloat3(&m_Players[i].posAfter);
		XMVECTOR dirMove = after - before;
		XMStoreFloat3(&m_Players[i].dirMove, dirMove);
	}
	
	//  指定人物转向
	void PlayerManager::TurnBack(int ID, XMFLOAT3 backStep)
	{
		int i = FindPlayerByID(ID);

		//  行走方向改变
		m_Players[i].dir *= -1;
		//  目标改变
		m_Players[i].posAfter = backStep;
		//  朝向改变
		XMVECTOR before = XMLoadFloat3(&m_Players[i].posBefore);
		XMVECTOR after = XMLoadFloat3(&m_Players[i].posAfter);
		XMVECTOR dirMove = after - before;
		XMStoreFloat3(&m_Players[i].dirMove, dirMove);
	}

	//  改变当前人物利率(rate为千分比，1 表示 0.1%)[利率限制在0 - 2%]
	void PlayerManager::AddRate(int rate)
	{
		int i = m_CurrPlyaerIndex;
		float nowRate = m_Players[i].moneyRate;

		nowRate = MathHelper::Clamp(nowRate + rate * 0.001f, 0.0f, 2.0f);
		m_Players[i].moneyRate = nowRate;
	}

	//  改变指定人物利率(rate为千分比，1 表示 0.1%)[利率限制在0 - 2%]
	void PlayerManager::AddRate(int ID, int rate)
	{
		int i = FindPlayerByID(ID);

		float nowRate = m_Players[i].moneyRate;

		nowRate = MathHelper::Clamp(nowRate + rate * 0.001f, 0.0f, 2.0f);
		m_Players[i].moneyRate = nowRate;
	}

	//  指定人物丢弃特定的卡
	CardType PlayerManager::ThrowCard(int ID, int cardIndex)
	{
		int i = FindPlayerByID(ID);

		//  没有卡牌
		if (m_Players[i].cards.size() <= 0)
			return CardType::CARD_NONE;

		//  索引超过范围
		if(cardIndex < 0 || cardIndex >= m_Players[i].cards.size())
			return CardType::CARD_NONE;

		//  丢弃一张卡牌
		CardType card = m_Players[i].cards[cardIndex];
		m_Players[i].cards.erase(m_Players[i].cards.begin() + cardIndex);
		return card;
	}

	//  当前人物丢弃第一张卡
	CardType PlayerManager::ThrowCard()
	{
		int i = m_CurrPlyaerIndex;

		//  没有卡牌
		if(m_Players[i].cards.size() <= 0)
			return CardType::CARD_NONE;

		//  丢弃一张卡牌
		CardType card = m_Players[i].cards[0];
		m_Players[i].cards.erase(m_Players[i].cards.begin());
		return card;
	}

	//  指定人物丢弃第一张卡
	CardType PlayerManager::ThrowCard(int ID)
	{
		int i = FindPlayerByID(ID);

		//  没有卡牌
		if (m_Players[i].cards.size() <= 0)
			return CardType::CARD_NONE;

		//  丢弃一张卡牌
		CardType card = m_Players[i].cards[0];
		m_Players[i].cards.erase(m_Players[i].cards.begin());
		return card;
	}

	//  当前人物获得一张卡
	void PlayerManager::ObtainCard(CardType card)
	{
		int i = m_CurrPlyaerIndex;

		//  丢弃卡片，直至手牌数量 <= 手牌上限-1
		while(m_Players[i].cards.size() > PLAYER_CARD_MAX - 1)
			ThrowCard();

		//  添加卡片
		m_Players[i].cards.push_back(card);
	}

	//  获取当前存活人物ID
	std::vector<int> PlayerManager::GetAlivePlayersID() const
	{
		std::vector<int> IDs;
		for (PLAYER player : m_Players)
		{
			IDs.push_back(player.ID);
		}
		return IDs;
	}

	//  指定人物获得一张卡
	void PlayerManager::ObtainCard(int ID, CardType card)
	{
		int i = FindPlayerByID(ID);

		//  丢弃卡片，直至手牌数量 <= 手牌上限-1
		while (m_Players[i].cards.size() > PLAYER_CARD_MAX - 1)
			ThrowCard(ID);

		//  添加卡片
		m_Players[i].cards.push_back(card);
	}

	//  获取当前人物的街道索引
	POINT PlayerManager::GetBlockIndex() const
	{
		int i = m_CurrPlyaerIndex;
		return m_Players[i].blockIndex;
	}

	//  获取当前人物的ID
	int PlayerManager::GetCurrPlayerID() const
	{
		int i = m_CurrPlyaerIndex;
		return m_Players[i].ID;
	}

	//  获取指定人物的道路索引
	POINT PlayerManager::GetBlockIndex(int ID) const
	{
		int i = FindPlayerByID(ID);
		return m_Players[i].blockIndex;
	}

	//  获取当前人物的移动方向(正、负)
	int PlayerManager::GetDirection() const
	{
		int i = m_CurrPlyaerIndex;
		return m_Players[i].dir;
	}

	//  获取指定人物的移动方向（正、负）
	int PlayerManager::GetDirection(int ID) const
	{
		int i = FindPlayerByID(ID);
		return m_Players[i].dir;
	}

	//  获取当前人物的真实位置
	XMFLOAT3 PlayerManager::GetPlayerRealPosition() const
	{
		int i = m_CurrPlyaerIndex;
		return m_Players[i].posNow;
	}

	//  获取当前人物的银行利息
	int PlayerManager::GetPlayerInterests() const
	{
		int i = m_CurrPlyaerIndex;
		return m_Players[i].interests;
	}

	//  获取当前人物拥有的卡片（枚举）
	const std::vector<CardType>& PlayerManager::GetPlayerCardType() const
	{
		int i = m_CurrPlyaerIndex;
		return m_Players[i].cards;
	}

	//  获取客户端玩家的ID号
	int PlayerManager::GetMyPlayerID() const
	{
		return m_MyPlayerID;
	}

	//  获取客户端玩家的金钱
	int PlayerManager::GetMyPlayerGold() const
	{
		int i = FindPlayerByID(m_MyPlayerID);
		return m_Players[i].gold;
	}

	//  获取客户端玩家的点券
	int PlayerManager::GetMyPlayerSilver() const
	{
		int i = FindPlayerByID(m_MyPlayerID);
		return m_Players[i].silver;
	}

	//  获取客户端玩家的卡牌
	const std::vector<CardType> & PlayerManager::GetMyPlayerCardType() const
	{
		int i = FindPlayerByID(m_MyPlayerID);
		return m_Players[i].cards;
	}

	//  获取客户端玩家的名字
	const std::string & PlayerManager::GetMyPlayerName() const
	{
		int i = FindPlayerByID(m_MyPlayerID);
		return m_Players[i].name;
	}

	//  获取指定人物的真实位置
	XMFLOAT3 PlayerManager::GetPlayerRealPosition(int ID) const
	{
		int i = FindPlayerByID(ID);
		return m_Players[i].posNow;
	}

	//  获取指定人物的银行利息
	int PlayerManager::GetPlayerInterests(int ID) const
	{
		int i = FindPlayerByID(ID);
		return m_Players[i].interests;
	}

	//  获取指定人物拥有的卡片（枚举）
	const std::vector<CardType>& PlayerManager::GetPlayerCardType(int ID) const
	{
		int i = FindPlayerByID(ID);
		return m_Players[i].cards;
	}

	//  设置步数并开启行走模式
	void PlayerManager::Move(int step)
	{
		int i = m_CurrPlyaerIndex;

		//  增加步数
		m_Players[i].leftStep += step;
		//  开启行走模式
		m_Players[i].state.isNeedMove = true;
		m_Players[i].state.isMoving = true;
		//  开启需要触发事情模式
		m_Players[i].state.isNeedToActivateAfterMove = true;
	}

	//  对指定人物设置步数但不触发行走结束后事件
	void PlayerManager::JustMove(int ID, int step)
	{
		int i = FindPlayerByID(ID);

		//  增加步数
		m_Players[i].leftStep += step;
		//  开启行走模式
		m_Players[i].state.isNeedMove = true;
		m_Players[i].state.isMoving = true;
	}

	//  设置当前玩家一次移动的目标位置和朝向
	void PlayerManager::SetTargetPosition(XMFLOAT3 pos)
	{
		int i = m_CurrPlyaerIndex;

		//  设置目标位置
		m_Players[i].posAfter = pos;

		//  获取单位方向向量
		XMVECTOR after = XMLoadFloat3(&m_Players[i].posAfter);
		XMVECTOR before = XMLoadFloat3(&m_Players[i].posBefore);
		XMVECTOR dirMove = XMVector3Normalize(after - before);
		XMStoreFloat3(&m_Players[i].dirMove, dirMove);

		//  设置正在移动状态
		m_Players[i].state.isMoving = true;
	}

	//  设置指定玩家一次移动的目标位置和朝向
	void PlayerManager::SetTargetPosition(int ID, XMFLOAT3 pos)
	{
		int i = FindPlayerByID(ID);

		//  设置目标位置
		m_Players[i].posAfter = pos;

		//  获取单位方向向量
		XMVECTOR after = XMLoadFloat3(&m_Players[i].posAfter);
		XMVECTOR before = XMLoadFloat3(&m_Players[i].posBefore);
		XMVECTOR dirMove = XMVector3Normalize(after - before);
		XMStoreFloat3(&m_Players[i].dirMove, dirMove);

		//  设置正在移动状态
		m_Players[i].state.isMoving = true;
	}

	//  自动更新人物移动
	void PlayerManager::Moving(int ID, float dt)
	{
		int i = FindPlayerByID(ID);

		//  不是在移动状态
		if (m_Players[i].state.isNeedMove == false)
			return;

		//  没有步数
		if (m_Players[i].leftStep <= 0)
		{
			//  行走动作完成
			m_Players[i].state.isMoving = false;
			m_Players[i].state.isNeedMove = false;

			//  需要行走后激活
			if (m_Players[i].state.isNeedToActivateAfterMove == true)
			{
				m_Players[i].state.isActivatingAfterMove = true;
				m_Players[i].state.isNeedToActivateAfterMove = false;
			}
		}

		//  还有步数
		else
		{
			//  判断目标位置是否在前方（当前->目标向量 与 方向向量的方向相同）
			XMVECTOR posNow = XMLoadFloat3(&m_Players[i].posNow);
			XMVECTOR posAfter = XMLoadFloat3(&m_Players[i].posAfter);
			XMVECTOR nowToAfter = posAfter - posNow;
			XMVECTOR dir = XMLoadFloat3(&m_Players[i].dirMove);

			XMVECTOR dotResult = XMVector3Dot(nowToAfter, dir);

			//  接近目的地
			if(XMVectorGetX(dotResult) < 0.0f)
			{
				//  完成一格的移动
				m_Players[i].posNow = m_Players[i].posAfter;
				m_Players[i].posBefore = m_Players[i].posAfter;
				m_Players[i].state.isMoving = false;
				--m_Players[i].leftStep;
			}

			//  正在前往目的地的路上
			else
			{
				//  以后直接用函数代替1.0f
				float speed = PLAYER_MOVE_SPEED;
				XMVECTOR posNow = XMLoadFloat3(&m_Players[i].posNow);
				XMVECTOR dir = XMLoadFloat3(&m_Players[i].dirMove);
				posNow = posNow + dir * speed * dt;
				XMStoreFloat3(&m_Players[i].posNow, posNow);

				//  设置正在前往状态
				m_Players[i].state.isMoving = true;
			}
		}
	}

	//  判断当前人物是否需要更新下一步的数据
	bool PlayerManager::IsUpdateNextStep() const
	{
		int i = m_CurrPlyaerIndex;

		//  人物需要继续走并且已经走完一格
		if (m_Players[i].state.isNeedMove == true)
			if (m_Players[i].state.isMoving == false)
				return true;
			
		return false;
	}

	//  判断指定人物是否需要更新下一步的数据
	bool PlayerManager::IsUpdateNextStep(int ID) const
	{
		int i = FindPlayerByID(ID);

		//  人物需要继续走并且已经走完一格
		if (m_Players[i].state.isNeedMove == true)
			if (m_Players[i].state.isMoving == false)
				return true;

		return false;
	}

	//  判断当前人物是否需要处理行走后激活事件（假如返回为true，就会自动复位，下一次只会是false）
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

	//  判断指定人物是否需要处理行走后激活事件（假如返回为true，就会自动复位，下一次只会是false）
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

	//  判断当前是否有其他玩家站在同一格（参数为刚移动的人物ID）
	int PlayerManager::IsStandOnSamePosition(int movePlayerID) const
	{
		//  刚移动的玩家
		int i = FindPlayerByID(movePlayerID);

		if (m_Players[i].leftStep > 0)
			return PLAYER_ID_DEFAULT;

		for (PLAYER otherPlayer : m_Players)
		{
			//  排除自己
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

	//  当前玩家与某一位玩家平均金钱
	void PlayerManager::AverageGold(int targetID)
	{
		int iMy = m_CurrPlyaerIndex;
		int iTarget = FindPlayerByID(targetID);

		//  计算平均值
		int first = m_Players[iMy].gold;
		int second = m_Players[iTarget].gold;
		int average = (first + second) / 2;

		//  分配金钱
		m_Players[iMy].gold = average;
		m_Players[iTarget].gold = average;
	}

	//  当前玩家获取所有玩家利息（包括自己）
	void PlayerManager::RobInterests()
	{
		int i = m_CurrPlyaerIndex;
		float totalGold = 0.0f;

		//  提取所有的利息
		for (PLAYER player : m_Players)
		{
			totalGold += player.interests;
			player.interests = 0.0f;
		}

		//  把利息给予当前玩家
		m_Players[i].gold += static_cast<int>(totalGold);
	}

	//  变卖当前人物的最多5张卡牌
	void PlayerManager::SellAllCard()
	{
		int i = m_CurrPlyaerIndex;
		int totalSilver = 0;

		//  计算所有卡牌的点劵价值
		for (CardType type : m_Players[i].cards)
		{
			CARD card = RichCardManager::GetInstance()->GetCardByEnum(type);
			totalSilver += card.silver;
		}

		//  丢弃所有的卡片
		while (m_Players[i].cards.size() > 0)
			ThrowCard();

		//  把变卖的金钱增加到当前人物
		m_Players[i].gold += totalSilver * EXCHANGE_SILVER_TO_GOLD;		
	}

	//  计算出指定人物的世界矩阵
	XMFLOAT4X4 PlayerManager::CaculateWorldMatrix(int ID)
	{
		//  获取指定人物的相关信息
		int i = FindPlayerByID(ID);

		//  获取平移矩阵
		XMMATRIX T = XMMatrixTranslation(
			m_Players[i].posNow.x,
			m_Players[i].posNow.y,
			m_Players[i].posNow.z);

		//  获取旋转矩阵
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

		//  获取拉伸矩阵
		XMMATRIX S = XMMatrixScaling(PLAYER_MODEL_SCALE, PLAYER_MODEL_SCALE, PLAYER_MODEL_SCALE);

		XMFLOAT4X4 matrix;
		XMStoreFloat4x4(&matrix, S*R*T);
		return matrix;

	}

	//  通过ID号寻找对应的人物在容器中的索引（失败返回-1）
	int PlayerManager::FindPlayerByID(int ID) const
	{
		//  通过ID寻找人物容器中的索引
		for (int i = 0; i < m_Players.size(); ++i)
		{
			if (m_Players[i].ID == ID)
				return i;
		}

		//  返回-1当做寻找失败
		return -1;
		
	}

} //  大富翁命名空间结束

