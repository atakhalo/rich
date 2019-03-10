//***************************************************************************************
//	���֣�			�������������
//	���ߣ�			¬��͢
//  �������ڣ�		2017-8-24
//  ����������ڣ�	2017-8-29
//  
//  ��Ҫ��ͷ�ļ�
//		CardBag.h
//
//  �汾��
//		2017-8-24 ʵ���˵�ǰ������ƶ���ֻ�ǵ�ǰ��					����by ¬��͢
//		2017-8-25 ʵ�����������ݽ����ӿڣ�δ�����Թ����ܿ�����BUG��	����by ¬��͢
//		2017-8-25 �޸�˼�벢�޸Ĵ��룺����ID�Ų�����������ֻ�Ǳ�ʶ	����by ¬��͢
//		2017-8-25 �޸���ͷ�ļ���ע�ͣ��޸Ĳ���������ĺ������ɳ����⣩��
//																	����by ¬��͢
//		2017-8-26 AddPlayer��ΪInitPlayer����һ��ID����Ϣֱ����������������
//																	����by ¬��͢
//		2017-8-26 ����������α��ײ����								����by ¬��͢
//		2017-8-26 ���ӻ�ȡ�ͻ�����������Ϣ�ĺ���					����by ¬��͢
//		2017-8-28 ���ӻ�ȡ��ǰ����ID����							����by ¬��͢
//		2017-8-28 �Ķ�Find����������const���������ӻ�ȡ���д�������ID��											
//																	����by ¬��͢
//		2017-8-28 ���Ӷ���ָ��������Ƭ����							����by ¬��͢
//		2017-8-29 ���ӻ�ȡ������Ϣ�����п�Ƭ�Ľӿ�					����by ¬��͢
//		2017-8-30 ��������������Ϣ�ӿڣ���������������Ϣ��		����by ¬��͢
//		2017-8-31 �޸Ŀ۳���ǮBUG									����by ¬��͢
//		2017-8-31 ���ӽ�����Ϣ��������Ϣ��Ϻ���					����by ¬��͢
//		2017-8-31 ������һ��ȴ����������BUG							����by ¬��͢
//		2017-9-1  ��������BUG										����by ¬��͢
//		2017-9-1  ��������ֹͣ�ƶ�����								����by ¬��͢ 
//
//	������
//		���ļ��ṩ��������Ա仯�Լ���3D�����еĻ��ơ���Ҫ�ϲ�����ߴ����ͼ�Ĳ�����Ϣ��
//		��˼·���Դ�����Ϊ��׼��ʹ�ÿ���ڡ�ͷ�ļ������������
//
//  ʹ��˵����
//		1������ʼ������Ҫ�� Update() �����ŵ��ܿ�ܵĸ��º����С�
//		2������ʼ������Ҫ�� Draw() �����ŵ��ܿ�ܵĻ��ƺ����С�
//
//		3��������ʱ��ʼ�������볡��ǰ���� InitPlayer() ����������
//			ע�⣺���ú���Ҫ��������������µĵ�ǰ��������λ�ú���һ������������λ��
//				  ��Ӧ�ĺ���Ϊ SetPlayerPosition() �� SetTargetPosition()
//
//		5�������ݴ������� SetTargetPosition() ��������һ��������λ�á�
//		5�������ݴ������� SetPlayerPosition() �����µ�ǰ������λ�á�
//		6�������ݴ������� SwitchPlayer() ��������ǰ����Ĳٿ�Ȩ
//		7�������ݴ������ú���ǰ׺��Ϊ Set...
//		8�������ݴ������ʳ�Ա����ǰ׺��Ϊ Get...
//		9�������ݴ��������ݳ�Ա������Ӻ���ǰ׺��Ϊ Add�����븺��Ϊ����
//
//		5��������㣩���� IsUpdateNextStep() ���ж��Ƿ���Ҫ������һ��������λ�ã�
//			trueΪ��Ҫ��
//		6��������㣩���� IsNeedDealWithEventAfterMove() ���ж��Ƿ���Ҫ�������ߺ��
//			�����¼���
//
//		7����ע�⣩�������еĹ��ܷ�����������һ�ݺ���ָ��ID�����ķ�����ʹ��ʱ����ʡ��ID��
//			Ĭ��Ϊ��ǰ��ҡ�
//		4�������ܷ��������� Move() ���������ߵĲ�����һ����·����ֻ������һ�Σ���
//		7�������ܷ��������� TurnBack() ����ת��ǰ���﷽��
//		
//***************************************************************************************


#ifndef PLAYER_MANAGER_H_
#define PLAYER_MANAGER_H_

