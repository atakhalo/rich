//ver.1.0:构造                                                 By X_Jun
//ver.1.1:把Camera视口角度 0.5*Pi 改成 0.25*Pi 消除“鱼眼效果”
//             在~GameApp()里面添加	ReleaseStatic(LightManager) 释放灯光管理者
//             头文件添加“UI.h”
//				 private成员添加 m_GameStatus标记游戏状态
//             添加主菜单UI                                 By Chitose Ruri
//ver.1.2:加入Initsock.h头文件以支持网络编程                By X_Jun
//ver.2.0:添加大量逻辑处理在UpdateUI()和ButtonClick()                By Chitose Ruri
//ver.2.1:增加人物移动以及跟UI的交互						By 卢俊廷

#ifndef _GAMEAPP_H_
#define _GAMEAPP_H_

#include "Initsock.h"
#include "d3dApp.h"
#include "Camera.h"
#include "MKInput.h"
#include "Sprite3D.h"
#include "LightManager.h"
#include "MessageManager.h"
#include "SkyAndTerrain.h"
#include "StreetManager.h"
#include "UI.h"


class GameApp : public D3DApp
{
public:
	GameApp(HINSTANCE hInstance);
	~GameApp();

	bool Init();

private:
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
	void InitBlend();
	void InitUI();
	void InitUIWordsType();

	void OnResize();
	void UpdateScene(float dt);
	void UpdateCamera(float dt);
	void UpdateUI(float dt);

	void DrawScene();
	void Draw2D();

	void ButtonClick();
	void CardEffect(int card);
	void EventEffect(int events);
	std::wstring CharToWstring(LPCSTR pszSrc, int nLen);

	//  根据当前道路索引与方向确定人物的当前世界坐标与下一步的世界坐标
	void UpDatePlayerPosition(int ID);
	//  玩家移动后把原来的道路索引设置为下一步的街道索引
	void UpDatePlayerBlockIndex(int ID);
	//  人物行走后，更新人物下一步信息
	void UpddatePlayerNextStep();

private:
	enum GameStatus
	{
		MAIN_MENU,							//主菜单
		CREATE_ROOM,						//创建房间
		JOIN_ROOM,								//加入房间
		GAME_USUAL,							//游戏内界面
		BUY_LAND,									//购买土地
		UPGRADE_LAND,						//升级土地上的房屋
		STORE,											//商店
		STEAM,											//黑市
		ROULETTE,									//轮盘卡界面
		EVENTS,										//事件触发界面
		CHOOSE_PLAYER						//选择玩家界面
	};

	enum Events
	{
		EVENT_PEACE_NIGHT,															//平安夜
		EVENT_RANDOM_HOUSE_UPGRADE,							//随机房屋升级
		EVENT_RANDOM_HOUSE_DEGRADE,							//随机房屋降级
		EVENT_AWARD_IN_ROTTERY,											//买彩票中
		EVENT_INVEST_FAILED,														//投资失败
		EVENT_EARTHQUAKE,															//地震灾害
		EVENT_POLICY_ENCOURAGE,											//政策鼓励
		EVENT_INFLATION,																//通货膨胀
		EVENT_POLICY_RETRENCH,												//政策紧缩
		EVENT_SELL_ALL_CARDS,													//变卖所有卡牌
		EVENT_ENVIRONMENT_POLLUTION,								//环境污染
		EVENT_HEATING_HOUSE_SELL,										//炒房热潮
		EVENT_RECHARGE_SUCCESS,											//充值成功
		EVENT_HELP_MYSTERY,														//救助神秘人
		EVENT_HELP_OLDS																//救助老人
	};

	Initsock*								m_pInitsock;																				// 初始化Winsock

	Camera									m_Camera;																				   // 摄影机
	D2DText									m_Font;																			       // 字体类					

	Rich::MessageManager * m_pMessageManager;
	Rich::PlayerManager m_PlayerManager;

	LightManager*							m_pLightManager;																  // 灯光管理者
	MKInput*								m_pMKInput;																			  // 输入类

	ID3DX11Effect*							m_pFXBasic;																     // Effects Basic.fx
	ID3DX11Effect*							m_pFX2D;																		 // Effects color.fx
	ID3DX11EffectTechnique*					m_pTech;																 // Technique3D
	ID3DX11EffectTechnique*					m_pTech2D;														 // Technique2D
	ID3D11RasterizerState*					m_NoCullRS;																 // 无背面消隐
	ID3D11BlendState*						m_AlphaToCoverageBS;												 // Alpha混合
	Sprite3D								m_Sprite;																					 // 精灵
	Rich::RichCardManager * m_pCards;																		    	//卡牌信息
	int										m_GameStatus;																			// 游戏状态

	std::vector<DX11UI::UI*>				m_UIManager;														// UI管理
	DX11UI::UI								m_UI[11];	   																			// 储存UI
	ID3D11InputLayout*						m_p2DInputLayout;													// 2D布局
	std::vector<ID3D11ShaderResourceView*>	m_pUITexture;										// 储存UI纹理

	//UI绘制时的文字控制
	std::vector<int> m_UIDrawsBegin;																					//UI文字索引
	std::vector<int> m_UIDrawsBack;
	std::vector<int> m_UIDraws;
	std::vector<std::wstring>	 m_UIWords;																			//UI文字储存
	



	unsigned m_Roulette;																											//轮盘旋转角,轮盘点数为6-（m_Roulette % 360）/ 60
	bool m_IsRouletteRotate;																										//转盘是否转动
	int m_CardInStore[8];																											//商店显示的卡的序号
	int m_SteamLess;																													//黑市剩余次数
	int m_LastTime;																														//上次触发时间
	int m_RoundTimeLess;																											//回合剩余时间
	int m_NumButtonReturn[8];																								//按钮对应返回值
	int m_PlayerChoose;																												//选择玩家
	int m_CardNeedUI;																												//对玩家进行操作的卡牌
	Rich::SkyAndTerrain m_Scene;																							//天空盒，地形，地面
	Rich::StreetManager  m_StreetManager;																			//街道管理
	int m_LastPlayerID;																												//上一帧玩家ID
};

#endif