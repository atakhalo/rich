//***************************************************************************************
//	名字：			大富翁人物管理类
//	作者：			卢俊廷
//  创建日期：		2017-8-24
//  最近更新日期：	2017-8-29
//  
//  需要的头文件
//		CardBag.h
//
//  版本：
//		2017-8-24 实现了当前人物的移动（只是当前）					——by 卢俊廷
//		2017-8-25 实现了人物数据交互接口（未曾调试过，很可能有BUG）	——by 卢俊廷
//		2017-8-25 修改思想并修改代码：人物ID号不再是索引，只是标识	——by 卢俊廷
//		2017-8-25 修改了头文件的注释，修改部分有问题的函数（飞出问题）。
//																	——by 卢俊廷
//		2017-8-26 AddPlayer改为InitPlayer，由一个ID等信息直接生成其他的人物
//																	——by 卢俊廷
//		2017-8-26 增加了人物伪碰撞处理								——by 卢俊廷
//		2017-8-26 增加获取客户端玩家相关信息的函数					——by 卢俊廷
//		2017-8-28 增加获取当前人物ID函数							——by 卢俊廷
//		2017-8-28 改动Find函数并增加const声明。增加获取所有存活人物的ID。											
//																	——by 卢俊廷
//		2017-8-28 增加丢弃指定索引卡片函数							——by 卢俊廷
//		2017-8-29 增加获取人物利息和所有卡片的接口					——by 卢俊廷
//		2017-8-30 增加设置人物利息接口（用于清空人物的利息）		——by 卢俊廷
//		2017-8-31 修改扣除金钱BUG									——by 卢俊廷
//		2017-8-31 增加结算利息和增加利息组合函数					——by 卢俊廷
//		2017-8-31 修正走一步却走了两步的BUG							——by 卢俊廷
//		2017-9-1  修正丢卡BUG										——by 卢俊廷
//		2017-9-1  增加人物停止移动函数								——by 卢俊廷 
//
//	简述：
//		本文件提供人物的属性变化以及在3D场景中的绘制。需要上层控制者传入地图的部分信息。
//		本思路是以触发点为基准，使用框架在【头文件的最后】贴出。
//
//  使用说明：
//		1、（初始化）需要将 Update() 函数放到总框架的更新函数中。
//		2、（初始化）需要将 Draw() 函数放到总框架的绘制函数中。
//
//		3、（运行时初始化）进入场景前调用 InitPlayer() 来载入人物
//			注意：调用后需要设置所有人物的新的当前世界坐标位置和下一步的世界坐标位置
//				  相应的函数为 SetPlayerPosition() 和 SetTargetPosition()
//
//		5、（数据处理）调用 SetTargetPosition() 来更新下一步的世界位置。
//		5、（数据处理）调用 SetPlayerPosition() 来更新当前的世界位置。
//		6、（数据处理）调用 SwitchPlayer() 来更换当前人物的操控权
//		7、（数据处理）设置函数前缀都为 Set...
//		8、（数据处理）访问成员函数前缀都为 Get...
//		9、（数据处理）对数据成员相对增加函数前缀都为 Add，传入负数为减少
//
//		5、（激活点）调用 IsUpdateNextStep() 来判断是否需要更新下一步的世界位置，
//			true为需要。
//		6、（激活点）调用 IsNeedDealWithEventAfterMove() 来判断是否需要处理行走后的
//			连锁事件。
//
//		7、（注意）几乎所有的功能方法都重载了一份含有指定ID参数的方法。使用时可以省略ID，
//			默认为当前玩家。
//		4、（功能方法）调用 Move() 来设置行走的步数（一次走路流程只是设置一次）；
//		7、（功能方法）调用 TurnBack() 来调转当前人物方向
//		
//***************************************************************************************


#ifndef PLAYER_MANAGER_H_
#define PLAYER_MANAGER_H_

#include "CardBag.h"
#include "Sprite3D.h"

#include <map>

