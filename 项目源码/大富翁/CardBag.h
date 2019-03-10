//***************************************************************************************
//	名字：			大富翁卡包类
//	作者：			卢俊廷
//  创建日期：		2017-8-24
//  最近更新日期：	2017-8-24
//  
//  需要的头文件：
//		XUtility.h
//
//	简述：
//		卡牌枚举本质上是对应卡包的索引。卡包以单例实现，可以通过静态方法获取卡牌基本信息。
//
//  使用说明：
//		1、在初始化Init中载入卡牌基本信息。调用载入函数 ReadCardFromFile()。
//		2、通过读取文档来载入信息。信息格式如下：
//				数量 num
//				名字 品质 点券 UI
//				......(卡片)
//		3、调用 GetCardByEnum() 可以通过枚举值获取卡牌基本信息
//***************************************************************************************

#ifndef CARD_BAG_H_
#define CARD_BAG_H_

#include "XUtility.h"

namespace Rich
{

	//  卡牌枚举表
	enum CardType
	{
		CARD_NONE,					//  空卡
		CARD_STEP_ONE,				//  一步卡
		CARD_STEP_TWO,				//  二步卡
		CARD_STEP_THREE,			//  三步卡
		CARD_STEP_FOUR,				//  四步卡
		CARD_STEP_FIVE,				//  五步卡
		CARD_STEP_SIX,				//  六步卡
		CARD_TURN_BACK,				//  转向卡
		CARD_STAY,					//  原地卡
		CARD_ROULETTE,				//  轮盘卡
		CARD_BARRIER,				//  路障卡
		CARD_DESTINY,				//  命运卡
		CARD_GOLD,					//  金钱卡
		CARD_UPGRADE_HOUSE,			//  升级卡
		CARD_APPREICATE_STREET,		//  升值卡
		CARD_DISMANTLE,				//  拆迁卡
		CARD_DEPREICATE_STREET,		//  贬值卡
		CARD_EXILE,					//  放逐卡
		CARD_CHIEF,					//  小偷卡
		CARD_DECREASE_INTERESTS,	//  减息卡
		CARD_INCREASE_INTERESTS,	//  加息卡
		CARD_PURCASE,				//  购置卡
		CARD_AVERAGE_GOLD,			//  均贫卡
		CARD_MONSTER,				//  怪兽卡
		CARD_RUBAN,					//  鲁班卡
		CARD_ROB_TAX,				//  抢税卡
		CARD_DEVIL,					//  恶魔卡
		CARD_LAND,					//  土地卡
		CARD_SAMSALA				//  轮回卡 
	};

	//  卡片结构体
	struct CARD
	{
	public:
		CARD();
		~CARD() = default;

	public:
		std::string name;	//  卡片名字
		int weight;			//  卡片品质
		int silver;			//  点卷消耗
		bool isPopUI;		//  使用时是否需要弹出UI
	};

	//  卡包——单例
	//  大富翁卡片单例类
	class RichCardManager
	{
		//  单例特征
	private:
		RichCardManager() = default;
		~RichCardManager() = default;

		static RichCardManager* m_pInstance;

	public:
		//  获得单例对象指针
		static RichCardManager* GetInstance();
		//  销毁单例对象
		static void DestroyInstance();

	public:
		//  初始化步骤：从文件中载入卡片基本信息
		HRESULT ReadCardFromFile(LPCSTR fileName);
		//  通过枚举获得卡片信息
		const CARD & GetCardByEnum(CardType type);

	private:
		std::vector<CARD> m_CardBag;	//  大富翁卡片包
	};


} //  大富翁命名空间结束

#endif // !CARD_BAG_H_
