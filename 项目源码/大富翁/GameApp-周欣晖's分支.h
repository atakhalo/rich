//ver.1.0:构造by X_Jun
//ver.1.1:把Camera视口角度 0.5*Pi 改成 0.25*Pi 消除“鱼眼效果”
//             在~GameApp()里面添加	ReleaseStatic(LightManager) 释放灯光管理者
//             头文件添加“UI.h”
//				 private成员添加 m_GameStatus标记游戏状态

//ver 2.0                           by 周欣晖   2017/8/25
//本版本加载需22秒,实现绘制地形，地图，天空盒，街道
//增加场景和街道类;
//增加InitScene()和InitStreet();
//UpdateScene()中调用m_StreetManager.UpdateScene();
//DrawScene()中调用:	
//XMFLOAT4X4 ViewProj;
//XMStoreFloat4x4(&ViewProj, m_Camera.GetXMViewProj());
//m_Scene.draw(ViewProj, m_Camera.GetXMFloat3Pos(), trans);
//m_StreetManager.Draw();
//
//需更新文件:
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
//obj文件夹，贴图文件夹，材质文件夹

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

	//场景初始化
	void InitScene();
	//街道初始化
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

	Camera										m_Camera;				    // 摄影机
	D2DText									m_Font;							// 字体类					


	LightManager*						m_pLightManager;		// 灯光管理者
	MKInput*									m_pMKInput;				// 输入类

	ID3DX11Effect*						m_pFXBasic;				// Effects Basic.fx
	ID3DX11EffectTechnique*	m_pTech;						// Technique

	ID3D11RasterizerState*		m_NoCullRS;				// 无背面消隐

	Rich::Sprite3D							m_Sprite;					    // 精灵

	int												m_GameStatus;			//游戏状态

	Rich::SkyAndTerrain m_Scene;									//天空盒，地形，地面
	Rich::StreetManager  m_StreetManager;					//街道管理
};

#endif