namespace Rich
{	
	const int PLAYER_ID_DEFAULT = 0;	//  人物ID号——默认未初始化
	const int PLAYER_ID_ONE = 1;		//  人物ID号——1号人物	
	const int PLAYER_ID_TWO = 2;		//  人物ID号——2号人物
	const int PLAYER_ID_THREE = 3;		//  人物ID号——3号人物
	const int PLAYER_ID_FOUR = 4;		//  人物ID号——4号人物

	const float PLAYER_MODEL_SCALE = 0.015f;	//  人物模型放缩量
	const int PLAYER_CARD_MAX = 5;				//  人物的手牌上限
	const int EXCHANGE_SILVER_TO_GOLD = 250;	//  兑换比率。点券 * 比率 = 金钱
	const float PLAYER_MOVE_SPEED = 50.0f;		//  人物移动速度

	//  人物结构体
	struct PLAYER
	{
	public:
		//  人物状态结构体
		struct PLAYER_STATE
		{
		public:
			PLAYER_STATE() 
				:isMoving(false),
				isNeedMove(false),
				isNeedToActivateAfterMove(false),
				isActivatingAfterMove(false){}
			~PLAYER_STATE() = default;

		public:
			bool isMoving;		//  是否在移动到下一格的路上
			bool isNeedMove;	//  是否需要移动(步数剩余值决定该变量，但变化不是同步)
			bool isNeedToActivateAfterMove;		//  是否需要在人物停下来的时候触发事情
			bool isActivatingAfterMove;			//  是否处于走完激活状态
		};

	public:
		PLAYER();
		~PLAYER() = default;

	public:
		std::string		name;			//  人物名字		
		PLAYER_STATE	state;			//  人物状态		
		int				ID;				//  人物ID（仅仅用作标识）		
		int				gold;			//  人物金钱	
		int				silver;			//  人物点券
		POINT			blockIndex;		//  人物在街道的索引		
		XMFLOAT3		posBefore;		//  人物在世界中的移动前的绝对位置		
		XMFLOAT3		posNow;			//  人物在世界中的真实位置
		XMFLOAT3		posAfter;		//  人物在世界中的移动后的绝对位置
		XMFLOAT3		dirMove;		//  人物在世界中的移动方向
		int				dir;			//  人物在地图中移动的方向（顺时针:1、逆时针:-1）			
		int				leftStep;		//  人物当前剩余步数			
		float			moneyRate;		//  人物当前银行利率
		float			interests;		//  人物当前银行利息
		std::vector<CardType> cards;	//  卡牌容器	
	};

	//  人物管理类
	class PlayerManager
	{
	public:
		PlayerManager() :m_CurrPlyaerIndex(0), m_MyPlayerID(0) {}
		~PlayerManager() = default;

	public:
		//  更新人物结构体信息
		void Update(float dt);
		//  绘制人物
		void Draw();

