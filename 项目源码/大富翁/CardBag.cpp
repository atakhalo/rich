//***************************************************************************************
//	���֣�			���̿����� * Դ�ļ�
//	���ߣ�			¬��͢
//  �������ڣ�		2017-8-24
//  ����������ڣ�	2017-8-24
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
			::MessageBox(nullptr, L"�޷��� : ����������Ϣ�ļ�", L"����", 0);
			return E_FAIL;
		}

		Rich::CARD oneCard;
		std::string ignore;
		int CardNum = 0;

		//  ��ȡ��Ƭ����
		in >> ignore;
		in >> CardNum;

		//  ��ȡ��Ƭ������Ϣ
		//  ���֣�Ʒ�ʣ��ㄻ���Ƿ�UI
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
			::MessageBox(nullptr, L"������� : �޷���ȷ������Ϣ���Ƿ��ĵ���ʽ����", L"����", 0);
			return E_FAIL;
		}

		return S_OK;
	}

	const CARD & RichCardManager::GetCardByEnum(CardType type)
	{
		int index = static_cast<int>(type);
		return m_CardBag[index];
	}

} //  ���������ռ����
