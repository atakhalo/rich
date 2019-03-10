#include "StreetManager.h"

Rich::StreetManager::StreetManager()
{
	m_originalPoint = { 0.0f,0.0f,0.0f };
	m_scaleFactor = 1.0f;
	m_ModelScale = 1.0f;
}

Rich::StreetManager::~StreetManager()
{
}

void Rich::StreetManager::initStreet(std::string txtName)
{
	std::fstream fin(txtName.c_str());
	int i = 0;
	int j = 0;
	int last_i = 0;
	int funcType = 0;
	std::vector<BLOCK> huTong;
	while (!fin.eof())
	{
		fin >> i;
		if (i != last_i)
		{
			m_Street.push_back(huTong);
			huTong.clear();
			last_i = i;
		}
		BLOCK tempBlock;
		fin >> tempBlock.Pos.x;
		fin >> tempBlock.Pos.y;
		fin >> tempBlock.Offset.x;
		fin >> tempBlock.Offset.y;
		fin >> funcType;
		tempBlock.FuncType = static_cast<FUNCTYPE>(funcType);
		fin >> tempBlock.PriceFactor;
		tempBlock.isBarrier = false;
		tempBlock.owner = 0;
		tempBlock.HouseLevel = 0;
		tempBlock.HouseType = NOHOUSE;
		if(!fin.eof())
		huTong.push_back(tempBlock);
	}
	m_Street.push_back(huTong);
	huTong.clear();
	fin.close();
}

Rich::StreetManager::BLOCK Rich::StreetManager::BlockGet(POINT index) const
{
	if (index.x >= m_Street.size() || index.y >= m_Street[index.x].size())
	return BLOCK();
	return m_Street[index.x][index.y];
}

int Rich::StreetManager::Rent(POINT index) const
{
	if (index.x >= m_Street.size() || index.y >= m_Street[index.x].size())
		return 0;
	if(m_Street[index.x][index.y].owner == 0)
	return 0;
	int HouseCount = 0;
	for(auto it = m_Street[index.x].begin(); it != m_Street[index.x].end();++it)
	{
		if (it->owner == m_Street[index.x][index.y].owner)
			++HouseCount;
	}
	if (m_Street[index.x][index.y].HouseType == NOHOUSE)
		return 200 + (m_Street[index.x][index.y].PriceFactor * 1000 * 0.4f) * HouseCount * (0.1f + 1); 
	else if (m_Street[index.x][index.y].HouseType == FLAT)
	{
		int housePrice = 0;
		switch (m_Street[index.x][index.y].HouseLevel)
		{
		case 5:	 housePrice += 3500;													//此处不加break，因为是累积值
		case 4:	 housePrice += 3000;
		case 3:	 housePrice += 2500;
		case 2:	 housePrice += 2000;
		case 1:	 housePrice += 1500;
		case 0: housePrice += 1000; break;	
		}
		return 200 + (m_Street[index.x][index.y].PriceFactor * housePrice * 0.4f) * (HouseCount * 0.1f + 1);
	}
	else if (m_Street[index.x][index.y].HouseType == COTTAGE)
	{
		int housePrice = 0;
		switch (m_Street[index.x][index.y].HouseLevel)
		{
		case 3:	 housePrice += 4500;									//此处不加break，因为是累积值
		case 2:	 housePrice += 3500;
		case 1:	 housePrice += 2000;
		case 0:housePrice += 1000; break;
		}
		return 200 + (m_Street[index.x][index.y].PriceFactor * housePrice *0.4f) * (HouseCount * 0.1f + 1);
	}
}

int Rich::StreetManager::HouseLevelGet(POINT index) const
{
	if (index.x >= m_Street.size() || index.y >= m_Street[index.x].size())
		return -1;
	return m_Street[index.x][index.y].HouseLevel;
}

Rich::HOUSETYPE Rich::StreetManager::HouseTypeGet(POINT index) const
{
	if (index.x >= m_Street.size() || index.y >= m_Street[index.x].size())
		return NOHOUSE;
	return m_Street[index.x][index.y].HouseType;
}

