#ifndef _SPRITE3D_H_
#define _SPRITE3D_H_
#include "d3dUtil.h"
#include "xnacollision.h"

/*
 * Sprite3D��
 * ���ߣ�X_Jun
 *
 * ���£�
 * 2017-8-24����������ģ��ֻ�ܻ���һ�������⣨�޸�DrawΪDrawInstanced����
 *            �Լ��ھ�̬����Release()�в�����ReleaseCOM(m_pDevice);
 *                                                          -By X_Jun
 * 2017-8-24��ɾ�����ƿռ�Rich��ע����GameApp.cpp���޸�     -By X_Jun
 *
 * �������ڣ�2017-8-22
 * ������
 *     ��3D����������ڶ�ȡ����ʾ�Ѿ���д�õ�����ģ�͡�
 * һ������ģ�͵Ķ�ȡ��Ҫ .obj�ļ�  .png�ļ�  �� .mtl�ļ���
 * �ֱ�����Object�ļ���  Texture�ļ���  Material�ļ����С�
 *     Ҫʹ��Sprite3D�࣬������Ҫ�����²�����о�̬�����ĵ�
 * �ã�
 *     �ٳ�ʼ������GameApp::InitModel()�е���Sprite3D::Initialize()
 *     �ڻ�ȡ��׶�壺��GameApp::OnResize()�����������ͶӰ��������
 * Sprite3D::UpdateFrustum()
 *     �ۻ�ȡ�۲������GameApp::Update()����������¹۲��������
 * Sprite3D::UpdateViewMatrix()
 *     ���ͷ�Sprite3D����GameApp::~GameApp()�е���
 * ReleaseStatic(Sprite3D)
 * 
 *     Ҫ���ƾ��飬������Ҫ�����²�����ж��������
 *     �ٳ�ʼ��ģ�ͣ���GameApp::InitModel()���Ѿ���ʼ��Sprite3D�����
 * ����Sprite3D::InitModel()
 *     �ڸ������������GameApp::UpdateScene()�е��÷���
 * Sprite3D::UpdateWorldMatrixes(); ���������������Ϊ��Ҫ���Ƶ�ʵ��
 * ��Ŀ�������������Ӧ���ڱ�ĵط��͹���á�
 *     �۽��л��ƣ���GameApp::DrawScene()�н��л��Ƶĵط����÷���
 * Sprite3D::DrawInstances(); 
 * 
 *
 */



 // 3D����Ķ���ṹ��
struct Vertex3D {
	XMFLOAT3 Pos;		// ��������
	XMFLOAT3 Normal;	// ������
	XMFLOAT2 Tex;		// ��������
};

// 3D�����ʵ���ṹ��
struct Instance {
	XMFLOAT4X4 World;				// �������
	XMFLOAT4X4 WorldInvTranspose;	// ���������ת��
};

class Sprite3D {
public:
	Sprite3D()
		: m_pVertexBuffer(nullptr), m_pInstanceBuffer(nullptr), m_pTexture(nullptr)
	{
	}
	~Sprite3D();

	// ��ʼ�����벼�֡�Technique��Effects����
	static void Initialize(ID3D11DeviceContext* devContext, ID3DX11Effect* pFX);

	// ���¹۲����ÿһֻ֡��Ҫ����һ�Σ�
	static void UpdateViewMatrix(const XMFLOAT4X4& view);

	// ����ȡ����׶�壨ֻ��ͶӰ����ı��ʱ����Ҫ����һ�Σ�
	static void UpdateFrustum(const XMFLOAT4X4& proj);

	// �ͷ����벼��
	static void Release();

	// ��ʼ��ģ�ͣ����ṩOBJ�ļ��������PNG�ļ��Ͳ���MTL�ļ�
	void InitModel(const char * objPath, const char * texPath, const char * mtlPath);

	// ��������������鲢�洢�����ﲻ��Ҫ�ṩ����ĵ���ת�þ������ڲ�������㣩
	void UpdateWorldMatrixes(const std::vector<XMFLOAT4X4>& worldMatrixes);

	// ��������ʵ��������Ҫ�ȸ�������������飩
	void DrawInstances();

private:
	static ID3DX11EffectShaderResourceVariable* m_pFXTexture;		// Effects����
	static ID3DX11EffectVariable*				m_pFXMaterial;		// Effects����
	static ID3DX11EffectMatrixVariable*			m_pFXWorld;			// Effects�������
	static ID3DX11EffectMatrixVariable*			m_pFXWorldInvTranspose;// Effects���絹��ת��


	static ID3D11Device*						m_pDevice;			// �豸
	static ID3D11DeviceContext*					m_pDeviceContext;	// �豸������
	static ID3D11InputLayout*					m_pInputLayout;		// ���벼��
	static ID3DX11EffectTechnique*				m_pTech;			// Technique

	static XMFLOAT4X4							m_View;				// �۲����
	static XNA::Frustum							m_CamFrustum;		// �������׶��


	ID3D11Buffer*								m_pVertexBuffer;	// ���㻺����
	size_t										m_VertexCount;		// ������Ŀ
	ID3D11Buffer*								m_pInstanceBuffer;	// ʵ��������
	ID3D11ShaderResourceView*					m_pTexture;			// ����
	Material									m_Material;			// ����

	std::vector<Instance>						m_Instances;		// ʵ������
	XNA::AxisAlignedBox							m_Sprite3DBox;		// AABB��


};


#endif	// _SPRITE_H_ End