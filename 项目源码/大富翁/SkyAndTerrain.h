//ver 1.0 by ������    
//2017/8/25      
//
//ע:���ļ�������Effect.h,Vertex.h,Grid.hlsl,Sky.hlsl,Terrain.hlsl,LightHelper.hlsl
//���ļ��ṩ
//��պл��ƣ����λ��ƣ���ͼ����
//ʹ�ã�
//��ʼ����m_Scene.init(m_pD3DDevice, m_pD3DImmediateContext);
//�����νṹ�壬���õ��Σ���պУ���ͼ
//���ƣ�
//	XMFLOAT4X4 ViewProj;
//XMStoreFloat4x4(&ViewProj, m_Camera.GetXMViewProj());
//m_Scene.draw(ViewProj, m_Camera.GetXMFloat3Pos());

//ver 1.0.0.0.0.0.1 ������
//2017/8/25
//ע�͵���պк͵��λ���,�Լ���debugʱ��
//��ʼ����
//	m_Scene.init(m_pD3DDevice, m_pD3DImmediateContext);
//	m_Scene.SetGrid(L"Texture/small.jpg");
//m_Scene.SetGridTrans(200.0f, 0.1f, -200.0f);
//m_Scene.SetGridScale(4.0f, 0.0f, 4.0f);
//����:
//XMFLOAT4X4 ViewProj;
//XMStoreFloat4x4(&ViewProj, m_Camera.GetXMViewProj());
//m_Scene.draw(ViewProj, m_Camera.GetXMFloat3Pos());

//ver 1.0.0.0.0.0.0.0.0.3 ������
//��ע��ȡ��������needSky��needTerrain���ж��Ƿ���Ҫ���κ���պ�
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

		//��ʼ��
		void init(ID3D11Device* d3dDevice, ID3D11DeviceContext* deviceContext);
		//��պ�����Ͱ뾶
		void SetSky(std::wstring skyTexName,float radius);
		//����
		void SetTerrain(Terrain::InitInfo tii);
		//��ͼ
		void SetGrid(std::wstring mapName);
		//��ͼλ��
		void SetGridTrans(float transX, float transY, float transZ);
		//��ͼ����
		void SetGridScale(float scaleX, float scaleY, float scaleZ);

		//����
		void draw(XMFLOAT4X4 ViewProj, XMFLOAT3 position);


	private:
		//���θ߶�
		float gethight(float x, float z);
		//���Ƶ�ͼ
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