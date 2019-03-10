//ver 1.0 by 周欣晖    
//2017/8/25      
//注:本文件依赖众多文件资源，请更新OBJ，Texture,Material
//本文件提供街道功能区枚举，房屋类型枚举
//街道类：
//载入：m_StreetManager.initStreet("Street/街区.txt");
//提供获取过路费的函数
//提供设置路障的函数
//提供一次移动的停止点（被路障挡住或完成移动)
//提供获取地块的信息的多个接口()
//提供改变地块信息的多个接口
//提供改变胡同地价，拆除胡同房子，升胡同房子等级
//
//绘制：
//需初始化:
//m_StreetManager.Initialize();
//需确定原点，默认零点
//坐标倍数，默认1倍
//模型倍数，默认1倍;
//m_StreetManager.originalPointSet({ 45.0f,0.0f,-55.0f });
//m_StreetManager.scaleFactorSet(10.0f);
//	m_StreetManager.ModelScaleSet(0.03f);
//设置拥有者（默认0），设置房屋类型（土地：NOHOUSE,默认土地），房屋等级（默认0）
//	m_StreetManager.HouseOwnerSet({ 0,0 }, 1);									//索引为0，0的地块被1买下
//m_StreetManager.HouseTypeSet({ 0,0 }, Rich::COTTAGE);		//索引为0，0的房屋类型设为别墅
//m_StreetManager.HouseLevelSet({ 0,0 }, 1);											//索引为0，0的地块设为一级
//m_StreetManager.BarrierSet({ 1,0 },true);												//在索引为1，0的地块设路障
//m_StreetManager.HutongDestroy(1);														//摧毁1号街道
//m_StreetManager.HutongLevelChange(0, 2);										//升级0号街道房子2级
//m_StreetManager.Rent({ 1,1 });																	//索引为0，0的地块的过路费
//需在UpdateScene()中调用m_StreetManager.UpdateScene();
//在DrawScene()中调用m_StreetManager.Draw();

//ver 1.0.0.0.0.0.0.1          by 周欣晖
//BlockNextGet函数和BlockNextPosGet函数
//在size那里加了强转
//if (index.x + 1 >= static_cast<LONG>( m_Street.size())) 本来的== 改为 >=
//改了模型载入，请更新模型和贴图

//ver 1.0.0.0.0.0.0.2          by 周欣晖
//void HutongValueChange(int index, int factor);第二个形参由float 改为int,赋值时*o.1f,并收缩在0.8,1.5间;
//BLOCK结构体设为public;

//ver 1.0.0.0.0.0.0.3			by周欣晖
//CARD枚举名改为FUNCTYPE_CARD
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
		FLAT,						//五层
		COTTAGE			//三层	
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

		//载入街区txt
		void initStreet(std::string txtName);

		//设置路障
		void BarrierSet(POINT index, bool isBarrier);


		//拆除一条巷子的房子
		void HutongDestroy(int index);
		//改变一条巷子的房子等级
		void HutongLevelChange(int index, int dLevel);
		//改变地价
		void HutongValueChange(int index, int factor);
		
		//设置房子等级
		void HouseLevelSet(POINT index, int toLevel);
		//设置房子类型
		void HouseTypeSet(POINT index, HOUSETYPE houseType);
		//房子升级
		void HouseLevelAdd(POINT index, int dLevel);
		//设置地块主人
		void HouseOwnerSet(POINT index, int playerID);
		//拆除房子
		void HouseDestroy(POINT index);
		//房子升满级
		void HouseLevelMax(POINT index);
		
		//设置坐标原点
		void originalPointSet(XMFLOAT3 originalPoint);
		//设置坐标倍数
		void scaleFactorSet(float scaleFactor);
		//设置模型倍数
		void ModelScaleSet(float scale);
		
		//获取街道总数
		int StreetSize() const; 
		//获取单条街道的大小
		int SingleStreetSize(int index) const;
		
		//获取当前地块全部信息
		BLOCK BlockGet(POINT index) const;
		//获取当前地块的租金
		int Rent(POINT index) const;
		//获取当前地块等级
		int HouseLevelGet(POINT index)  const;
		//获取房屋类型
		HOUSETYPE HouseTypeGet(POINT index) const;
		//获取地块主人
		int HouseOwnerGet(POINT index) const;
		//判断是否有路障
		bool BarrierGet(POINT index) const;
		//获取功能区类型
		FUNCTYPE FuncTypeGet(POINT index) const;

		//获取下一地块的索引
		POINT BlockNextGet(POINT index, int dir) const;


		//获取地块的位置Float3
		XMFLOAT3 BlockTransFloat3Get(POINT index) const;

		//初始化OBJ和纹理（房屋 + 边框 + 路障）
		void Initialize();
		// 绘制模型（房屋 + 边框 + 路障）
		void Draw();
		//更新矩阵BUFFER
		void UpdateScene();

	private:
		//获取地块的位置矩阵
		XMFLOAT4X4 BlockTransMatrixGet(POINT index) const;
		//获取下一地块位置
		POINT BlockNextPosGet(POINT index,int dir) const;
		//获取当前地块位置
		POINT BlockPosGet(POINT index) const;
		//根据方向走几步，返回有路障的索引或完成行走的索引
		POINT MoveTo(POINT index, int dir, int stepNum) const;
	private:
		std::vector<std::vector<BLOCK>> m_Street;				//街道
		std::map<std::string, Sprite3D> m_Models;					//模型

		XMFLOAT3 m_originalPoint;											//坐标原点，默认(0,0,0)
		float m_scaleFactor;																//坐标缩放倍数,默认一倍
		
		float m_ModelScale;																//模型缩放倍数	默认一倍
	};
}