//ver.1.0:����by X_Jun
//ver.1.1:��Camera�ӿڽǶ� 0.5*Pi �ĳ� 0.25*Pi ����������Ч����
//             ��~GameApp()�������	ReleaseStatic(LightManager) �ͷŵƹ������
//             ͷ�ļ���ӡ�UI.h��
//				 private��Ա��� m_GameStatus�����Ϸ״̬

//ver 2.0                           by ������   2017/8/25
//���汾������22��,ʵ�ֻ��Ƶ��Σ���ͼ����պУ��ֵ�
//���ӳ����ͽֵ���;
//����InitScene()��InitStreet();
//UpdateScene()�е���m_StreetManager.UpdateScene();
//DrawScene()�е���:	
//XMFLOAT4X4 ViewProj;
//XMStoreFloat4x4(&ViewProj, m_Camera.GetXMViewProj());
//m_Scene.draw(ViewProj, m_Camera.GetXMFloat3Pos(), trans);
//m_StreetManager.Draw();
//
//������ļ�:
//RenderStates.h,.cpp
//Vertex.h,.cpp;
//Effects.h,.cpp;
//Sky.h,.cpp;
//Terrain.h,.cpp;
//GeometryGenerator.h,.cpp;
//SkyAndTerrain.h,.cpp;
//StreetManager.h,.cpp;
//HLSL:
//Grid.hlsl;
//Sky.hlsl;
//Terrain.hlsl;
//obj�ļ��У���ͼ�ļ��У������ļ���

#ifndef _GAMEAPP_H_
#define _GAMEAPP_H_

#include "d3dApp.h"
#include "Camera.h"
#include "MKInput.h"
#include "Sprite3D.h"
#include "LightManager.h"
#include "SkyAndTerrain.h"
#include "StreetManager.h"

class GameApp : public D3DApp
{
public:
	GameApp(HINSTANCE hInstance);
	~GameApp();

	bool Init();

	//������ʼ��
	void InitScene();
	//�ֵ���ʼ��
	void InitStreet();
	void InitFX();
	void InitLight();
	void InitModel();
	void InitDInput();
	void InitText();
	void InitRenderState();
	void InitCamera();

	void OnResize();
	void UpdateScene(float dt);
	void UpdateCamera(float dt);
	void DrawScene();
	void Draw2D();



private:

	Camera										m_Camera;				    // ��Ӱ��
	D2DText									m_Font;							// ������					


	LightManager*						m_pLightManager;		// �ƹ������
	MKInput*									m_pMKInput;				// ������

	ID3DX11Effect*						m_pFXBasic;				// Effects Basic.fx
	ID3DX11EffectTechnique*	m_pTech;						// Technique

	ID3D11RasterizerState*		m_NoCullRS;				// �ޱ�������

	Rich::Sprite3D							m_Sprite;					    // ����

	int												m_GameStatus;			//��Ϸ״̬

	Rich::SkyAndTerrain m_Scene;									//��պУ����Σ�����
	Rich::StreetManager  m_StreetManager;					//�ֵ�����
};

#endif