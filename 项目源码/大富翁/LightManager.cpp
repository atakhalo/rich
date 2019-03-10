#include "LightManager.h"
#include "XUtility.h"

LightManager* LightManager::m_LightManager = nullptr;
ID3DX11Effect* LightManager::m_pFX = nullptr;


LightManager::LightManager() :
	m_pFXDirLight(nullptr), m_pFXPointLight(nullptr),
	m_pFXSpotLight(nullptr), m_pFXDirNums(nullptr),
	m_pFXPointNums(nullptr), m_pFXSpotNums(nullptr)
{
	m_pFXDirLight = m_pFX->GetVariableByName("gDirLight");
	m_pFXPointLight = m_pFX->GetVariableByName("gPointLight");
	m_pFXSpotLight = m_pFX->GetVariableByName("gSpotLight");
	m_pFXDirNums = m_pFX->GetVariableByName("gDirNums");
	m_pFXPointNums = m_pFX->GetVariableByName("gPointNums");
	m_pFXSpotNums = m_pFX->GetVariableByName("gSpotNums");
}

LightManager::~LightManager()
{
}

void LightManager::Initialize(ID3DX11Effect* pFX)
{
	if (!m_pFX)
	{
		m_pFX = pFX;
	}	
}

LightManager * LightManager::GetLightManager()
{
	if (!m_pFX)
		return nullptr;
	if (!m_LightManager)
		m_LightManager = new LightManager;
	return m_LightManager;
}

void LightManager::Release()
{
	if (m_LightManager)
	{
		delete m_LightManager;
		m_LightManager = nullptr;
	}
}

void LightManager::ClearLight()
{
	DirectionalLight dlight;
	PointLight plight;
	SpotLight slight;
	for (int i = 0; i < MAX_LIGHTS; ++i)
	{
		m_pFXDirLight->GetElement(i)->SetRawValue(&dlight, 0, sizeof(DirectionalLight));
		m_pFXPointLight->GetElement(i)->SetRawValue(&plight, 0, sizeof(PointLight));
		m_pFXSpotLight->GetElement(i)->SetRawValue(&slight, 0, sizeof(SpotLight));
	}
	int zero = 0;
	m_pFXDirNums->SetRawValue(&zero, 0, sizeof(int));
	m_pFXPointNums->SetRawValue(&zero, 0, sizeof(int));
	m_pFXSpotNums->SetRawValue(&zero, 0, sizeof(int));
}

// *********************模板部分********************
// ****************设置/添加灯光部分****************
template<>
void LightManager::SetLight<DirectionalLight>(LPCWSTR lightName, const DirectionalLight& light)
{
	if (m_DirLight.size() >= MAX_LIGHTS)			// 最多允许16盏
		return;
	auto it = m_DirLight.find(lightName);			// 首先寻找是否存在
	size_t pos = 0;									// 寻找修改位置
	if (it == m_DirLight.end())						// 若不存在
	{
		if (!m_EmptyFXDirLight.empty())				// 如果队列非空，使用队列索引
		{
			pos = m_EmptyFXDirLight.top();
			m_EmptyFXDirLight.pop();  
		}
		else										// 否则扩增元素
			pos = m_DirLight.size();
		
		m_DirLight[lightName] = std::make_pair(light, pos);
	}
	else											// 若存在，直接修改对应位置
	{
		pos = (it->second).second;
		(it->second).first = light;
	}
	m_pFXDirLight->GetElement(pos)->SetRawValue(&light, 0, sizeof(DirectionalLight));
	// 设置遍历终点
	pos = m_DirLight.size() + m_EmptyFXDirLight.size();
	m_pFXDirNums->SetRawValue(&pos, 0, sizeof(int));
}

template<>
void LightManager::SetLight<PointLight>(LPCWSTR lightName, const PointLight& light)
{
	if (m_DirLight.size() >= MAX_LIGHTS)			// 最多允许16盏
		return;
	auto it = m_PointLight.find(lightName);			// 首先寻找是否存在
	size_t pos = 0;									// 寻找修改位置
	if (it == m_PointLight.end())						// 若不存在
	{
		if (!m_EmptyFXPointLight.empty())				// 如果队列非空，使用队列索引
		{
			pos = m_EmptyFXPointLight.top();
			m_EmptyFXPointLight.pop();
		}
		else										// 否则扩增元素
			pos = m_PointLight.size();

		m_PointLight[lightName] = std::make_pair(light, pos);
	}
	else											// 若存在，直接修改对应位置
	{
		pos = (it->second).second;
		(it->second).first = light;
	}
	m_pFXPointLight->GetElement(pos)->SetRawValue(&light, 0, sizeof(PointLight));
	// 设置遍历终点
	pos = m_PointLight.size() + m_EmptyFXPointLight.size();
	m_pFXPointNums->SetRawValue(&pos, 0, sizeof(int));
}

