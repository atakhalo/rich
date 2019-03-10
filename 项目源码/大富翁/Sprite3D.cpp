#include "Sprite3D.h"
#include "XUtility.h"


ID3DX11EffectShaderResourceVariable*	Sprite3D::m_pFXTexture = nullptr;			// Effects����
ID3DX11EffectVariable*					Sprite3D::m_pFXMaterial = nullptr;			// Effects����
ID3DX11EffectMatrixVariable*			Sprite3D::m_pFXWorld = nullptr;				// Effects�������
ID3DX11EffectMatrixVariable*			Sprite3D::m_pFXWorldInvTranspose = nullptr;	// Effects���絹��ת��
ID3D11Device*							Sprite3D::m_pDevice = nullptr;				// �豸
ID3D11DeviceContext*					Sprite3D::m_pDeviceContext = nullptr;		// �豸������
ID3D11InputLayout*						Sprite3D::m_pInputLayout = nullptr;			// ���벼��
ID3DX11EffectTechnique*					Sprite3D::m_pTech = nullptr;				// Technique
																					// ����������������
XMFLOAT4X4								Sprite3D::m_View;							// �۲����
XNA::Frustum							Sprite3D::m_CamFrustum;						// �������׶��

Sprite3D::~Sprite3D()
{
	ReleaseCOM(m_pInstanceBuffer);
	ReleaseCOM(m_pVertexBuffer);
	ReleaseCOM(m_pTexture);
}

void Sprite3D::Initialize(ID3D11DeviceContext* devContext, ID3DX11Effect * pFX)
{
	if (!m_pDeviceContext)
	{
		m_pDeviceContext = devContext;
		m_pDeviceContext->GetDevice(&m_pDevice);
	}
	if (!m_pTech)
		m_pTech = pFX->GetTechniqueByName("Sprite3D");
	if (!m_pFXTexture)
		m_pFXTexture = pFX->GetVariableByName("gTexture")->AsShaderResource();
	if (!m_pFXMaterial)
		m_pFXMaterial = pFX->GetVariableByName("gMaterial");
	if (!m_pFXWorld)
		m_pFXWorld = pFX->GetVariableByName("gWorld")->AsMatrix();
	if (!m_pFXWorldInvTranspose)
		m_pFXWorldInvTranspose = pFX->GetVariableByName("gWorldInvTranspose")->AsMatrix();

	D3D11_INPUT_ELEMENT_DESC inputLayoutDesc[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "WORLD", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 0, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "WORLD", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "WORLD", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 32, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "WORLD", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 48, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "WORLDINVT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 64, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "WORLDINVT", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 80, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "WORLDINVT", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 96, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "WORLDINVT", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 112, D3D11_INPUT_PER_INSTANCE_DATA, 1 }
	};
	// ��ʼ�����벼��
	D3DX11_PASS_DESC passDesc;
	m_pTech->GetPassByIndex(0)->GetDesc(&passDesc);
	HR(m_pDevice->CreateInputLayout(inputLayoutDesc, 11, passDesc.pIAInputSignature,
		passDesc.IAInputSignatureSize, &m_pInputLayout));
}

void Sprite3D::UpdateViewMatrix(const XMFLOAT4X4 & view)
{
	m_View = view;
}

void Sprite3D::UpdateFrustum(const XMFLOAT4X4 & proj)
{
	// �����������׶��
	XNA::ComputeFrustumFromProjection(&m_CamFrustum, &XMLoadFloat4x4(&proj));
}

void Sprite3D::Release()
{
	ReleaseCOM(m_pDevice);
	ReleaseCOM(m_pInputLayout);
}

