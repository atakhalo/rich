#include "Sprite2D.h"

ID3D11Device * Sprite2D::m_pD3dDevice = nullptr;
ID3D11DeviceContext * Sprite2D::m_pD3dContext = nullptr;
LPD3D11EFFECTMATRIXVARIABLE Sprite2D::m_pWorldEffect2D = nullptr;
LPD3D11EFFECTTECHNIQUE Sprite2D::m_pTechniqueEffect2D = nullptr;
LPD3D11EFFECTSHADERRESOURCEVARIABLE Sprite2D::m_pTextureEffect2D = nullptr;
ID3D11InputLayout * Sprite2D::m_pInputLayout2D = nullptr;
ID3D11BlendState* Sprite2D::m_pAlphaToCoverageBS = nullptr;
HWND Sprite2D::m_Hwnd = nullptr;


Sprite2D::Sprite2D()
{
	m_IsChange = false;
	m_Scale = XMFLOAT2(1.0f, 1.0f);
	m_Rotation = XMMatrixIdentity();
	m_Translation = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_pIndexBuff = nullptr;
	m_pVertexBuff = nullptr;
	stride = sizeof(Vertex2D);
	offset = 0;
	m_World = XMMatrixIdentity();
}

Sprite2D::~Sprite2D()
{
	ReleaseCOM(m_pVertexBuff);
	ReleaseCOM(m_pIndexBuff);
}

void Sprite2D::InitAll(ID3D11Device * d3DDevice, ID3D11DeviceContext * d3DDeviceContext, ID3DX11Effect * fX,
	ID3D11InputLayout * pInputLayout, ID3D11BlendState* pAlphaToCoverageBS, HWND hwnd)
{
	m_pD3dDevice = d3DDevice;
	m_pD3dContext = d3DDeviceContext;
	m_pInputLayout2D = pInputLayout;
	m_pAlphaToCoverageBS = pAlphaToCoverageBS;
	m_pTechniqueEffect2D = fX->GetTechniqueByName("TexTech");
	m_pWorldEffect2D = fX->GetVariableByName("gWorldViewProj")->AsMatrix();
	m_pTextureEffect2D = fX->GetVariableByName("Texture")->AsShaderResource();
	m_Hwnd = hwnd;
}

void Sprite2D::Release()
{
}

void Sprite2D::SetTexture(ID3D11ShaderResourceView * texture)
{
	m_pTexture = texture;
}

void Sprite2D::Init(XMFLOAT2 size)
{
	m_Size = size;
	float x = 0.5f*size.x;
	float y = 0.5f*size.y;
	Vertex2D vertices[] =
	{ { XMFLOAT3(-x, y, 0.0f),  XMFLOAT2(0.0f,0.0f) },
	{ XMFLOAT3(x, y, 0.0f),  XMFLOAT2(1.0f,0.0f) },
	{ XMFLOAT3(-x,-y, 0.0f),  XMFLOAT2(0.0f,1.0f) },
	{ XMFLOAT3(x, -y, 0.0f),  XMFLOAT2(1.0f,1.0f) } };

	D3D11_BUFFER_DESC vertexBuffDesc;
	vertexBuffDesc.Usage = D3D11_USAGE_IMMUTABLE;
	vertexBuffDesc.ByteWidth = sizeof(Vertex2D) * 4;
	vertexBuffDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBuffDesc.CPUAccessFlags = 0;
	vertexBuffDesc.MiscFlags = 0;
	vertexBuffDesc.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = vertices;
	HR(m_pD3dDevice->CreateBuffer(&vertexBuffDesc, &vinitData, &m_pVertexBuff));

	UINT indices[]
	{
		0,1,2,
		2,1,3
	};
	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(UINT) * 6;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	ibd.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = indices;
	HR(m_pD3dDevice->CreateBuffer(&ibd, &iinitData, &m_pIndexBuff));
}

void Sprite2D::SetDeep(float deep)
{
	m_Translation.z = deep;
	m_IsChange = true;
}

void Sprite2D::Draw()
{
	m_pD3dContext->IASetInputLayout(m_pInputLayout2D);
	m_pD3dContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	float blendFactor[] = { 0.0f, 0.0f, 0.0f, 0.0f };
	m_pD3dContext->OMSetBlendState(m_pAlphaToCoverageBS, blendFactor, 0xffffffff);
	m_pD3dContext->IASetVertexBuffers(0, 1, &m_pVertexBuff, &stride, &offset);
	m_pD3dContext->IASetIndexBuffer(m_pIndexBuff, DXGI_FORMAT_R32_UINT, 0);
	m_pTextureEffect2D->SetResource(m_pTexture);
	if (m_IsChange)
	{
		m_World = m_Rotation*XMMatrixScaling(m_Scale.x, m_Scale.y, 1.0f)*XMMatrixTranslation(m_Translation.x, m_Translation.y, m_Translation.z);
		m_IsChange = false;
	}
	m_pWorldEffect2D->SetMatrix(reinterpret_cast<float*>(&m_World));

	D3DX11_TECHNIQUE_DESC techDesc;
	m_pTechniqueEffect2D->GetDesc(&techDesc);
	for (UINT p = 0; p < techDesc.Passes; ++p)
	{
		m_pTechniqueEffect2D->GetPassByIndex(p)->Apply(0, m_pD3dContext);

		m_pD3dContext->DrawIndexed(6, 0, 0);
	}
	m_pD3dContext->OMSetBlendState(0, blendFactor, 0xffffffff);
}

void Sprite2D::Translate(float x, float y)
{
	m_Translation.x = x;
	m_Translation.y = y;
	m_IsChange = true;
}

void Sprite2D::Rotate(float radian)
{
	m_Rotation = XMMatrixRotationEllipseZ(1.0f, radian);
	m_IsChange = true;
}

void Sprite2D::RotateElliptical(float radian)
{
	RECT rect;
	GetWindowRect(m_Hwnd, &rect);
	m_Rotation = XMMatrixRotationEllipseZ((rect.bottom - rect.top - 47)*1.0f / (rect.right - rect.left - 18), radian);
	m_IsChange = true;
}

void Sprite2D::Scale(float x, float y)
{
	m_Scale.x = x;
	m_Scale.y = y;
	m_IsChange = true;
}

void Sprite2D::ReSet()
{
	m_Scale = XMFLOAT2(1.0f, 1.0f);
	m_Rotation = XMMatrixIdentity();
	m_Translation = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_IsChange = true;
}

XMMATRIX Sprite2D::XMMatrixRotationEllipseZ(float ratio, float radian)
{
	float fcos = cos(radian);
	float fsin = sin(radian);
	XMMATRIX M(
		fcos, fsin / ratio, 0, 0,
		-ratio*fsin, fcos, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1);
	return M;
}