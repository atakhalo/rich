//ver 1.0 by 周欣晖    
//2017/8/25      
//
//注:本文件依赖于Effect.h,Vertex.h,Grid.hlsl,Sky.hlsl,Terrain.hlsl,LightHelper.hlsl
//本文件提供
//天空盒绘制，地形绘制，地图绘制
//使用：
//初始化：m_Scene.init(m_pD3DDevice, m_pD3DImmediateContext);
//填充地形结构体，设置地形，天空盒，地图
//绘制：
//	XMFLOAT4X4 ViewProj;
//XMStoreFloat4x4(&ViewProj, m_Camera.GetXMViewProj());
//m_Scene.draw(ViewProj, m_Camera.GetXMFloat3Pos());

//ver 1.0.0.0.0.0.1 周欣晖
//2017/8/25
//注释掉天空盒和地形绘制,以减少debug时间
//初始化：
//	m_Scene.init(m_pD3DDevice, m_pD3DImmediateContext);
//	m_Scene.SetGrid(L"Texture/small.jpg");
//m_Scene.SetGridTrans(200.0f, 0.1f, -200.0f);
//m_Scene.SetGridScale(4.0f, 0.0f, 4.0f);
//绘制:
//XMFLOAT4X4 ViewProj;
//XMStoreFloat4x4(&ViewProj, m_Camera.GetXMViewProj());
//m_Scene.draw(ViewProj, m_Camera.GetXMFloat3Pos());

//ver 1.0.0.0.0.0.0.0.0.3 周欣晖
//把注释取消并改用needSky和needTerrain来判断是否需要地形和天空盒
#pragma once
#include "Sky.h"
#include "Terrain.h"
#include "RenderStates.h"
#include "GeometryGenerator.h"
namespace Rich 
{
	class SkyAndTerrain
	{
	public:
		SkyAndTerrain();
		~SkyAndTerrain();

		//初始化
		void init(ID3D11Device* d3dDevice, ID3D11DeviceContext* deviceContext);
		//天空盒纹理和半径
		void SetSky(std::wstring skyTexName,float radius);
		//地形
		void SetTerrain(Terrain::InitInfo tii);
		//地图
		void SetGrid(std::wstring mapName);
		//地图位移
		void SetGridTrans(float transX, float transY, float transZ);
		//地图缩放
		void SetGridScale(float scaleX, float scaleY, float scaleZ);

		//绘制
		void draw(XMFLOAT4X4 ViewProj, XMFLOAT3 position);


	private:
		//地形高度
		float gethight(float x, float z);
		//绘制地图
		void drawGrid(XMFLOAT4X4 ViewProj);
		Sky* m_Sky;
		Terrain m_Terrain;
		ID3D11Device* m_d3dDevice;
		ID3D11DeviceContext* m_DeviceContext;
		
		DirectionalLight m_DirLights[3];
		DirectionalLight m_TerrainDirLights[3];
		Material m_Mat;

		ID3D11Buffer* m_GridVB;
		ID3D11Buffer* m_GridIB;
		ID3D11ShaderResourceView* m_GridTex;
		XMMATRIX m_GridWorld;
		XMMATRIX m_GridScale;
		XMMATRIX m_GridTrans;

		std::vector<Vertex::Basic32> m_GridVertices;
		std::vector<UINT> m_GridIndices;
		UINT m_GridIndexCount;

		UINT m_stride;
		UINT m_offset;
		UINT m_VertexOffset = 0;
		UINT m_IndexOffset = 0;

		bool needTerrain;
		bool needSky;
	};

}