int Rich::StreetManager::HouseOwnerGet(POINT index) const
{
	if (index.x >= m_Street.size() || index.y >= m_Street[index.x].size())
		return 0;
	return m_Street[index.x][index.y].owner;
}

bool Rich::StreetManager::BarrierGet(POINT index) const
{
	if (index.x >= m_Street.size() || index.y >= m_Street[index.x].size())
		return false;
	return m_Street[index.x][index.y].isBarrier;
}

Rich::FUNCTYPE Rich::StreetManager::FuncTypeGet(POINT index) const
{
	if (index.x >= m_Street.size() || index.y >= m_Street[index.x].size())
		return HOUSE;
	return m_Street[index.x][index.y].FuncType;
}

POINT Rich::StreetManager::BlockPosGet(POINT index) const
{
	if (index.x >= m_Street.size() || index.y >= m_Street[index.x].size())
		return {0,0};
	return m_Street[index.x][index.y].Pos;
}

POINT Rich::StreetManager::BlockNextGet(POINT index, int dir) const
{
	if (index.x >= m_Street.size() || index.y >= m_Street[index.x].size())
		return { 0,0 };
	if (dir != -1 && dir != 1)
		return index;
	if (index.y + dir >= static_cast<LONG>(m_Street[index.x].size()))
	{
		if (index.x + 1 >= static_cast<LONG>(m_Street.size()))
		{
			return { 0,0 };
		}
		else
			return { index.x + 1,0 };
	}
	else if (index.y + dir < 0)
	{
		if (index.x - 1 < 0)
		{
			return { static_cast<LONG>(m_Street.size() - 1), static_cast<LONG>(m_Street[m_Street.size() - 1].size() - 1 )};
		}
		else
			return { index.x - 1,static_cast<LONG>(m_Street[index.x - 1].size() - 1) };
	}
	else
		return {index.x,index.y + dir};
}

POINT Rich::StreetManager::BlockNextPosGet(POINT index, int dir) const
{
	if (index.x >= m_Street.size() || index.y >= m_Street[index.x].size())
		return {0,0};
	if (dir != -1 && dir != 1)
		return m_Street[index.x][index.y].Pos;
	if (index.y + dir >= static_cast<LONG>(m_Street[index.x].size()))
	{
		if (index.x + 1 >= static_cast<LONG>( m_Street.size()))
		{
			return m_Street[0][0].Pos;
		}
		else
			return m_Street[index.x + 1][0].Pos;
	}
	else if (index.y + dir < 0)
	{
		if (index.x - 1 < 0)
		{
			return m_Street.back().back().Pos;
		}
		else
			return m_Street[index.x - 1].back().Pos;
	}
	else
		return m_Street[index.x][index.y + dir].Pos;
}

XMFLOAT4X4 Rich::StreetManager::BlockTransMatrixGet(POINT index) const
{
	if (index.x >= m_Street.size() || index.y >= m_Street[index.x].size())
		return XMFLOAT4X4();
	float transx = m_originalPoint.x + m_scaleFactor * (m_Street[index.x][index.y].Pos.x);
	float transy = m_originalPoint.y;
	float transz = m_originalPoint.z - m_scaleFactor * (m_Street[index.x][index.y].Pos.y);
	XMFLOAT4X4 BlockTrans;
	XMStoreFloat4x4(&BlockTrans, XMMatrixTranslation(transx, transy, transz));
		return BlockTrans;
}

XMFLOAT3 Rich::StreetManager::BlockTransFloat3Get(POINT index) const
{
	if (index.x >= m_Street.size() || index.y >= m_Street[index.x].size())
		return XMFLOAT3();
	float transx = m_originalPoint.x + m_scaleFactor * (m_Street[index.x][index.y].Pos.x);
	float transy = m_originalPoint.y;
	float transz = m_originalPoint.z - m_scaleFactor * (m_Street[index.x][index.y].Pos.y);
	return XMFLOAT3{ transx,transy,transz };
}

