//***************************************************************************************
//	名字：			大富翁卡包类 * 源文件
//	作者：			卢俊廷
//  创建日期：		2017-8-24
//  最近更新日期：	2017-8-24
//***************************************************************************************


#include "CardBag.h"


namespace Rich
{
	CARD::CARD() : name(std::string("(None)")),weight(0),silver(0),isPopUI(false)
	{
	}

	/*---------------------------------------------------------------------------------*/

	RichCardManager* RichCardManager::m_pInstance = nullptr;

	RichCardManager * RichCardManager::GetInstance()
	{
		if (m_pInstance == nullptr)
			m_pInstance = new RichCardManager();

		return m_pInstance;
	}

	void RichCardManager::DestroyInstance()
	{
		if (m_pInstance)
		{
			delete m_pInstance;
			m_pInstance = nullptr;
		}
	}

	HRESULT RichCardManager::ReadCardFromFile(LPCSTR fileName)
	{
		std::ifstream in(fileName);
		if (!in)
		{
			::MessageBox(nullptr, L"无法打开 : 基本卡牌信息文件", L"错误", 0);
			return E_FAIL;
		}

		Rich::CARD oneCard;
		std::string ignore;
		int CardNum = 0;

		//  读取卡片数量
		in >> ignore;
		in >> CardNum;

		//  读取卡片基本信息
		//  名字，品质，点劵，是否UI
		in >> ignore; 
		in >> ignore; 
		in >> ignore; 
		in >> ignore;

		for (int i = 0; i < CardNum; ++i)
		{
			in >> oneCard.name;
			in >> oneCard.weight;
			in >> oneCard.silver;
			in >> oneCard.isPopUI;
			m_CardBag.push_back(oneCard);
		}

		in.close();

		if (m_CardBag.size() <= 0)
		{
			::MessageBox(nullptr, L"读入错误 : 无法正确读入信息，是否文档格式出错。", L"错误", 0);
			return E_FAIL;
		}

		return S_OK;
	}

	const CARD & RichCardManager::GetCardByEnum(CardType type)
	{
		int index = static_cast<int>(type);
		return m_CardBag[index];
	}

} //  大富翁命名空间结束
