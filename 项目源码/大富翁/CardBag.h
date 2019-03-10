//***************************************************************************************
//	���֣�			���̿�����
//	���ߣ�			¬��͢
//  �������ڣ�		2017-8-24
//  ����������ڣ�	2017-8-24
//  
//  ��Ҫ��ͷ�ļ���
//		XUtility.h
//
//	������
//		����ö�ٱ������Ƕ�Ӧ�����������������Ե���ʵ�֣�����ͨ����̬������ȡ���ƻ�����Ϣ��
//
//  ʹ��˵����
//		1���ڳ�ʼ��Init�����뿨�ƻ�����Ϣ���������뺯�� ReadCardFromFile()��
//		2��ͨ����ȡ�ĵ���������Ϣ����Ϣ��ʽ���£�
//				���� num
//				���� Ʒ�� ��ȯ UI
//				......(��Ƭ)
//		3������ GetCardByEnum() ����ͨ��ö��ֵ��ȡ���ƻ�����Ϣ
//***************************************************************************************

#ifndef CARD_BAG_H_
#define CARD_BAG_H_

#include "XUtility.h"

namespace Rich
{

	//  ����ö�ٱ�
	enum CardType
	{
		CARD_NONE,					//  �տ�
		CARD_STEP_ONE,				//  һ����
		CARD_STEP_TWO,				//  ������
		CARD_STEP_THREE,			//  ������
		CARD_STEP_FOUR,				//  �Ĳ���
		CARD_STEP_FIVE,				//  �岽��
		CARD_STEP_SIX,				//  ������
		CARD_TURN_BACK,				//  ת��
		CARD_STAY,					//  ԭ�ؿ�
		CARD_ROULETTE,				//  ���̿�
		CARD_BARRIER,				//  ·�Ͽ�
		CARD_DESTINY,				//  ���˿�
		CARD_GOLD,					//  ��Ǯ��
		CARD_UPGRADE_HOUSE,			//  ������
		CARD_APPREICATE_STREET,		//  ��ֵ��
		CARD_DISMANTLE,				//  ��Ǩ��
		CARD_DEPREICATE_STREET,		//  ��ֵ��
		CARD_EXILE,					//  ����
		CARD_CHIEF,					//  С͵��
		CARD_DECREASE_INTERESTS,	//  ��Ϣ��
		CARD_INCREASE_INTERESTS,	//  ��Ϣ��
		CARD_PURCASE,				//  ���ÿ�
		CARD_AVERAGE_GOLD,			//  ��ƶ��
		CARD_MONSTER,				//  ���޿�
		CARD_RUBAN,					//  ³�࿨
		CARD_ROB_TAX,				//  ��˰��
		CARD_DEVIL,					//  ��ħ��
		CARD_LAND,					//  ���ؿ�
		CARD_SAMSALA				//  �ֻؿ� 
	};

	//  ��Ƭ�ṹ��
	struct CARD
	{
	public:
		CARD();
		~CARD() = default;

	public:
		std::string name;	//  ��Ƭ����
		int weight;			//  ��ƬƷ��
		int silver;			//  �������
		bool isPopUI;		//  ʹ��ʱ�Ƿ���Ҫ����UI
	};

	//  ������������
	//  ���̿�Ƭ������
	class RichCardManager
	{
		//  ��������
	private:
		RichCardManager() = default;
		~RichCardManager() = default;

		static RichCardManager* m_pInstance;

	public:
		//  ��õ�������ָ��
		static RichCardManager* GetInstance();
		//  ���ٵ�������
		static void DestroyInstance();

	public:
		//  ��ʼ�����裺���ļ������뿨Ƭ������Ϣ
		HRESULT ReadCardFromFile(LPCSTR fileName);
		//  ͨ��ö�ٻ�ÿ�Ƭ��Ϣ
		const CARD & GetCardByEnum(CardType type);

	private:
		std::vector<CARD> m_CardBag;	//  ���̿�Ƭ��
	};


} //  ���������ռ����

#endif // !CARD_BAG_H_
