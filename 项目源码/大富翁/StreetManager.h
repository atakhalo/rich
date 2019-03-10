//ver 1.0 by ������    
//2017/8/25      
//ע:���ļ������ڶ��ļ���Դ�������OBJ��Texture,Material
//���ļ��ṩ�ֵ�������ö�٣���������ö��
//�ֵ��ࣺ
//���룺m_StreetManager.initStreet("Street/����.txt");
//�ṩ��ȡ��·�ѵĺ���
//�ṩ����·�ϵĺ���
//�ṩһ���ƶ���ֹͣ�㣨��·�ϵ�ס������ƶ�)
//�ṩ��ȡ�ؿ����Ϣ�Ķ���ӿ�()
//�ṩ�ı�ؿ���Ϣ�Ķ���ӿ�
//�ṩ�ı��ͬ�ؼۣ������ͬ���ӣ�����ͬ���ӵȼ�
//
//���ƣ�
//���ʼ��:
//m_StreetManager.Initialize();
//��ȷ��ԭ�㣬Ĭ�����
//���걶����Ĭ��1��
//ģ�ͱ�����Ĭ��1��;
//m_StreetManager.originalPointSet({ 45.0f,0.0f,-55.0f });
//m_StreetManager.scaleFactorSet(10.0f);
//	m_StreetManager.ModelScaleSet(0.03f);
//����ӵ���ߣ�Ĭ��0�������÷������ͣ����أ�NOHOUSE,Ĭ�����أ������ݵȼ���Ĭ��0��
//	m_StreetManager.HouseOwnerSet({ 0,0 }, 1);									//����Ϊ0��0�ĵؿ鱻1����
//m_StreetManager.HouseTypeSet({ 0,0 }, Rich::COTTAGE);		//����Ϊ0��0�ķ���������Ϊ����
//m_StreetManager.HouseLevelSet({ 0,0 }, 1);											//����Ϊ0��0�ĵؿ���Ϊһ��
//m_StreetManager.BarrierSet({ 1,0 },true);												//������Ϊ1��0�ĵؿ���·��
//m_StreetManager.HutongDestroy(1);														//�ݻ�1�Žֵ�
//m_StreetManager.HutongLevelChange(0, 2);										//����0�Žֵ�����2��
//m_StreetManager.Rent({ 1,1 });																	//����Ϊ0��0�ĵؿ�Ĺ�·��
//����UpdateScene()�е���m_StreetManager.UpdateScene();
//��DrawScene()�е���m_StreetManager.Draw();

//ver 1.0.0.0.0.0.0.1          by ������
//BlockNextGet������BlockNextPosGet����
//��size�������ǿת
//if (index.x + 1 >= static_cast<LONG>( m_Street.size())) ������== ��Ϊ >=
//����ģ�����룬�����ģ�ͺ���ͼ

//ver 1.0.0.0.0.0.0.2          by ������
//void HutongValueChange(int index, int factor);�ڶ����β���float ��Ϊint,��ֵʱ*o.1f,��������0.8,1.5��;
//BLOCK�ṹ����Ϊpublic;

//ver 1.0.0.0.0.0.0.3			by������
//CARDö������ΪFUNCTYPE_CARD
#pragma once
#include <fstream>
#include <Windows.h>
#include <string>
#include <vector>
#include <map>
#include "Sprite3D.h"
namespace Rich
{
	enum FUNCTYPE
	{
		 HOUSE,
		SILVER_20,
		 SILVER_30,
		 SILVER_50,
		 STEAM,
		 MARKET,
		 BANK,
		 DESTINY,
		 FUNCTYPE_CARD
	};
	
	enum HOUSETYPE
	{
		NOHOUSE,
		FLAT,						//���
		COTTAGE			//����	
	};

	class StreetManager
	{

	public:
		struct BLOCK
		{
			POINT Pos; 
			POINT Offset; 
			bool isBarrier; 
			int HouseLevel; 
			HOUSETYPE HouseType; 
			float PriceFactor; 
			int owner; 
			FUNCTYPE FuncType; 
		};

		StreetManager();
		~StreetManager();

		//�������txt
		void initStreet(std::string txtName);

		//����·��
		void BarrierSet(POINT index, bool isBarrier);


		//���һ�����ӵķ���
		void HutongDestroy(int index);
		//�ı�һ�����ӵķ��ӵȼ�
		void HutongLevelChange(int index, int dLevel);
		//�ı�ؼ�
		void HutongValueChange(int index, int factor);
		
		//���÷��ӵȼ�
		void HouseLevelSet(POINT index, int toLevel);
		//���÷�������
		void HouseTypeSet(POINT index, HOUSETYPE houseType);
		//��������
		void HouseLevelAdd(POINT index, int dLevel);
		//���õؿ�����
		void HouseOwnerSet(POINT index, int playerID);
		//�������
		void HouseDestroy(POINT index);
		//����������
		void HouseLevelMax(POINT index);
		
		//��������ԭ��
		void originalPointSet(XMFLOAT3 originalPoint);
		//�������걶��
		void scaleFactorSet(float scaleFactor);
		//����ģ�ͱ���
		void ModelScaleSet(float scale);
		
		//��ȡ�ֵ�����
		int StreetSize() const; 
		//��ȡ�����ֵ��Ĵ�С
		int SingleStreetSize(int index) const;
		
		//��ȡ��ǰ�ؿ�ȫ����Ϣ
		BLOCK BlockGet(POINT index) const;
		//��ȡ��ǰ�ؿ�����
		int Rent(POINT index) const;
		//��ȡ��ǰ�ؿ�ȼ�
		int HouseLevelGet(POINT index)  const;
		//��ȡ��������
		HOUSETYPE HouseTypeGet(POINT index) const;
		//��ȡ�ؿ�����
		int HouseOwnerGet(POINT index) const;
		//�ж��Ƿ���·��
		bool BarrierGet(POINT index) const;
		//��ȡ����������
		FUNCTYPE FuncTypeGet(POINT index) const;

		//��ȡ��һ�ؿ������
		POINT BlockNextGet(POINT index, int dir) const;


		//��ȡ�ؿ��λ��Float3
		XMFLOAT3 BlockTransFloat3Get(POINT index) const;

		//��ʼ��OBJ���������� + �߿� + ·�ϣ�
		void Initialize();
		// ����ģ�ͣ����� + �߿� + ·�ϣ�
		void Draw();
		//���¾���BUFFER
		void UpdateScene();

	private:
		//��ȡ�ؿ��λ�þ���
		XMFLOAT4X4 BlockTransMatrixGet(POINT index) const;
		//��ȡ��һ�ؿ�λ��
		POINT BlockNextPosGet(POINT index,int dir) const;
		//��ȡ��ǰ�ؿ�λ��
		POINT BlockPosGet(POINT index) const;
		//���ݷ����߼�����������·�ϵ�������������ߵ�����
		POINT MoveTo(POINT index, int dir, int stepNum) const;
	private:
		std::vector<std::vector<BLOCK>> m_Street;				//�ֵ�
		std::map<std::string, Sprite3D> m_Models;					//ģ��

		XMFLOAT3 m_originalPoint;											//����ԭ�㣬Ĭ��(0,0,0)
		float m_scaleFactor;																//�������ű���,Ĭ��һ��
		
		float m_ModelScale;																//ģ�����ű���	Ĭ��һ��
	};
}