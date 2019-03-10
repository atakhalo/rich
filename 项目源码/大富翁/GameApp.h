//ver.1.0:����                                                 By X_Jun
//ver.1.1:��Camera�ӿڽǶ� 0.5*Pi �ĳ� 0.25*Pi ����������Ч����
//             ��~GameApp()�������	ReleaseStatic(LightManager) �ͷŵƹ������
//             ͷ�ļ���ӡ�UI.h��
//				 private��Ա��� m_GameStatus�����Ϸ״̬
//             ������˵�UI                                 By Chitose Ruri
//ver.1.2:����Initsock.hͷ�ļ���֧��������                By X_Jun
//ver.2.0:��Ӵ����߼�������UpdateUI()��ButtonClick()                By Chitose Ruri
//ver.2.1:���������ƶ��Լ���UI�Ľ���						By ¬��͢

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

	//  ���ݵ�ǰ��·�����뷽��ȷ������ĵ�ǰ������������һ������������
	void UpDatePlayerPosition(int ID);
	//  ����ƶ����ԭ���ĵ�·��������Ϊ��һ���Ľֵ�����
	void UpDatePlayerBlockIndex(int ID);
	//  �������ߺ󣬸���������һ����Ϣ
	void UpddatePlayerNextStep();

private:
	enum GameStatus
	{
		MAIN_MENU,							//���˵�
		CREATE_ROOM,						//��������
		JOIN_ROOM,								//���뷿��
		GAME_USUAL,							//��Ϸ�ڽ���
		BUY_LAND,									//��������
		UPGRADE_LAND,						//���������ϵķ���
		STORE,											//�̵�
		STEAM,											//����
		ROULETTE,									//���̿�����
		EVENTS,										//�¼���������
		CHOOSE_PLAYER						//ѡ����ҽ���
	};

	enum Events
	{
		EVENT_PEACE_NIGHT,															//ƽ��ҹ
		EVENT_RANDOM_HOUSE_UPGRADE,							//�����������
		EVENT_RANDOM_HOUSE_DEGRADE,							//������ݽ���
		EVENT_AWARD_IN_ROTTERY,											//���Ʊ��
		EVENT_INVEST_FAILED,														//Ͷ��ʧ��
		EVENT_EARTHQUAKE,															//�����ֺ�
		EVENT_POLICY_ENCOURAGE,											//���߹���
		EVENT_INFLATION,																//ͨ������
		EVENT_POLICY_RETRENCH,												//���߽���
		EVENT_SELL_ALL_CARDS,													//�������п���
		EVENT_ENVIRONMENT_POLLUTION,								//������Ⱦ
		EVENT_HEATING_HOUSE_SELL,										//�����ȳ�
		EVENT_RECHARGE_SUCCESS,											//��ֵ�ɹ�
		EVENT_HELP_MYSTERY,														//����������
		EVENT_HELP_OLDS																//��������
	};

	Initsock*								m_pInitsock;																				// ��ʼ��Winsock

	Camera									m_Camera;																				   // ��Ӱ��
	D2DText									m_Font;																			       // ������					

	Rich::MessageManager * m_pMessageManager;
	Rich::PlayerManager m_PlayerManager;

	LightManager*							m_pLightManager;																  // �ƹ������
	MKInput*								m_pMKInput;																			  // ������

	ID3DX11Effect*							m_pFXBasic;																     // Effects Basic.fx
	ID3DX11Effect*							m_pFX2D;																		 // Effects color.fx
	ID3DX11EffectTechnique*					m_pTech;																 // Technique3D
	ID3DX11EffectTechnique*					m_pTech2D;														 // Technique2D
	ID3D11RasterizerState*					m_NoCullRS;																 // �ޱ�������
	ID3D11BlendState*						m_AlphaToCoverageBS;												 // Alpha���
	Sprite3D								m_Sprite;																					 // ����
	Rich::RichCardManager * m_pCards;																		    	//������Ϣ
	int										m_GameStatus;																			// ��Ϸ״̬

	std::vector<DX11UI::UI*>				m_UIManager;														// UI����
	DX11UI::UI								m_UI[11];	   																			// ����UI
	ID3D11InputLayout*						m_p2DInputLayout;													// 2D����
	std::vector<ID3D11ShaderResourceView*>	m_pUITexture;										// ����UI����

	//UI����ʱ�����ֿ���
	std::vector<int> m_UIDrawsBegin;																					//UI��������
	std::vector<int> m_UIDrawsBack;
	std::vector<int> m_UIDraws;
	std::vector<std::wstring>	 m_UIWords;																			//UI���ִ���
	



	unsigned m_Roulette;																											//������ת��,���̵���Ϊ6-��m_Roulette % 360��/ 60
	bool m_IsRouletteRotate;																										//ת���Ƿ�ת��
	int m_CardInStore[8];																											//�̵���ʾ�Ŀ������
	int m_SteamLess;																													//����ʣ�����
	int m_LastTime;																														//�ϴδ���ʱ��
	int m_RoundTimeLess;																											//�غ�ʣ��ʱ��
	int m_NumButtonReturn[8];																								//��ť��Ӧ����ֵ
	int m_PlayerChoose;																												//ѡ�����
	int m_CardNeedUI;																												//����ҽ��в����Ŀ���
	Rich::SkyAndTerrain m_Scene;																							//��պУ����Σ�����
	Rich::StreetManager  m_StreetManager;																			//�ֵ�����
	int m_LastPlayerID;																												//��һ֡���ID
};

#endif