	//基本函数
	public:
		//  初始化游戏玩家
		void InitPlayer(int playerNum, int gold, int silver, int myID);
		//  切换人物操控权
		void SwitchPlayer();
		//  设置当前玩家位置
		void SetPlayerPosition(XMFLOAT3 pos);
		//  设置当前玩家银行利息
		void SetPlayerInterests(int interests);
		//  改变当前人物金钱
		void AddGold(int money);
		//  改变当前人物点券
		void AddSilver(int silver);
		//  设置当前人物道路索引
		void SetBlockIndex(POINT point);
		//  当前人物转向（为了处理拐角处的朝向问题,传入上一步的位置）
		void TurnBack(XMFLOAT3 backStep);
		//  改变当前人物利率(rate为千分比，1 表示 0.1%)[利率限制在0 - 2%]
		void AddRate(int rate);
		//  当前人物丢弃第一张卡
		CardType ThrowCard();
		//  当前人物获得一张卡
		void ObtainCard(CardType card);
		//  获取当前存活人物ID
		std::vector<int> GetAlivePlayersID() const;
		//  获取当前人物的街道索引
		POINT GetBlockIndex() const;
		//  获取当前人物的ID
		int GetCurrPlayerID() const;
		//  获取当前人物的移动方向(正、负)
		int GetDirection() const;
		//  获取当前人物的真实位置
		XMFLOAT3 GetPlayerRealPosition() const;
		//  获取当前人物的银行利息
		int GetPlayerInterests() const;
		//  获取当前人物拥有的卡片（枚举）
		const std::vector<CardType> & GetPlayerCardType() const;
		//  获取客户端玩家的ID号
		int GetMyPlayerID() const;
		//  获取客户端玩家的金钱
		int GetMyPlayerGold() const;
		//  获取客户端玩家的点券
		int GetMyPlayerSilver() const;
		//  获取客户端玩家的卡牌
		const std::vector<CardType> & GetMyPlayerCardType() const;
		//  获取客户端玩家的名字
		const std::string & GetMyPlayerName() const;
		//  对当前人物设置步数并开启行走模式
		void Move(int step);
		//  对指定人物设置步数但不触发行走结束后事件
		void JustMove(int ID, int step);
		//  设置当前玩家一次移动的目标位置和朝向
		void SetTargetPosition(XMFLOAT3 pos);
		//  判断当前人物是否需要更新下一步的数据
		bool IsUpdateNextStep() const;
		//  判断当前人物是否需要处理行走后激活事件（假如返回为true，就会自动复位，下一次只会是false）
		bool IsNeedDealWithEventAfterMove();
		
	
	//  指定角色函数
	public:
		//  设置指定玩家位置
		void SetPlayerPosition(int ID, XMFLOAT3 pos);
		//  设置指定玩家的银行利息
		void SetPlayerInterests(int ID, int interests);
		//  改变指定人物金钱
		void AddGold(int ID, int money);
		//  改变指定人物点券
		void AddSilver(int ID, int silver);
		//  设置指定人物道路索引
		void SetBlockIndex(int ID, POINT point);
		//  指定人物转向（为了处理拐角处的朝向问题,传入上一步的位置）
		void TurnBack(int ID, XMFLOAT3 backStep);
		//  改变指定人物利率(rate为千分比，1 表示 0.1%)[利率限制在0 - 2%]
		void AddRate(int ID, int rate);
		//  指定人物丢弃特定的卡
		CardType ThrowCard(int ID, int cardIndex);
		//  指定人物丢弃第一张卡
		CardType ThrowCard(int ID);
		//  指定人物获得一张卡(性能有待提高)
		void ObtainCard(int ID, CardType card);
		//  获取指定人物的道路索引（失败返回（-1，-1））
		POINT GetBlockIndex(int ID) const;
		//  获取指定人物的移动方向（正、负）（失败返回0）
		int GetDirection(int ID) const;
		//  获取指定人物的真实位置
		XMFLOAT3 GetPlayerRealPosition(int ID) const;
		//  获取指定人物的银行利息
		int GetPlayerInterests(int ID) const;
		//  获取指定人物拥有的卡片（枚举）
		const std::vector<CardType> & GetPlayerCardType(int ID) const;
		//  设置指定玩家一次移动的目标位置和朝向
		void SetTargetPosition(int ID, XMFLOAT3 pos);
		//  判断指定人物是否需要更新下一步的数据
		bool IsUpdateNextStep(int ID) const;
		//  判断指定人物是否需要处理行走后激活事件（假如返回为true，就会自动复位，下一次只会是false）
		bool IsNeedDealWithEventAfterMove(int ID);
		//  判断当前是否有其他玩家站在同一格（参数为刚移动的人物ID，返回值为正在这一格
		//  的玩家ID，假如没有玩家返回 PLAYER_ID_DEFAULT）
		int IsStandOnSamePosition(int movePlayerID) const;

	//  组合修改函数
	public:
		//  当前玩家与某一位玩家平均金钱
		void AverageGold(int targetID);
		//  当前玩家获取所有玩家利息（包括自己）
		void RobInterests();
		//  变卖当前人物的最多5张卡牌
		void SellAllCard();
		//  结算当前玩家银行利息【组合技】
		void CalculateCurrPlayerInterests();
		//  当前人物进入银行取钱，把利息转变为金钱【组合技】
		void ChangeInterestsToMoney();
		//  暂停指定人物移动
		void StopPlayer(int ID);

