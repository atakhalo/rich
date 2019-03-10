/*
* Sprite2D类
* 作者：Chitose Ruri
* 创建日期：2017-8-22
* 详述：
*     该2D精灵类仅用于2D图片的展示，用户自定义图片大小后图片中心点默认在屏幕中心
*	   用户需要传入已经确定的纹理指针，可以对图片进行平移、旋转、椭圆旋转、缩放
* 
*     ①先进行静态函数的调用初始化Sprite2D：在GameApp::InitModel()中调用Sprite2D::InitAll()
 *	   ②设置Sprite2D: 调用Init()设置图片大小，调用SetDeep()设置深度，调用SetTexture()传入纹理指针。
*	
*
*/#pragma once
#include "d3dApp.h"
#include "d3dx11effect.h"
#include "MathHelper.h"
#include <math.h>


	struct Vertex2D
	{
		Vertex2D() {}
		Vertex2D(const XMFLOAT3 &pos, const XMFLOAT2 &tex)
			:position(pos), texCoord(tex) {}
		Vertex2D(float positionX, float positionY, float positionZ, float texCoordU, float CoordV)
			:position(positionX, positionY, positionZ), texCoord(texCoordU, CoordV) {}
		XMFLOAT3 position;
		XMFLOAT2 texCoord;
	};

	class Sprite2D
	{
	public:
		Sprite2D();
		~Sprite2D();
		//静态初始化Sprite2D类
		static void InitAll(ID3D11Device * d3DDevice, ID3D11DeviceContext * d3DDeviceContext, ID3DX11Effect * fX,
			ID3D11InputLayout * pInputLayout, ID3D11BlendState* pAlphaToCoverageBS, HWND hwnd);
		//静态销毁Sprite2D类
		static void Release();
		//传入纹理指针
		void SetTexture(ID3D11ShaderResourceView * texture);
		//初始化该精灵
		void Init(XMFLOAT2 size);
		//设置深度
		void SetDeep(float deep);
		//绘制
		void Draw();
		//平移
		void Translate(float x, float y);
		//旋转
		void Rotate(float radian);
		//矩形旋转
		void RotateElliptical(float radian);
		//缩放
		void Scale(float x, float y);
		//重置转换信息
		void ReSet();

	private:
		XMMATRIX XMMatrixRotationEllipseZ(float radio, float radian);
	private:

		static ID3D11Device * m_pD3dDevice;
		static ID3D11DeviceContext * m_pD3dContext;
		static LPD3D11EFFECTMATRIXVARIABLE m_pWorldEffect2D;
		static LPD3D11EFFECTTECHNIQUE m_pTechniqueEffect2D;
		static LPD3D11EFFECTSHADERRESOURCEVARIABLE m_pTextureEffect2D;
		static ID3D11InputLayout * m_pInputLayout2D;
		static ID3D11BlendState* m_pAlphaToCoverageBS;
		static HWND m_Hwnd;

		ID3D11ShaderResourceView * m_pTexture;
		ID3D11Buffer * m_pVertexBuff;
		ID3D11Buffer * m_pIndexBuff;
		XMFLOAT2 m_LeftTopLocation;
		XMFLOAT2 m_Size;

		XMMATRIX m_World;
		XMMATRIX m_Rotation;
		XMFLOAT2 m_Scale;
		XMFLOAT3 m_Translation;


		bool m_IsChange;
		UINT stride;
		UINT offset;
	};