#include "CardBag.h"
#include "Sprite3D.h"

#include <map>

namespace Rich
{	
	const int PLAYER_ID_DEFAULT = 0;	//  ����ID�š���Ĭ��δ��ʼ��
	const int PLAYER_ID_ONE = 1;		//  ����ID�š���1������	
	const int PLAYER_ID_TWO = 2;		//  ����ID�š���2������
	const int PLAYER_ID_THREE = 3;		//  ����ID�š���3������
	const int PLAYER_ID_FOUR = 4;		//  ����ID�š���4������

	const float PLAYER_MODEL_SCALE = 0.015f;	//  ����ģ�ͷ�����
	const int PLAYER_CARD_MAX = 5;				//  �������������
	const int EXCHANGE_SILVER_TO_GOLD = 250;	//  �һ����ʡ���ȯ * ���� = ��Ǯ
	const float PLAYER_MOVE_SPEED = 50.0f;		//  �����ƶ��ٶ�

	//  ����ṹ��
	struct PLAYER
	{
	public:
		//  ����״̬�ṹ��
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
			bool isMoving;		//  �Ƿ����ƶ�����һ���·��
			bool isNeedMove;	//  �Ƿ���Ҫ�ƶ�(����ʣ��ֵ�����ñ��������仯����ͬ��)
			bool isNeedToActivateAfterMove;		//  �Ƿ���Ҫ������ͣ������ʱ�򴥷�����
			bool isActivatingAfterMove;			//  �Ƿ������꼤��״̬
		};

	public:
		PLAYER();
		~PLAYER() = default;