	private:
		//  计算出指定人物的世界矩阵
		XMFLOAT4X4 CaculateWorldMatrix(int ID);
		//  自动更新人物移动
		void Moving(int ID, float dt);
		//  通过ID号寻找对应的人物在容器中的索引（失败返回-1）
		int FindPlayerByID(int ID) const;

	private:
		std::vector<PLAYER>		m_Players;			//  人物容器[逻辑上最多4个玩家]		
		int						m_CurrPlyaerIndex;	//  当前操作的人物索引	
		std::map<std::string, Sprite3D> m_SPlayer;	//  人物名字 <--> 3D精灵 哈希表
		int						m_MyPlayerID;		//  当前客户端的玩家ID号
	};


} //  大富翁命名空间

#endif // !PLAYER_MANAGER_H_

//*************************************************************************************
//	使用框架：
//  需要在主框架中增加2个函数，方便人物类和地图类的信息交互。
//
////  根据当前道路索引与方向确定人物的当前世界坐标与下一步的世界坐标
//void GameApp::UpDatePlayerPosition(int ID)
//{
//	//  设置玩家当前位置
//	POINT blockIndex = m_PlayerManager.GetBlockIndex(ID);
//	XMFLOAT3 pos = m_StreetManager.BlockTransFloat3Get(blockIndex);
//	m_PlayerManager.SetPlayerPosition(ID, pos);
//
//	//  设置玩家下一步位置
//	POINT nextPosIndex = m_StreetManager.BlockNextGet(
//		m_PlayerManager.GetBlockIndex(ID),
//		m_PlayerManager.GetDirection(ID));
//	XMFLOAT3 nextPos = m_StreetManager.BlockTransFloat3Get(nextPosIndex);
//	m_PlayerManager.SetTargetPosition(ID, nextPos);
//}
//
////  玩家移动后把原来的道路索引设置为下一步的街道索引
//void GameApp::UpDatePlayerBlockIndex(int ID)
//{
//	POINT blockIndex = m_StreetManager.BlockNextGet(
//		m_PlayerManager.GetBlockIndex(ID),
//		m_PlayerManager.GetDirection(ID)
//	);
//	m_PlayerManager.SetBlockIndex(ID, blockIndex);
//}
//
//	创建房间后，进入游戏场景前，加载人物，并获取自己的ID号
//	m_PlayerManager.InitPlayer(人数, 初始金额, 初始点券, 自己的ID号);
//	测试的时候可以在Init()中加入：
//
////  加载人物
// m_PlayerManager.InitPlayer(4, 1000, 100, Rich::PLAYER_ID_ONE);  
////更新人物世界坐标位置
//UpDatePlayerPosition(Rich::PLAYER_ID_ONE);						
//UpDatePlayerPosition(Rich::PLAYER_ID_TWO);
//UpDatePlayerPosition(Rich::PLAYER_ID_THREE);
//UpDatePlayerPosition(Rich::PLAYER_ID_FOUR);
//
//  信息处理框架（加在UpdateScreen函数中）
//
//int ID[4] = {
//Rich::PLAYER_ID_ONE,
//Rich::PLAYER_ID_TWO,
//Rich::PLAYER_ID_THREE,
//Rich::PLAYER_ID_FOUR };
//
//for (int i = 0; i < 4; i++)
//{
//	//  人物走完一步后的操作
//	if (m_PlayerManager.IsUpdateNextStep(ID[i]) == true)
//	{
//		UpDatePlayerBlockIndex(ID[i]);
//		UpDatePlayerPosition(ID[i]);
//
//		int playerID = m_PlayerManager.IsStandOnSamePosition(ID[i]);
//		//  移动的位置已经有玩家
//		if (playerID != Rich::PLAYER_ID_DEFAULT)
//		{
//			m_PlayerManager.JustMove(playerID, 1);
//		}
//	}
//
//	//  人物走完一次后的操作
//	if (m_PlayerManager.IsNeedDealWithEventAfterMove(ID[i]) == true)
//	{
//		::MessageBox(NULL, L"触发行走后事件", L"消息", 0);
//	}
//}
//
////  更新人物数据
//m_PlayerManager.Update(dt);
//
////  绘制人物 DrawScene()
//m_PlayerManager.Draw();
//*************************************************************************************