void Sprite3D::InitModel(const char * objPath, const char * texPath, const char * mtlPath)
{
	// ��ʼ����ɫ����Դ��ͼ(����)
	HR(D3DX11CreateShaderResourceViewFromFileA(m_pDevice, texPath, 0, 0, &m_pTexture, 0));

	// ��ʼ��ģ��
	int v, vn, vt, num = 0;
	std::vector<XMFLOAT3> position;
	std::vector<XMFLOAT3> normal;
	std::vector<XMFLOAT2> texCoord;

	std::vector<Vertex3D> vertices;
	char type[2048];
	FILE * fp = fopen(objPath, "r");
	XMFLOAT3 f3;
	XMFLOAT2 f2;

	// ���ڹ���AABB��
	XMFLOAT3 vMinf3(+MathHelper::Infinity, +MathHelper::Infinity, +MathHelper::Infinity);
	XMFLOAT3 vMaxf3(-MathHelper::Infinity, -MathHelper::Infinity, -MathHelper::Infinity);
	XMVECTOR vMin = XMLoadFloat3(&vMinf3);
	XMVECTOR vMax = XMLoadFloat3(&vMaxf3);

	while (!feof(fp))				//�����ļ�β
	{
		fscanf_s(fp, "%s", type, 2048);	//���˶����ַ�
		if (!strcmp(type, "v"))
		{
			fscanf_s(fp, "%f%f%f", &f3.x, &f3.y, &f3.z);
			position.push_back(f3);

			XMVECTOR P = XMLoadFloat3(&f3);

			vMin = XMVectorMin(vMin, P);
			vMax = XMVectorMax(vMax, P);
		}
		else if (!strcmp(type, "vt"))
		{
			fscanf_s(fp, "%f%f", &f2.x, &f2.y);
			f2.y = 1 - f2.y;
			texCoord.push_back(f2);
		}
		else if (!strcmp(type, "vn"))
		{
			fscanf_s(fp, "%f%f%f", &f3.x, &f3.y, &f3.z);
			normal.push_back(f3);
		}
		else if (!strcmp(type, "f"))
		{
			fscanf_s(fp, "%d/%d/%d", &v, &vt, &vn);
			vertices.push_back(Vertex3D{ position[v - 1], normal[vn - 1], texCoord[vt - 1] });

			fscanf_s(fp, "%d/%d/%d", &v, &vt, &vn);
			vertices.push_back(Vertex3D{ position[v - 1], normal[vn - 1], texCoord[vt - 1] });

			fscanf_s(fp, "%d/%d/%d", &v, &vt, &vn);
			vertices.push_back(Vertex3D{ position[v - 1], normal[vn - 1], texCoord[vt - 1] });

			num += 3;
		}
	}
	fclose(fp);

	// ����AABB��
	XMStoreFloat3(&m_Sprite3DBox.Center, 0.5f*(vMin + vMax));
	XMStoreFloat3(&m_Sprite3DBox.Extents, 0.5f*(vMax - vMin));
	// ��Ƕ�����Ŀ
	m_VertexCount = vertices.size();
	// ��ʼ�������������������㻺����
	D3D11_BUFFER_DESC vertexBuffDesc;
	vertexBuffDesc.Usage = D3D11_USAGE_IMMUTABLE;
	vertexBuffDesc.ByteWidth = sizeof(Vertex3D) * m_VertexCount;
	vertexBuffDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBuffDesc.CPUAccessFlags = 0;
	vertexBuffDesc.MiscFlags = 0;
	vertexBuffDesc.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA vertexInitData;
	vertexInitData.pSysMem = &vertices[0];
	HR(m_pDevice->CreateBuffer(&vertexBuffDesc, &vertexInitData, &m_pVertexBuffer));


	// ����ʵ��������
	D3D11_BUFFER_DESC instanceBuffDesc;
	instanceBuffDesc.Usage = D3D11_USAGE_DYNAMIC;
	instanceBuffDesc.ByteWidth = sizeof(Instance) * 100;
	instanceBuffDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	instanceBuffDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	instanceBuffDesc.MiscFlags = 0;
	instanceBuffDesc.StructureByteStride = 0;
	HR(m_pDevice->CreateBuffer(&instanceBuffDesc, nullptr, &m_pInstanceBuffer));
	// ��ʼ������
	fp = fopen(mtlPath, "r");
	while (fscanf(fp, "%s", type) && strcmp(type, "Ka"))
		continue;
	fscanf(fp, "%f%f%f", &m_Material.Ambient.x, &m_Material.Ambient.y, &m_Material.Ambient.z);
	m_Material.Ambient.w = 1.0f;
	fscanf(fp, "%*s%f%f%f", &m_Material.Diffuse.x, &m_Material.Diffuse.y, &m_Material.Diffuse.z);
	m_Material.Diffuse.w = 1.0f;
	fscanf(fp, "%*s%f%f%f", &m_Material.Specular.x, &m_Material.Specular.y, &m_Material.Specular.z);
	m_Material.Specular.w = 1.0f;
	fclose(fp);
}

