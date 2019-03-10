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
	// ��ʼ��Light�ر����
	static void Initialize(ID3DX11Effect* pFX);
	// ��ȡ�ƹ������
	static LightManager* GetLightManager();
	// �ͷŵƹ������
	static void Release();
	// ������еƹ�
	void ClearLight();
	// ��ѡ����Ϊ��DirectionalLight��PointLight��SpotLight
	// ���Ҳ���lightName,����½�һ���ƹ⣻��������ΪlightName�ĵƹ⣬����޸ĵƹ�
	template<class LightType>
	void SetLight(LPCWSTR lightName, const LightType& light);

	// ��ѡ����Ϊ��DirectionalLight��PointLight��SpotLight
	// ���Ҳ���lightName����᷵��һ���յĵƹ⣬�ҵ���ֱ�ӷ���
	template<class LightType>
	LightType GetLight(LPCWSTR lightName);

	// ��ѡ����Ϊ��DirectionalLight��PointLight��SpotLight
	// ɾ��һ���ƹ�
	template<class LightType>
	void DeleteLight(LPCWSTR lightName);
	
	// ��ѡ����Ϊ��DirectionalLight��PointLight��SpotLight
	// ���ö�Ӧ�ƹ�Ŀ���
	template<class LightType>
	void Control(LPCWSTR lightName, bool openState);

	
	
private:
	LightManager();
	~LightManager();
	static LightManager*											m_LightManager;			// ����
	static ID3DX11Effect*											m_pFX;					// Effects


	ID3DX11EffectVariable*											m_pFXDirLight;			// ֱ���HLSL����
	ID3DX11EffectVariable*											m_pFXPointLight;		// ���HLSL����
	ID3DX11EffectVariable*											m_pFXSpotLight;			// �۹��HLSL����
	ID3DX11EffectVariable*											m_pFXDirNums;			// ֱ�����Ŀ
	ID3DX11EffectVariable*											m_pFXPointNums;			// �����Ŀ
	ID3DX11EffectVariable*											m_pFXSpotNums;			// �۹����Ŀ

	std::unordered_map<std::wstring, std::pair<DirectionalLight, int>>	m_DirLight;				// ֱ���
	std::unordered_map<std::wstring, std::pair<PointLight, int>>		m_PointLight;		// ���
	std::unordered_map<std::wstring, std::pair<SpotLight, int>>			m_SpotLight;			// �۹��
	
	std::priority_queue<int, std::vector<int>, Larger<int>>			m_EmptyFXDirLight;		// ��Ǳ�ɾ����ֱ���λ��
	std::priority_queue<int, std::vector<int>, Larger<int>>			m_EmptyFXPointLight;	// ��Ǳ�ɾ���ĵ��λ��
	std::priority_queue<int, std::vector<int>, Larger<int>>			m_EmptyFXSpotLight;		// ��Ǳ�ɾ���ľ۹��λ��
};


