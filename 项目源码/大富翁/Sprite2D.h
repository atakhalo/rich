/*
* Sprite2D��
* ���ߣ�Chitose Ruri
* �������ڣ�2017-8-22
* ������
*     ��2D�����������2DͼƬ��չʾ���û��Զ���ͼƬ��С��ͼƬ���ĵ�Ĭ������Ļ����
*	   �û���Ҫ�����Ѿ�ȷ��������ָ�룬���Զ�ͼƬ����ƽ�ơ���ת����Բ��ת������
* 
*     ���Ƚ��о�̬�����ĵ��ó�ʼ��Sprite2D����GameApp::InitModel()�е���Sprite2D::InitAll()
 *	   ������Sprite2D: ����Init()����ͼƬ��С������SetDeep()������ȣ�����SetTexture()��������ָ�롣
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
		//��̬��ʼ��Sprite2D��
		static void InitAll(ID3D11Device * d3DDevice, ID3D11DeviceContext * d3DDeviceContext, ID3DX11Effect * fX,
			ID3D11InputLayout * pInputLayout, ID3D11BlendState* pAlphaToCoverageBS, HWND hwnd);
		//��̬����Sprite2D��
		static void Release();
		//��������ָ��
		void SetTexture(ID3D11ShaderResourceView * texture);
		//��ʼ���þ���
		void Init(XMFLOAT2 size);
		//�������
		void SetDeep(float deep);
		//����
		void Draw();
		//ƽ��
		void Translate(float x, float y);
		//��ת
		void Rotate(float radian);
		//������ת
		void RotateElliptical(float radian);
		//����
		void Scale(float x, float y);
		//����ת����Ϣ
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
