#ifndef _SPRITE3D_H_
#define _SPRITE3D_H_
#include "d3dUtil.h"
#include "xnacollision.h"

/*
 * Sprite3D类
 * 作者：X_Jun
 *
 * 更新：
 * 2017-8-24：修正绘制模型只能绘制一个的问题（修改Draw为DrawInstanced），
 *            以及在静态方法Release()中补上了ReleaseCOM(m_pDevice);
 *                                                          -By X_Jun
 * 2017-8-24：删除名称空间Rich，注意在GameApp.cpp中修改     -By X_Jun
 *
 * 创建日期：2017-8-22
 * 详述：
 *     该3D精灵类仅用于读取并显示已经编写好的网格模型。
 * 一个网格模型的读取需要 .obj文件  .png文件  和 .mtl文件，
 * 分别存放于Object文件夹  Texture文件夹  Material文件夹中。
 *     要使用Sprite3D类，首先需要按如下步骤进行静态函数的调
 * 用：
 *     ①初始化：在GameApp::InitModel()中调用Sprite3D::Initialize()
 *     ②获取视锥体：在GameApp::OnResize()中摄像机更新投影矩阵后调用
 * Sprite3D::UpdateFrustum()
 *     ③获取观察矩阵：在GameApp::Update()中摄像机更新观察矩阵后调用
 * Sprite3D::UpdateViewMatrix()
 *     ④释放Sprite3D：在GameApp::~GameApp()中调用
 * ReleaseStatic(Sprite3D)
 * 
 *     要绘制精灵，首先需要按如下步骤进行对象操作：
 *     ①初始化模型：在GameApp::InitModel()中已经初始化Sprite3D后调用
 * 方法Sprite3D::InitModel()
 *     ②更新世界矩阵：在GameApp::UpdateScene()中调用方法
 * Sprite3D::UpdateWorldMatrixes(); 传入的世界矩阵个数为需要绘制的实例
 * 数目，所以世界矩阵应该在别的地方就构造好。
 *     ③进行绘制：在GameApp::DrawScene()中进行绘制的地方调用方法
 * Sprite3D::DrawInstances(); 
 * 
 *
 */



 // 3D精灵的顶点结构体
struct Vertex3D {
	XMFLOAT3 Pos;		// 顶点坐标
	XMFLOAT3 Normal;	// 法向量
	XMFLOAT2 Tex;		// 纹理坐标
};

// 3D精灵的实例结构体
struct Instance {
	XMFLOAT4X4 World;				// 世界矩阵
	XMFLOAT4X4 WorldInvTranspose;	// 世界矩阵倒置转置
};

class Sprite3D {
public:
	Sprite3D()
		: m_pVertexBuffer(nullptr), m_pInstanceBuffer(nullptr), m_pTexture(nullptr)
	{
	}
	~Sprite3D();

	// 初始化输入布局、Technique和Effects纹理
	static void Initialize(ID3D11DeviceContext* devContext, ID3DX11Effect* pFX);

	// 更新观察矩阵（每一帧只需要更新一次）
	static void UpdateViewMatrix(const XMFLOAT4X4& view);

	// 更新取得视锥体（只有投影矩阵改变的时候需要更新一次）
	static void UpdateFrustum(const XMFLOAT4X4& proj);

	// 释放输入布局
	static void Release();

	// 初始化模型，需提供OBJ文件、纹理的PNG文件和材质MTL文件
	void InitModel(const char * objPath, const char * texPath, const char * mtlPath);

	// 更新世界矩阵数组并存储（这里不需要提供世界的倒置转置矩阵，在内部完成运算）
	void UpdateWorldMatrixes(const std::vector<XMFLOAT4X4>& worldMatrixes);

	// 绘制所有实例（必须要先更新世界矩阵数组）
	void DrawInstances();

private:
	static ID3DX11EffectShaderResourceVariable* m_pFXTexture;		// Effects纹理
	static ID3DX11EffectVariable*				m_pFXMaterial;		// Effects材质
	static ID3DX11EffectMatrixVariable*			m_pFXWorld;			// Effects世界矩阵
	static ID3DX11EffectMatrixVariable*			m_pFXWorldInvTranspose;// Effects世界倒置转置


	static ID3D11Device*						m_pDevice;			// 设备
	static ID3D11DeviceContext*					m_pDeviceContext;	// 设备上下文
	static ID3D11InputLayout*					m_pInputLayout;		// 输入布局
	static ID3DX11EffectTechnique*				m_pTech;			// Technique

	static XMFLOAT4X4							m_View;				// 观察矩阵
	static XNA::Frustum							m_CamFrustum;		// 摄像机视锥体


	ID3D11Buffer*								m_pVertexBuffer;	// 顶点缓冲区
	size_t										m_VertexCount;		// 顶点数目
	ID3D11Buffer*								m_pInstanceBuffer;	// 实例缓冲区
	ID3D11ShaderResourceView*					m_pTexture;			// 纹理
	Material									m_Material;			// 材质

	std::vector<Instance>						m_Instances;		// 实例数组
	XNA::AxisAlignedBox							m_Sprite3DBox;		// AABB盒


};


#endif	// _SPRITE_H_ End