void Rich::StreetManager::BarrierSet(POINT index,bool isBarrier)
{
	if (index.x >= m_Street.size() || index.y >= m_Street[index.x].size())
		return;
	m_Street[index.x][index.y].isBarrier = isBarrier;
}

POINT Rich::StreetManager::MoveTo(POINT index, int dir,int stepNum) const
{
	if(stepNum == 0 || (dir != -1 && dir != 1))
		return index;
	for (int i = 0; i != stepNum; ++i)
	{
		index = BlockNextGet(index, dir);
		if (m_Street[index.x][index.y].isBarrier)
			break;
	}
	return index;
}

void Rich::StreetManager::HutongDestroy(int index)
{
	if (index < 0 || index >= m_Street.size())
		return;
	for (auto it = m_Street[index].begin(); it != m_Street[index].end(); ++it)
	{
		it->HouseLevel = 0;
		it->HouseType = NOHOUSE;
	}
}

void Rich::StreetManager::HutongLevelChange(int index, int dLevel)
{
	if (index < 0 || index >= m_Street.size())
		return;
	for (int i  = 0; i !=  m_Street[index].size();++i)
	{
		HouseLevelAdd({ index,i }, dLevel);
	}
}

void Rich::StreetManager::HutongValueChange(int index, int factor)
{
	if (index < 0 || index >= m_Street.size())
		return;
	for (auto it = m_Street[index].begin(); it != m_Street[index].end(); ++it)
	{
		it->PriceFactor = MathHelper::Clamp(it->PriceFactor + 0.1f * factor, 0.8f, 1.5f);
	}
}

void Rich::StreetManager::HouseLevelSet(POINT index, int toLevel)
{
	if (index.x >= m_Street.size() || index.y >= m_Street[index.x].size())
		return;
	if (m_Street[index.x][index.y].owner == 0)
		return;
	m_Street[index.x][index.y].HouseLevel = toLevel;
}

void Rich::StreetManager::HouseTypeSet(POINT index, HOUSETYPE houseType)
{
	if (index.x >= m_Street.size() || index.y >= m_Street[index.x].size())
		return;
	if (m_Street[index.x][index.y].owner == 0)
		return;
	m_Street[index.x][index.y].HouseType = houseType;
}

void Rich::StreetManager::HouseLevelAdd(POINT index, int dLevel)
{
	if (index.x >= m_Street.size() || index.y >= m_Street[index.x].size())
		return;
	if (m_Street[index.x][index.y].owner == 0)
		return;
	if(m_Street[index.x][index.y].HouseType == COTTAGE && m_Street[index.x][index.y].HouseLevel + dLevel <= 3 && m_Street[index.x][index.y].HouseLevel + dLevel >= 0)
		m_Street[index.x][index.y].HouseLevel += dLevel;
	else if(m_Street[index.x][index.y].HouseType == FLAT && m_Street[index.x][index.y].HouseLevel + dLevel <= 5 && m_Street[index.x][index.y].HouseLevel + dLevel >= 0)
		m_Street[index.x][index.y].HouseLevel += dLevel;	
}

void Rich::StreetManager::HouseOwnerSet(POINT index, int playerID)
{
	if (index.x >= m_Street.size() || index.y >= m_Street[index.x].size())
		return;
	m_Street[index.x][index.y].owner = playerID;
}

void Rich::StreetManager::HouseDestroy(POINT index)
{
	if (index.x >= m_Street.size() || index.y >= m_Street[index.x].size())
		return;
	m_Street[index.x][index.y].HouseLevel = 0;
	m_Street[index.x][index.y].HouseType = NOHOUSE;
}

