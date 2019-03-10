#pragma once

#include "d3dUtil.h"
#include <unordered_map>
#include <queue>

#define MAX_LIGHTS 16

template<class T>
struct Larger
{	
	constexpr bool operator()(const T& _Left, const T& _Right) const
	{	// apply operator> to operands
		return (_Left > _Right);
	}
};


class LightManager {
public:
	// 初始化Light必备组件
	static void Initialize(ID3DX11Effect* pFX);
	// 获取灯光管理者
	static LightManager* GetLightManager();
	// 释放灯光管理者
	static void Release();
	// 清空所有灯光
	void ClearLight();
	// 可选类型为：DirectionalLight、PointLight、SpotLight
	// 若找不到lightName,则会新建一个灯光；若存在名为lightName的灯光，则会修改灯光
	template<class LightType>
	void SetLight(LPCWSTR lightName, const LightType& light);

	// 可选类型为：DirectionalLight、PointLight、SpotLight
	// 若找不到lightName，则会返回一个空的灯光，找到则直接返回
	template<class LightType>
	LightType GetLight(LPCWSTR lightName);

	// 可选类型为：DirectionalLight、PointLight、SpotLight
	// 删除一个灯光
	template<class LightType>
	void DeleteLight(LPCWSTR lightName);
	
	// 可选类型为：DirectionalLight、PointLight、SpotLight
	// 设置对应灯光的开关
	template<class LightType>
	void Control(LPCWSTR lightName, bool openState);

	
	
private:
	LightManager();
	~LightManager();
	static LightManager*											m_LightManager;			// 单例
	static ID3DX11Effect*											m_pFX;					// Effects


	ID3DX11EffectVariable*											m_pFXDirLight;			// 直射光HLSL数组
	ID3DX11EffectVariable*											m_pFXPointLight;		// 点光HLSL数组
	ID3DX11EffectVariable*											m_pFXSpotLight;			// 聚光灯HLSL数组
	ID3DX11EffectVariable*											m_pFXDirNums;			// 直射光数目
	ID3DX11EffectVariable*											m_pFXPointNums;			// 点光数目
	ID3DX11EffectVariable*											m_pFXSpotNums;			// 聚光灯数目

	std::unordered_map<std::wstring, std::pair<DirectionalLight, int>>	m_DirLight;				// 直射光
	std::unordered_map<std::wstring, std::pair<PointLight, int>>		m_PointLight;		// 点光
	std::unordered_map<std::wstring, std::pair<SpotLight, int>>			m_SpotLight;			// 聚光灯
	
	std::priority_queue<int, std::vector<int>, Larger<int>>			m_EmptyFXDirLight;		// 标记被删掉的直射光位置
	std::priority_queue<int, std::vector<int>, Larger<int>>			m_EmptyFXPointLight;	// 标记被删掉的点光位置
	std::priority_queue<int, std::vector<int>, Larger<int>>			m_EmptyFXSpotLight;		// 标记被删掉的聚光灯位置
};


