#include "SkyAndTerrain.h"

Rich::SkyAndTerrain::SkyAndTerrain()
{
}

Rich::SkyAndTerrain::~SkyAndTerrain()
{
	if(needSky)SafeDelete(m_Sky);
	ReleaseCOM(m_GridVB);
	ReleaseCOM(m_GridIB);
	ReleaseCOM(m_GridTex);
	Effects::DestroyAll();
	InputLayouts::DestroyAll();
	RenderStates::DestroyAll();
}

void Rich::SkyAndTerrain::init(ID3D11Device * d3dDevice, ID3D11DeviceContext * deviceContext)
{
	Effects::InitAll(d3dDevice);
	InputLayouts::InitAll(d3dDevice);
	RenderStates::InitAll(d3dDevice);
	m_d3dDevice = d3dDevice;
	m_DeviceContext = deviceContext;

	m_DirLights[0].Ambient = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	m_DirLights[0].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	m_DirLights[0].Specular = XMFLOAT4(0.8f, 0.8f, 0.7f, 1.0f);
	m_DirLights[0].Direction = XMFLOAT3(0.707f, -0.707f, 0.0f);

	m_TerrainDirLights[0].Ambient = XMFLOAT4(0.5f, 0.5f, 0.5f, 0.5f);
	m_TerrainDirLights[0].Diffuse = XMFLOAT4(0.5f, 0.5f, 0.5f, 0.5f);
	m_TerrainDirLights[0].Specular = XMFLOAT4(0.8f, 0.8f, 0.7f, 1.0f);
	m_TerrainDirLights[0].Direction = XMFLOAT3(0.707f, -0.707f, 0.0f);

	m_Mat.Ambient = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	m_Mat.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	m_Mat.Specular = XMFLOAT4(0.0f, 0.0f, 0.0f, 64.0f);
	m_Mat.Reflect = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);

	needSky = false;
	needTerrain = false;
}

void Rich::SkyAndTerrain::SetSky(std::wstring skyTexName,float radius)
{
	m_Sky = new Sky(m_d3dDevice, skyTexName.c_str(),radius);
	needSky = true;
}

void Rich::SkyAndTerrain::SetTerrain(Terrain::InitInfo tii)
{
	m_Terrain.Init(m_d3dDevice, m_DeviceContext, tii);
	needTerrain = true;
}

void Rich::SkyAndTerrain::SetGrid(std::wstring mapName)
{
	XMMATRIX I = XMMatrixIdentity();
	m_GridWorld = I;
	m_GridScale = I;
	m_GridTrans = I;
	m_stride = sizeof(Vertex::Basic32);
	m_offset = 0;
	m_VertexOffset = 0;
	m_IndexOffset = 0;

	HR(D3DX11CreateShaderResourceViewFromFile(m_d3dDevice,
		mapName.c_str(), 0, 0, &m_GridTex, 0));

	GeometryGenerator::MeshData grid;
	GeometryGenerator geoGen;

	geoGen.CreateGrid(100.0f, 100.0f, 2, 2, grid);

	m_GridIndexCount = grid.Indices.size();

	m_VertexOffset = 0;
	m_IndexOffset = 0;

	UINT totalVertexCount = grid.Vertices.size();

	UINT totalIndexCount = m_GridIndexCount;

	m_GridVertices.resize(totalVertexCount);
	for (size_t i = 0; i < totalVertexCount; ++i)
	{
		m_GridVertices[i].Pos = grid.Vertices[i].Position;
		m_GridVertices[i].Normal = grid.Vertices[i].Normal;
		m_GridVertices[i].Tex = grid.Vertices[i].TexC;
	}

	m_GridIndices.resize(totalIndexCount);
	for (UINT j = 0; j < totalIndexCount; ++j)
	{
		m_GridIndices[j] = grid.Indices[j];
	}

	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(Vertex::Basic32) * totalVertexCount;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = &m_GridVertices[0];
	HR(m_d3dDevice->CreateBuffer(&vbd, &vinitData, &m_GridVB));

	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(UINT) * totalIndexCount;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = &m_GridIndices[0];
	HR(m_d3dDevice->CreateBuffer(&ibd, &iinitData, &m_GridIB));
}

void Rich::SkyAndTerrain::SetGridTrans(float transX, float transY, float transZ)
{
	m_GridTrans = XMMatrixTranslation(transX, transY, transZ);
}

void Rich::SkyAndTerrain::SetGridScale(float scaleX, float scaleY, float scaleZ)
{
	m_GridScale = XMMatrixScaling(scaleX, scaleY, scaleZ);
}

void Rich::SkyAndTerrain::draw(XMFLOAT4X4 ViewProj, XMFLOAT3 position)
{
	float blendFactor[] = { 0.0f, 0.0f, 0.0f, 0.0f };
	m_DeviceContext->OMSetBlendState(0, blendFactor, 0xffffffff);
	m_DeviceContext->OMSetDepthStencilState(0,0);
	m_DeviceContext->RSSetState(0);
	if (needSky)
	{
		m_Sky->Draw(m_DeviceContext, ViewProj, position);
		m_DeviceContext->OMSetDepthStencilState(0, 0);
		m_DeviceContext->RSSetState(0);
	}
	if (needTerrain)
	{
		m_Terrain.Draw(m_DeviceContext, ViewProj, position, m_TerrainDirLights);
		m_DeviceContext->OMSetDepthStencilState(0, 0);
		m_DeviceContext->RSSetState(0);
	}
	drawGrid(ViewProj);
}

float Rich::SkyAndTerrain::gethight(float x, float z)
{
	return m_Terrain.GetHeight(x, z);
}

void Rich::SkyAndTerrain::drawGrid(XMFLOAT4X4 ViewProj)
{
	m_DeviceContext->IASetInputLayout(InputLayouts::Basic32);
	m_DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	Effects::BasicFX->SetDirLights(m_DirLights);
	Effects::BasicFX->SetTexTransform(XMMatrixIdentity());
	Effects::BasicFX->SetMaterial(m_Mat);
	XMMATRIX world = m_GridWorld * m_GridScale * m_GridTrans;
	Effects::BasicFX->SetWorld(world);
	XMMATRIX ModelworldInvTranspose = MathHelper::InverseTranspose(world);
	Effects::BasicFX->SetWorldInvTranspose(ModelworldInvTranspose);
	Effects::BasicFX->SetWorldViewProj(world * XMLoadFloat4x4(&ViewProj));
	Effects::BasicFX->SetDiffuseMap(m_GridTex);

	m_DeviceContext->IASetVertexBuffers(0, 1, &m_GridVB, &m_stride, &m_offset);
	m_DeviceContext->IASetIndexBuffer(m_GridIB, DXGI_FORMAT_R32_UINT, 0);
	ID3DX11EffectTechnique* Tech = Effects::BasicFX->Light1TexAlphaClipTech;
	Tech->GetPassByIndex(0)->Apply(0, m_DeviceContext);
	m_DeviceContext->DrawIndexed(m_GridIndexCount, m_IndexOffset, m_VertexOffset);
}