void Rich::StreetManager::HouseLevelMax(POINT index)
{
	if (index.x >= m_Street.size() || index.y >= m_Street[index.x].size())
		return;
	if (m_Street[index.x][index.y].owner == 0)
		return;
	switch (m_Street[index.x][index.y].HouseType)
	{
	case NOHOUSE:
		return;
	case FLAT:
		m_Street[index.x][index.y].HouseLevel = 5;
		break;
	case COTTAGE:
		m_Street[index.x][index.y].HouseLevel = 3;
		break;
	}
}

void Rich::StreetManager::originalPointSet(XMFLOAT3 originalPoint)
{
	m_originalPoint = originalPoint;
}

void Rich::StreetManager::scaleFactorSet(float scaleFactor)
{
	m_scaleFactor = scaleFactor;
}

void Rich::StreetManager::ModelScaleSet(float scale)
{
	m_ModelScale = scale;
}

int Rich::StreetManager::StreetSize() const
{
	return m_Street.size();
}

int Rich::StreetManager::SingleStreetSize(int index) const
{
	if (index >= m_Street.size() )
		return 0;
	return m_Street[index].size();
}

void Rich::StreetManager::Initialize()
{
	//m_Houses["别墅1"].InitModel("Object/房屋.obj", "Texture/Town_S_House.png", "Material/房屋.mtl");
	m_Models["别墅1"].InitModel("Object/C1.obj", "Texture/C1.png", "Material/C1.mtl");
	m_Models["别墅2"].InitModel("Object/C2.obj", "Texture/C2.png", "Material/C2.mtl");
	m_Models["别墅3"].InitModel("Object/C3.obj", "Texture/C3.png", "Material/C3.mtl");
	m_Models["公寓1"].InitModel("Object/F1.obj", "Texture/F1.png", "Material/F1.mtl");
	m_Models["公寓2"].InitModel("Object/F2.obj", "Texture/F2.png", "Material/F2.mtl");
	m_Models["公寓3"].InitModel("Object/F3.obj", "Texture/F3.png", "Material/F3.mtl");
	m_Models["公寓4"].InitModel("Object/F4.obj", "Texture/F4.png", "Material/F4.mtl");
	m_Models["公寓5"].InitModel("Object/F5.obj", "Texture/F5.png", "Material/F5.mtl");
	m_Models["所有者1"].InitModel("Object/owner.obj", "Texture/owner1.png", "Material/owner1.mtl");
	m_Models["所有者2"].InitModel("Object/owner.obj", "Texture/owner2.png", "Material/owner2.mtl");
	m_Models["所有者3"].InitModel("Object/owner.obj", "Texture/owner3.png", "Material/owner3.mtl");
	m_Models["所有者4"].InitModel("Object/owner.obj", "Texture/owner4.png", "Material/owner4.mtl");
	m_Models["路障"].InitModel("Object/barrier.obj", "Texture/barrier.png", "Material/barrier.mtl");

}

void Rich::StreetManager::Draw()
{
	m_Models["别墅1"].DrawInstances();
	m_Models["别墅2"].DrawInstances();
	m_Models["别墅3"].DrawInstances();
	m_Models["公寓1"].DrawInstances();
	m_Models["公寓2"].DrawInstances();
	m_Models["公寓3"].DrawInstances();
	m_Models["公寓4"].DrawInstances();
	m_Models["公寓5"].DrawInstances();
	m_Models["所有者1"].DrawInstances();
	m_Models["所有者2"].DrawInstances();
	m_Models["所有者3"].DrawInstances();
	m_Models["所有者4"].DrawInstances();
	m_Models["路障"].DrawInstances();
}