template<>
void LightManager::SetLight<SpotLight>(LPCWSTR lightName, const SpotLight& light)
{
	if (m_DirLight.size() >= MAX_LIGHTS)			// 最多允许16盏
		return;
	auto it = m_SpotLight.find(lightName);			// 首先寻找是否存在
	size_t pos = 0;									// 寻找修改位置
	if (it == m_SpotLight.end())						// 若不存在
	{
		if (!m_EmptyFXSpotLight.empty())				// 如果队列非空，使用队列索引
		{
			pos = m_EmptyFXSpotLight.top();
			m_EmptyFXSpotLight.pop();
		}
		else										// 否则扩增元素
			pos = m_SpotLight.size();

		m_SpotLight[lightName] = std::make_pair(light, pos);
	}
	else											// 若存在，直接修改对应位置
	{
		pos = (it->second).second;
		(it->second).first = light;
	}
	m_pFXSpotLight->GetElement(pos)->SetRawValue(&light, 0, sizeof(SpotLight));
	// 设置遍历终点
	pos = m_SpotLight.size() + m_EmptyFXSpotLight.size();
	m_pFXSpotNums->SetRawValue(&pos, 0, sizeof(int));
}

template<class LightType>
void LightManager::SetLight(LPCWSTR lightName, const LightType& light)
{
}


// ****************获取灯光部分***************
template<>
DirectionalLight LightManager::GetLight<DirectionalLight>(LPCWSTR lightName)
{
	auto it = m_DirLight.find(lightName);
	if (it != m_DirLight.end())
		return (it->second).first;
	else
		return DirectionalLight();
}

template<>
PointLight LightManager::GetLight<PointLight>(LPCWSTR lightName)
{
	auto it = m_PointLight.find(lightName);
	if (it != m_PointLight.end())
		return (it->second).first;
	else
		return PointLight();
}

template<>
SpotLight LightManager::GetLight<SpotLight>(LPCWSTR lightName)
{
	auto it = m_SpotLight.find(lightName);
	if (it != m_SpotLight.end())
		return (it->second).first;
	else
		return SpotLight();
}


template<class LightType>
LightType LightManager::GetLight(LPCWSTR lightName)
{
	return LightType();
}

// ****************删除灯光部分****************
template<>
void LightManager::DeleteLight<DirectionalLight>(LPCWSTR lightName)
{
	auto it = m_DirLight.find(lightName);				// 首先寻找是否存在
	if (it != m_DirLight.end())
	{
		(it->second).first.State = 0;					// 关掉灯光
														// 更新状态
		m_pFXDirLight->GetElement((it->second).second)->SetRawValue(&((it->second).first), 
			0, 
			sizeof(DirectionalLight));
		m_EmptyFXDirLight.push((it->second).second);	// 标记空区域
		m_DirLight.erase(it);							// 删掉对应灯光
	}
		
	
}

template<>
void LightManager::DeleteLight<PointLight>(LPCWSTR lightName)
{
	auto it = m_PointLight.find(lightName);				// 首先寻找是否存在
	if (it != m_PointLight.end())
	{
		(it->second).first.State = 0;					// 关掉灯光
														// 更新状态
		m_pFXPointLight->GetElement((it->second).second)->SetRawValue(&((it->second).first),
			0,
			sizeof(PointLight));
		m_EmptyFXPointLight.push((it->second).second);	// 标记空区域
		m_PointLight.erase(it);							// 删掉对应灯光
	}
}

template<>
void LightManager::DeleteLight<SpotLight>(LPCWSTR lightName)
{
	auto it = m_SpotLight.find(lightName);				// 首先寻找是否存在
	if (it != m_SpotLight.end())
	{
		(it->second).first.State = 0;					// 关掉灯光
														// 更新状态
		m_pFXSpotLight->GetElement((it->second).second)->SetRawValue(&((it->second).first),
			0,
			sizeof(SpotLight));
		m_EmptyFXSpotLight.push((it->second).second);	// 标记空区域
		m_SpotLight.erase(it);							// 删掉对应灯光
	}
}

template<class LightType>
void LightManager::DeleteLight(LPCWSTR lightName)
{
}


// ****************控制灯光部分****************
template<>
void LightManager::Control<DirectionalLight>(LPCWSTR lightName, bool openState)
{
	auto it = m_DirLight.find(lightName);
	if (it != m_DirLight.end())
	{
		(it->second).first.State = openState;
		// 更新状态
		m_pFXDirLight->GetElement((it->second).second)->SetRawValue(&((it->second).first),
			0,
			sizeof(DirectionalLight));
	}
		
}

template<>
void LightManager::Control<PointLight>(LPCWSTR lightName, bool openState)
{
	auto it = m_PointLight.find(lightName);
	if (it != m_PointLight.end())
	{
		(it->second).first.State = openState;
		// 更新状态
		m_pFXPointLight->GetElement((it->second).second)->SetRawValue(&((it->second).first),
			0,
			sizeof(PointLight));
	}
		
}

template<>
void LightManager::Control<SpotLight>(LPCWSTR lightName, bool openState)
{
	auto it = m_SpotLight.find(lightName);
	if (it != m_SpotLight.end())
	{
		(it->second).first.State = openState;
		// 更新状态
		m_pFXSpotLight->GetElement((it->second).second)->SetRawValue(&((it->second).first),
			0,
			sizeof(SpotLight));
	}
		
}

template<class LightType>
void LightManager::Control(LPCWSTR lightName, bool openState)
{
}