	public:
		std::string		name;			//  ��������		
		PLAYER_STATE	state;			//  ����״̬		
		int				ID;				//  ����ID������������ʶ��		
		int				gold;			//  �����Ǯ	
		int				silver;			//  �����ȯ
		POINT			blockIndex;		//  �����ڽֵ�������		
		XMFLOAT3		posBefore;		//  �����������е��ƶ�ǰ�ľ���λ��		
		XMFLOAT3		posNow;			//  �����������е���ʵλ��
		XMFLOAT3		posAfter;		//  �����������е��ƶ���ľ���λ��
		XMFLOAT3		dirMove;		//  �����������е��ƶ�����
		int				dir;			//  �����ڵ�ͼ���ƶ��ķ���˳ʱ��:1����ʱ��:-1��			
		int				leftStep;		//  ���ﵱǰʣ�ಽ��			
		float			moneyRate;		//  ���ﵱǰ��������
		float			interests;		//  ���ﵱǰ������Ϣ
		std::vector<CardType> cards;	//  ��������	
	};

	//  ���������
	class PlayerManager
	{
	public:
		PlayerManager() :m_CurrPlyaerIndex(0), m_MyPlayerID(0) {}
		~PlayerManager() = default;

	public:
		//  ��������ṹ����Ϣ
		void Update(float dt);
		//  ��������
		void Draw();

	//��������
	public:
		//  ��ʼ����Ϸ���
		void InitPlayer(int playerNum, int gold, int silver, int myID);
		//  �л�����ٿ�Ȩ
		void SwitchPlayer();
		//  ���õ�ǰ���λ��
		void SetPlayerPosition(XMFLOAT3 pos);
		//  ���õ�ǰ���������Ϣ
		void SetPlayerInterests(int interests);
		//  �ı䵱ǰ�����Ǯ
		void AddGold(int money);
		//  �ı䵱ǰ�����ȯ
		void AddSilver(int silver);
		//  ���õ�ǰ�����·����
		void SetBlockIndex(POINT point);
		//  ��ǰ����ת��Ϊ�˴���սǴ��ĳ�������,������һ����λ�ã�
		void TurnBack(XMFLOAT3 backStep);
		//  �ı䵱ǰ��������(rateΪǧ�ֱȣ�1 ��ʾ 0.1%)[����������0 - 2%]
		void AddRate(int rate);
		//  ��ǰ���ﶪ����һ�ſ�
		CardType ThrowCard();
		//  ��ǰ������һ�ſ�
		void ObtainCard(CardType card);
		//  ��ȡ��ǰ�������ID
		std::vector<int> GetAlivePlayersID() const;
		//  ��ȡ��ǰ����Ľֵ�����
		POINT GetBlockIndex() const;
		//  ��ȡ��ǰ�����ID
		int GetCurrPlayerID() const;
		//  ��ȡ��ǰ������ƶ�����(������)
		int GetDirection() const;
		//  ��ȡ��ǰ�������ʵλ��
		XMFLOAT3 GetPlayerRealPosition() const;
		//  ��ȡ��ǰ�����������Ϣ
		int GetPlayerInterests() const;
		//  ��ȡ��ǰ����ӵ�еĿ�Ƭ��ö�٣�
		const std::vector<CardType> & GetPlayerCardType() const;
		//  ��ȡ�ͻ�����ҵ�ID��
		int GetMyPlayerID() const;
		//  ��ȡ�ͻ�����ҵĽ�Ǯ
		int GetMyPlayerGold() const;
		//  ��ȡ�ͻ�����ҵĵ�ȯ
		int GetMyPlayerSilver() const;
		//  ��ȡ�ͻ�����ҵĿ���
		const std::vector<CardType> & GetMyPlayerCardType() const;
		//  ��ȡ�ͻ�����ҵ�����
		const std::string & GetMyPlayerName() const;
		//  �Ե�ǰ�������ò�������������ģʽ
		void Move(int step);
		//  ��ָ���������ò��������������߽������¼�
		void JustMove(int ID, int step);
		//  ���õ�ǰ���һ���ƶ���Ŀ��λ�úͳ���
		void SetTargetPosition(XMFLOAT3 pos);
		//  �жϵ�ǰ�����Ƿ���Ҫ������һ��������
		bool IsUpdateNextStep() const;
		//  �жϵ�ǰ�����Ƿ���Ҫ�������ߺ󼤻��¼������緵��Ϊtrue���ͻ��Զ���λ����һ��ֻ����false��
		bool IsNeedDealWithEventAfterMove();
		
	
	//  ָ����ɫ����
	public:
		//  ����ָ�����λ��
		void SetPlayerPosition(int ID, XMFLOAT3 pos);
		//  ����ָ����ҵ�������Ϣ
		void SetPlayerInterests(int ID, int interests);
		//  �ı�ָ�������Ǯ
		void AddGold(int ID, int money);
		//  �ı�ָ�������ȯ
		void AddSilver(int ID, int silver);
		//  ����ָ�������·����
		void SetBlockIndex(int ID, POINT point);
		//  ָ������ת��Ϊ�˴���սǴ��ĳ�������,������һ����λ�ã�
		void TurnBack(int ID, XMFLOAT3 backStep);
		//  �ı�ָ����������(rateΪǧ�ֱȣ�1 ��ʾ 0.1%)[����������0 - 2%]
		void AddRate(int ID, int rate);
		//  ָ�����ﶪ���ض��Ŀ�
		CardType ThrowCard(int ID, int cardIndex);
		//  ָ�����ﶪ����һ�ſ�
		CardType ThrowCard(int ID);
		//  ָ��������һ�ſ�(�����д����)
		void ObtainCard(int ID, CardType card);
		//  ��ȡָ������ĵ�·������ʧ�ܷ��أ�-1��-1����
		POINT GetBlockIndex(int ID) const;
		//  ��ȡָ��������ƶ���������������ʧ�ܷ���0��
		int GetDirection(int ID) const;
		//  ��ȡָ���������ʵλ��
		XMFLOAT3 GetPlayerRealPosition(int ID) const;
		//  ��ȡָ�������������Ϣ
		int GetPlayerInterests(int ID) const;
		//  ��ȡָ������ӵ�еĿ�Ƭ��ö�٣�
		const std::vector<CardType> & GetPlayerCardType(int ID) const;
		//  ����ָ�����һ���ƶ���Ŀ��λ�úͳ���
		void SetTargetPosition(int ID, XMFLOAT3 pos);
		//  �ж�ָ�������Ƿ���Ҫ������һ��������
		bool IsUpdateNextStep(int ID) const;
		//  �ж�ָ�������Ƿ���Ҫ�������ߺ󼤻��¼������緵��Ϊtrue���ͻ��Զ���λ����һ��ֻ����false��
		bool IsNeedDealWithEventAfterMove(int ID);
		//  �жϵ�ǰ�Ƿ����������վ��ͬһ�񣨲���Ϊ���ƶ�������ID������ֵΪ������һ��
		//  �����ID������û����ҷ��� PLAYER_ID_DEFAULT��
		int IsStandOnSamePosition(int movePlayerID) const;

	//  ����޸ĺ���
	public:
		//  ��ǰ�����ĳһλ���ƽ����Ǯ
		void AverageGold(int targetID);
		//  ��ǰ��һ�ȡ���������Ϣ�������Լ���
		void RobInterests();
		//  ������ǰ��������5�ſ���
		void SellAllCard();
		//  ���㵱ǰ���������Ϣ����ϼ���
		void CalculateCurrPlayerInterests();
		//  ��ǰ�����������ȡǮ������Ϣת��Ϊ��Ǯ����ϼ���
		void ChangeInterestsToMoney();
		//  ��ָͣ�������ƶ�
		void StopPlayer(int ID);

	private:
		//  �����ָ��������������
		XMFLOAT4X4 CaculateWorldMatrix(int ID);
		//  �Զ����������ƶ�
		void Moving(int ID, float dt);
		//  ͨ��ID��Ѱ�Ҷ�Ӧ�������������е�������ʧ�ܷ���-1��
		int FindPlayerByID(int ID) const;

	private:
		std::vector<PLAYER>		m_Players;			//  ��������[�߼������4�����]		
		int						m_CurrPlyaerIndex;	//  ��ǰ��������������	
		std::map<std::string, Sprite3D> m_SPlayer;	//  �������� <--> 3D���� ��ϣ��
		int						m_MyPlayerID;		//  ��ǰ�ͻ��˵����ID��
	};


} //  ���������ռ�

#endif // !PLAYER_MANAGER_H_

//*************************************************************************************
//	ʹ�ÿ�ܣ�
//  ��Ҫ�������������2������������������͵�ͼ�����Ϣ������
//
////  ���ݵ�ǰ��·�����뷽��ȷ������ĵ�ǰ������������һ������������
//void GameApp::UpDatePlayerPosition(int ID)
//{
//	//  ������ҵ�ǰλ��
//	POINT blockIndex = m_PlayerManager.GetBlockIndex(ID);
//	XMFLOAT3 pos = m_StreetManager.BlockTransFloat3Get(blockIndex);
//	m_PlayerManager.SetPlayerPosition(ID, pos);
//
//	//  ���������һ��λ��
//	POINT nextPosIndex = m_StreetManager.BlockNextGet(
//		m_PlayerManager.GetBlockIndex(ID),
//		m_PlayerManager.GetDirection(ID));
//	XMFLOAT3 nextPos = m_StreetManager.BlockTransFloat3Get(nextPosIndex);
//	m_PlayerManager.SetTargetPosition(ID, nextPos);
//}
//
////  ����ƶ����ԭ���ĵ�·��������Ϊ��һ���Ľֵ�����
//void GameApp::UpDatePlayerBlockIndex(int ID)
//{
//	POINT blockIndex = m_StreetManager.BlockNextGet(
//		m_PlayerManager.GetBlockIndex(ID),
//		m_PlayerManager.GetDirection(ID)
//	);
//	m_PlayerManager.SetBlockIndex(ID, blockIndex);
//}
//
//	��������󣬽�����Ϸ����ǰ�������������ȡ�Լ���ID��
//	m_PlayerManager.InitPlayer(����, ��ʼ���, ��ʼ��ȯ, �Լ���ID��);
//	���Ե�ʱ�������Init()�м��룺
//
////  ��������
// m_PlayerManager.InitPlayer(4, 1000, 100, Rich::PLAYER_ID_ONE);  
////����������������λ��
//UpDatePlayerPosition(Rich::PLAYER_ID_ONE);						
//UpDatePlayerPosition(Rich::PLAYER_ID_TWO);
//UpDatePlayerPosition(Rich::PLAYER_ID_THREE);
//UpDatePlayerPosition(Rich::PLAYER_ID_FOUR);
//
//  ��Ϣ�����ܣ�����UpdateScreen�����У�
//
//int ID[4] = {
//Rich::PLAYER_ID_ONE,
//Rich::PLAYER_ID_TWO,
//Rich::PLAYER_ID_THREE,
//Rich::PLAYER_ID_FOUR };
//
//for (int i = 0; i < 4; i++)
//{
//	//  ��������һ����Ĳ���
//	if (m_PlayerManager.IsUpdateNextStep(ID[i]) == true)
//	{
//		UpDatePlayerBlockIndex(ID[i]);
//		UpDatePlayerPosition(ID[i]);
//
//		int playerID = m_PlayerManager.IsStandOnSamePosition(ID[i]);
//		//  �ƶ���λ���Ѿ������
//		if (playerID != Rich::PLAYER_ID_DEFAULT)
//		{
//			m_PlayerManager.JustMove(playerID, 1);
//		}
//	}
//
//	//  ��������һ�κ�Ĳ���
//	if (m_PlayerManager.IsNeedDealWithEventAfterMove(ID[i]) == true)
//	{
//		::MessageBox(NULL, L"�������ߺ��¼�", L"��Ϣ", 0);
//	}
//}
//
////  ������������
//m_PlayerManager.Update(dt);
//
////  �������� DrawScene()
//m_PlayerManager.Draw();
//*************************************************************************************
