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

// *********************ģ�岿��********************
// ****************����/��ӵƹⲿ��****************
template<>
void LightManager::SetLight<DirectionalLight>(LPCWSTR lightName, const DirectionalLight& light)
{
	if (m_DirLight.size() >= MAX_LIGHTS)			// �������16յ
		return;
	auto it = m_DirLight.find(lightName);			// ����Ѱ���Ƿ����
	size_t pos = 0;									// Ѱ���޸�λ��
	if (it == m_DirLight.end())						// ��������
	{
		if (!m_EmptyFXDirLight.empty())				// ������зǿգ�ʹ�ö�������
		{
			pos = m_EmptyFXDirLight.top();
			m_EmptyFXDirLight.pop();  
		}
		else										// ��������Ԫ��
			pos = m_DirLight.size();
		
		m_DirLight[lightName] = std::make_pair(light, pos);
	}
	else											// �����ڣ�ֱ���޸Ķ�Ӧλ��
	{
		pos = (it->second).second;
		(it->second).first = light;
	}
	m_pFXDirLight->GetElement(pos)->SetRawValue(&light, 0, sizeof(DirectionalLight));
	// ���ñ����յ�
	pos = m_DirLight.size() + m_EmptyFXDirLight.size();
	m_pFXDirNums->SetRawValue(&pos, 0, sizeof(int));
}

template<>
void LightManager::SetLight<PointLight>(LPCWSTR lightName, const PointLight& light)
{
	if (m_DirLight.size() >= MAX_LIGHTS)			// �������16յ
		return;
	auto it = m_PointLight.find(lightName);			// ����Ѱ���Ƿ����
	size_t pos = 0;									// Ѱ���޸�λ��
	if (it == m_PointLight.end())						// ��������
	{
		if (!m_EmptyFXPointLight.empty())				// ������зǿգ�ʹ�ö�������
		{
			pos = m_EmptyFXPointLight.top();
			m_EmptyFXPointLight.pop();
		}
		else										// ��������Ԫ��
			pos = m_PointLight.size();

		m_PointLight[lightName] = std::make_pair(light, pos);
	}
	else											// �����ڣ�ֱ���޸Ķ�Ӧλ��
	{
		pos = (it->second).second;
		(it->second).first = light;
	}
	m_pFXPointLight->GetElement(pos)->SetRawValue(&light, 0, sizeof(PointLight));
	// ���ñ����յ�
	pos = m_PointLight.size() + m_EmptyFXPointLight.size();
	m_pFXPointNums->SetRawValue(&pos, 0, sizeof(int));
}

template<>
void LightManager::SetLight<SpotLight>(LPCWSTR lightName, const SpotLight& light)
{
	if (m_DirLight.size() >= MAX_LIGHTS)			// �������16յ
		return;
	auto it = m_SpotLight.find(lightName);			// ����Ѱ���Ƿ����
	size_t pos = 0;									// Ѱ���޸�λ��
	if (it == m_SpotLight.end())						// ��������
	{
		if (!m_EmptyFXSpotLight.empty())				// ������зǿգ�ʹ�ö�������
		{
			pos = m_EmptyFXSpotLight.top();
			m_EmptyFXSpotLight.pop();
		}
		else										// ��������Ԫ��
			pos = m_SpotLight.size();

		m_SpotLight[lightName] = std::make_pair(light, pos);
	}
	else											// �����ڣ�ֱ���޸Ķ�Ӧλ��
	{
		pos = (it->second).second;
		(it->second).first = light;
	}
	m_pFXSpotLight->GetElement(pos)->SetRawValue(&light, 0, sizeof(SpotLight));
	// ���ñ����յ�
	pos = m_SpotLight.size() + m_EmptyFXSpotLight.size();
	m_pFXSpotNums->SetRawValue(&pos, 0, sizeof(int));
}

template<class LightType>
void LightManager::SetLight(LPCWSTR lightName, const LightType& light)
{
}


// ****************��ȡ�ƹⲿ��***************
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

// ****************ɾ���ƹⲿ��****************
template<>
void LightManager::DeleteLight<DirectionalLight>(LPCWSTR lightName)
{
	auto it = m_DirLight.find(lightName);				// ����Ѱ���Ƿ����
	if (it != m_DirLight.end())
	{
		(it->second).first.State = 0;					// �ص��ƹ�
														// ����״̬
		m_pFXDirLight->GetElement((it->second).second)->SetRawValue(&((it->second).first), 
			0, 
			sizeof(DirectionalLight));
		m_EmptyFXDirLight.push((it->second).second);	// ��ǿ�����
		m_DirLight.erase(it);							// ɾ����Ӧ�ƹ�
	}
		
	
}

template<>
void LightManager::DeleteLight<PointLight>(LPCWSTR lightName)
{
	auto it = m_PointLight.find(lightName);				// ����Ѱ���Ƿ����
	if (it != m_PointLight.end())
	{
		(it->second).first.State = 0;					// �ص��ƹ�
														// ����״̬
		m_pFXPointLight->GetElement((it->second).second)->SetRawValue(&((it->second).first),
			0,
			sizeof(PointLight));
		m_EmptyFXPointLight.push((it->second).second);	// ��ǿ�����
		m_PointLight.erase(it);							// ɾ����Ӧ�ƹ�
	}
}

template<>
void LightManager::DeleteLight<SpotLight>(LPCWSTR lightName)
{
	auto it = m_SpotLight.find(lightName);				// ����Ѱ���Ƿ����
	if (it != m_SpotLight.end())
	{
		(it->second).first.State = 0;					// �ص��ƹ�
														// ����״̬
		m_pFXSpotLight->GetElement((it->second).second)->SetRawValue(&((it->second).first),
			0,
			sizeof(SpotLight));
		m_EmptyFXSpotLight.push((it->second).second);	// ��ǿ�����
		m_SpotLight.erase(it);							// ɾ����Ӧ�ƹ�
	}
}

template<class LightType>
void LightManager::DeleteLight(LPCWSTR lightName)
{
}


// ****************���Ƶƹⲿ��****************
template<>
void LightManager::Control<DirectionalLight>(LPCWSTR lightName, bool openState)
{
	auto it = m_DirLight.find(lightName);
	if (it != m_DirLight.end())
	{
		(it->second).first.State = openState;
		// ����״̬
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
		// ����״̬
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
		// ����״̬
		m_pFXSpotLight->GetElement((it->second).second)->SetRawValue(&((it->second).first),
			0,
			sizeof(SpotLight));
	}
		
}

template<class LightType>
void LightManager::Control(LPCWSTR lightName, bool openState)
{
}