void Sprite3D::UpdateWorldMatrixes(const std::vector<XMFLOAT4X4>& worldMatrixes)
{
	int sz = worldMatrixes.size();
	m_Instances.clear();

	XMMATRIX xmView = XMLoadFloat4x4(&m_View);
	XMVECTOR detView = XMMatrixDeterminant(xmView);
	XMMATRIX xmInvView = XMMatrixInverse(&detView, xmView);

	for (int i = 0; i < sz; ++i)
	{
		XMMATRIX xmWorld = XMLoadFloat4x4(&worldMatrixes[i]);
		XMMATRIX xmInvWorld = XMMatrixInverse(&XMMatrixDeterminant(xmWorld), xmWorld);

		// �ӹ۲�����ϵ��ʵ���ľֲ�����ϵ
		XMMATRIX xmToLocal = xmInvView * xmInvWorld;
		// ���������ֽ��3�������Ĳ���
		XMVECTOR scale, rotQuat, translation;
		XMMatrixDecompose(&scale, &rotQuat, &translation, xmToLocal);

		// ���������׶��ӹ۲�����ϵת����ʵ���ľֲ�����ϵ
		XNA::Frustum localspaceFrustum;
		XNA::TransformFrustum(&localspaceFrustum, &m_CamFrustum, XMVectorGetX(scale), rotQuat, translation);

		// ������ײ���ԣ�AABB������Ƶ��ֲ�����ϵ����׶�壩
		// �����ص����֣��������
		if (XNA::IntersectAxisAlignedBoxFrustum(&m_Sprite3DBox, &localspaceFrustum) != 0)
		{
			m_Instances.push_back(Instance{ worldMatrixes[i] });
			XMStoreFloat4x4(&m_Instances.back().WorldInvTranspose,
				MathHelper::InverseTranspose(XMLoadFloat4x4(&worldMatrixes[i])));
		}



	}
}

void Sprite3D::DrawInstances()
{
	if (m_Instances.empty())
		return;
	// ����װ��׶�����
	m_pDeviceContext->IASetInputLayout(m_pInputLayout);
	m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	// ͬʱʹ��2�������
	UINT stride[2] = { sizeof(Vertex3D), sizeof(Instance) };
	UINT offset[2] = { 0, 0 };
	ID3D11Buffer* buffers[2] = { m_pVertexBuffer, m_pInstanceBuffer };
	m_pDeviceContext->IASetVertexBuffers(0, 2, buffers, stride, offset);
	// ��������Ͳ���
	m_pFXTexture->SetResource(m_pTexture);
	m_pFXMaterial->SetRawValue(&m_Material, 0, sizeof(Material));
	// �޸�ʵ��������
	D3D11_MAPPED_SUBRESOURCE mappedData;
	m_pDeviceContext->Map(m_pInstanceBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData);
	memcpy_s(mappedData.pData, sizeof(Instance) * 100,
		m_Instances.data(), sizeof(Instance) * m_Instances.size());
	m_pDeviceContext->Unmap(m_pInstanceBuffer, 0);
	// Ӧ�ò�����
	m_pTech->GetPassByIndex(0)->Apply(0, m_pDeviceContext);
	m_pDeviceContext->DrawInstanced(m_VertexCount, m_Instances.size(), 0, 0);
}