void Rich::StreetManager::UpdateScene()
{
		std::vector<XMFLOAT4X4> cottage1;
		std::vector<XMFLOAT4X4> cottage2;
		std::vector<XMFLOAT4X4> cottage3;
		std::vector<XMFLOAT4X4> flat1;
		std::vector<XMFLOAT4X4> flat2;
		std::vector<XMFLOAT4X4> flat3;
		std::vector<XMFLOAT4X4> flat4;
		std::vector<XMFLOAT4X4> flat5;
		std::vector<XMFLOAT4X4> owner1;
		std::vector<XMFLOAT4X4> owner2;
		std::vector<XMFLOAT4X4> owner3;
		std::vector<XMFLOAT4X4> owner4;
		std::vector<XMFLOAT4X4> barrier;

		for (auto it1 = m_Street.begin(); it1 != m_Street.end(); ++it1)
		{
			for (auto it2 = it1->begin(); it2 != it1->end(); ++it2)
			{
				//获得房屋世界矩阵	
				float transx = m_originalPoint.x + m_scaleFactor * (it2->Pos.x + it2->Offset.x);
				float transy = m_originalPoint.y;
				float transz = m_originalPoint.z - m_scaleFactor * (it2->Pos.y + it2->Offset.y);
				float rotation = 0.0f;
				if (it2->Offset.y == -1)
					rotation = MathHelper::Pi;
				else if (it2->Offset.x == -1)
					rotation = 0.5 * MathHelper::Pi;
				else if (it2->Offset.x == 1)
					rotation = 1.5 * MathHelper::Pi;
				XMMATRIX XMworld = XMMatrixIdentity() * XMMatrixScaling(m_ModelScale, m_ModelScale, m_ModelScale) * XMMatrixRotationY(rotation) * XMMatrixTranslation(transx, transy, transz);
				XMFLOAT4X4 world;
				XMStoreFloat4x4(&world, XMworld);
				switch (it2->owner)
				{
				case 0:break;
				case 1:
					owner1.push_back(world); break;
				case 2:
					owner2.push_back(world); break;
				case 3:
					owner3.push_back(world); break;
				case 4:
					owner4.push_back(world); break;
				}
				if (it2->HouseType == COTTAGE)
				{
					switch (it2->HouseLevel)
					{
					case 1:
						cottage1.push_back(world); break;
					case 2:
						cottage2.push_back(world); break;
					case 3:
						cottage3.push_back(world); break;
					}
				}
				else if (it2->HouseType == FLAT)
				{
					switch (it2->HouseLevel)
					{
					case 1:
						flat1.push_back(world); break;
					case 2:
						flat2.push_back(world); break;
					case 3:
						flat3.push_back(world); break;
					case 4:
						flat4.push_back(world); break;
					case 5:
						flat5.push_back(world); break;
					}
				}
				//获得路障矩阵
				if (it2->isBarrier)
				{
					float barrierTransx = m_originalPoint.x + m_scaleFactor * (it2->Pos.x);
					float barrierTransy = m_originalPoint.y;
					float barrierTransz = m_originalPoint.z - m_scaleFactor * (it2->Pos.y);
					XMMATRIX XMBarrierworld = XMMatrixIdentity() * XMMatrixScaling(m_ModelScale, m_ModelScale, m_ModelScale) * XMMatrixTranslation(barrierTransx, barrierTransy, barrierTransz);
					XMFLOAT4X4 barrierWorld;
					XMStoreFloat4x4(&barrierWorld, XMBarrierworld);
					barrier.push_back(barrierWorld);
				}
			}
		}

		m_Models["别墅1"].UpdateWorldMatrixes(cottage1);
		m_Models["别墅2"].UpdateWorldMatrixes(cottage2);
		m_Models["别墅3"].UpdateWorldMatrixes(cottage3);
		m_Models["公寓1"].UpdateWorldMatrixes(flat1);
		m_Models["公寓2"].UpdateWorldMatrixes(flat2);
		m_Models["公寓3"].UpdateWorldMatrixes(flat3);
		m_Models["公寓4"].UpdateWorldMatrixes(flat4);
		m_Models["公寓5"].UpdateWorldMatrixes(flat5);
		m_Models["路障"].UpdateWorldMatrixes(barrier);
		m_Models["所有者1"].UpdateWorldMatrixes(owner1);
		m_Models["所有者2"].UpdateWorldMatrixes(owner2);
		m_Models["所有者3"].UpdateWorldMatrixes(owner3);
		m_Models["所有者4"].UpdateWorldMatrixes(owner4);
}
