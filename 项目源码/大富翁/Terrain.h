//***************************************************************************************
//周欣晖   v1.0.0.0
//地形         可用最多五种贴图融合绘制，须提供高度度和融合图。
//可使用GetHeight(float x, float z)函数获得地形上某个点的高度，可用于物体或摄影机贴地(山)运动
//
//	使用方法:
//首先填充结构体InitInfo，初始化类。//在init阶段完成
//Terrain::InitInfo tii;
//tii.HeightMapFilename = L"Textures/terrain2.raw";高度图  只支持8-bit的raw文件
//tii.LayerMapFilename0 = L"Textures/grass.dds";纹理
//tii.LayerMapFilename1 = L"Textures/darkdirt.dds";
//tii.LayerMapFilename2 = L"Textures/stone.dds";
//tii.LayerMapFilename3 = L"Textures/lightdirt.dds";
//tii.LayerMapFilename4 = L"Textures/snow.dds";
//tii.BlendMapFilename = L"Textures/blend.dds";//融合图
//tii.HeightScale = 50.0f;//（原为0-255）高度放大倍数
//tii.HeightmapWidth = 2049;//图片大小
//tii.HeightmapHeight = 2049;
//tii.CellSpacing = 0.5f;
//tii.texCount = 5;//纹理个数(最小为1)
//初始化
//	m_Terrain.Init(md3dDevice, md3dImmediateContext, tii);
//
//draw阶段
//绘制（传递摄影机和灯光的数据）注：此实现建立在Effect.h基础上
//XMFLOAT4X4 ViewProj;
//XMStoreFloat4x4(&ViewProj, m_Cam.ViewProj());
//m_Terrain.Draw(md3dImmediateContext, ViewProj, m_Cam.GetPosition(), m_DirLights);
//
//为方便灯光自主管理类，此版本不设置灯光（我也不知道有没有用）
//void Draw(ID3D11DeviceContext* dc, XMFLOAT4X4 ViewProj, XMFLOAT3 position);
//***************************************************************************************
#pragma once

#include "d3dUtil.h"
#include "LightHelper.h"
#include "Effects.h"
#include "Vertex.h"
#include <fstream>
#include <sstream>

struct DirectionalLight;

class Terrain
{
public:
	struct InitInfo
	{
		std::wstring HeightMapFilename;
		std::wstring LayerMapFilename0;
		std::wstring LayerMapFilename1;
		std::wstring LayerMapFilename2;
		std::wstring LayerMapFilename3;
		std::wstring LayerMapFilename4;
		std::wstring BlendMapFilename;
		float HeightScale;
		UINT HeightmapWidth;
		UINT HeightmapHeight;
		float CellSpacing;
		int texCount;
	};

public:
	Terrain();
	~Terrain();

	float GetWidth()const;
	float GetDepth()const;
	float GetHeight(float x, float z)const;

	XMMATRIX GetWorld()const;
	void SetWorld(CXMMATRIX M);

	void Init(ID3D11Device* device, ID3D11DeviceContext* dc, const InitInfo& initInfo);

	void Draw(ID3D11DeviceContext* dc, XMFLOAT4X4 viewProj, XMFLOAT3 position, DirectionalLight lights[3]);
	void Draw(ID3D11DeviceContext* dc, XMFLOAT4X4 ViewProj, XMFLOAT3 position);

private:
	void LoadHeightmap();
	void Smooth();
	bool InBounds(int i, int j);
	float Average(int i, int j);
	void CalcAllPatchBoundsY();
	void CalcPatchBoundsY(UINT i, UINT j);
	void BuildQuadPatchVB(ID3D11Device* device);
	void BuildQuadPatchIB(ID3D11Device* device);
	void BuildHeightmapSRV(ID3D11Device* device);

private:

	// Divide heightmap into patches such that each patch has CellsPerPatch cells
	// and CellsPerPatch+1 vertices.  Use 64 so that if we tessellate all the way 
	// to 64, we use all the data from the heightmap.  
	static const int CellsPerPatch = 64;

	ID3D11Buffer* m_QuadPatchVB;
	ID3D11Buffer* m_QuadPatchIB;

	ID3D11ShaderResourceView* m_LayerMapArraySRV;
	ID3D11ShaderResourceView* m_BlendMapSRV;
	ID3D11ShaderResourceView* m_HeightMapSRV;

	InitInfo mInfo;

	UINT m_NumPatchVertices;
	UINT m_NumPatchQuadFaces;

	UINT m_NumPatchVertRows;
	UINT m_NumPatchVertCols;

	XMFLOAT4X4 m_World;

	Material m_Mat;

	std::vector<XMFLOAT2> m_PatchBoundsY;
	std::vector<float> m_Heightmap;
};

