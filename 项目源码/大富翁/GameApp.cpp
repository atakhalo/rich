#include "GameApp.h"
#include "XUtility.h"


GameApp::GameApp(HINSTANCE hInstance)
	: D3DApp(hInstance), m_pFXBasic(nullptr), m_pTech(nullptr)
{
	m_MainWndCaption = L"Rich";
	for (unsigned uidraws : m_UIDrawsBegin)				//初始化UI界面文字框数据
	{
		uidraws = 0;
	}
	m_GameStatus = MAIN_MENU;
	m_UIManager.push_back(&m_UI[0]);
	m_Roulette = 0;
	m_IsRouletteRotate = false;
	m_pInitsock = Initsock::GetInitsock();
	m_LastTime = timeGetTime();
	m_RoundTimeLess = 0;
	srand(time(nullptr));
	m_PlayerChoose = -1;
	m_LastPlayerID = 0;
	m_SteamLess = 0;
}

GameApp::~GameApp()
{
	ReleaseCOM(m_pFXBasic);
	ReleaseCOM(m_pFX2D);
	ReleaseCOM(m_NoCullRS);
	for (ID3D11ShaderResourceView * &texture : m_pUITexture)
	{
			ReleaseCOM(texture);
	}
	ReleaseCOM(m_p2DInputLayout);
	ReleaseCOM(m_AlphaToCoverageBS);

	ReleaseStatic(D2DText);
	ReleaseStatic(Sprite3D);
	ReleaseStatic(MKInput);
	ReleaseStatic(LightManager);
	ReleaseStatic(Initsock);
	ReleaseStatic(Rich::MessageManager);
	m_pCards->DestroyInstance();
}

bool GameApp::Init()
{
	if (!D3DApp::Init())
		return false;


	InitFX();
	InitLight();
	InitModel();
	InitDInput();
	InitText();
	InitCamera();
	InitRenderState();
	InitBlend();
	InitUIWordsType();
	InitUI();
	//街道和地图
	InitScene();
	InitStreet();

	m_pMessageManager = Rich::MessageManager::GetMessageManager();
	m_pMessageManager->Initialize("ipAddr.txt", &m_PlayerManager, &m_StreetManager);
	m_pMessageManager->Detach();
	m_pCards = Rich::RichCardManager::GetInstance();
	m_pCards->ReadCardFromFile("Cards.txt");

	return true;
}

void GameApp::InitScene()
{
	//获得这两个指针
	m_Scene.init(m_pD3DDevice, m_pD3DImmediateContext);
	//地形结构体
	Terrain::InitInfo tii;
	tii.HeightMapFilename = L"Texture/anothertest.raw";
	tii.LayerMapFilename0 = L"Texture/grass.dds";
	tii.LayerMapFilename1 = L"Texture/darkdirt.dds";
	tii.LayerMapFilename2 = L"Texture/stone.dds";
	tii.LayerMapFilename3 = L"Texture/lightdirt.dds";
	tii.LayerMapFilename4 = L"Texture/snow.dds";
	tii.BlendMapFilename = L"Texture/blend.dds";
	tii.HeightScale = 100.0f;
	tii.HeightmapWidth = 2049;
	tii.HeightmapHeight = 2049;
	tii.CellSpacing = 1.0f;
	tii.texCount = 5;
	m_Scene.SetTerrain(tii);
	m_Scene.SetSky(L"Texture/desertcube1024.dds", 5000.0f);
	m_Scene.SetGrid(L"Texture/small.jpg");
	m_Scene.SetGridTrans(200.0f, 0.1f, -200.0f);
	m_Scene.SetGridScale(4.0f, 0.0f, 4.0f);
}

void GameApp::InitStreet()
{
	m_StreetManager.initStreet("Street/街区.txt");
	m_StreetManager.Initialize();
	m_StreetManager.originalPointSet({ 45.0f,0.0f,-55.0f });
	m_StreetManager.scaleFactorSet(10.0f);
	m_StreetManager.ModelScaleSet(0.015f);
//#pragma region 示例
//	m_StreetManager.HouseOwnerSet({ 0,0 }, 1);
//	m_StreetManager.HouseTypeSet({ 0,0 }, Rich::COTTAGE);
//	m_StreetManager.HouseLevelSet({ 0,0 }, 1);
//
//	m_StreetManager.HouseOwnerSet({ 0,1 }, 2);
//	m_StreetManager.HouseTypeSet({ 0,1 }, Rich::COTTAGE);
//	m_StreetManager.HouseLevelSet({ 0,1 }, 2);
//
//	m_StreetManager.HouseOwnerSet({ 0,2 }, 1);
//	m_StreetManager.HouseTypeSet({ 0,2 }, Rich::COTTAGE);
//	m_StreetManager.HouseLevelSet({ 0,2 }, 3);
//
//	m_StreetManager.HouseOwnerSet({ 0,3 }, 1);
//	//m_StreetManager.HouseTypeSet({ 0,3 }, Rich::FLAT);
//	//m_StreetManager.HouseLevelSet({ 0,3 }, 5);
//
//	m_StreetManager.HouseOwnerSet({ 2,0 }, 1);
//	m_StreetManager.HouseTypeSet({ 2,0 }, Rich::COTTAGE);
//	m_StreetManager.HouseLevelSet({ 2,0 }, 1);
//
//
//	m_StreetManager.HouseOwnerSet({ 3,1 }, 1);
//	m_StreetManager.HouseTypeSet({ 3,1 }, Rich::COTTAGE);
//	m_StreetManager.HouseLevelSet({ 3,1 }, 2);
//
//	m_StreetManager.HouseOwnerSet({ 1,0 }, 1);
//	m_StreetManager.HouseTypeSet({ 1,0 }, Rich::FLAT);
//	m_StreetManager.HouseLevelSet({ 1,0 }, 1);
//
//	m_StreetManager.HouseOwnerSet({ 1,1 }, 3);
//	m_StreetManager.HouseTypeSet({ 1,1 }, Rich::FLAT);
//	m_StreetManager.HouseLevelSet({ 1,1 }, 2);
//
//	m_StreetManager.HouseOwnerSet({ 1,2 }, 4);
//	m_StreetManager.HouseTypeSet({ 1,2 }, Rich::FLAT);
//	m_StreetManager.HouseLevelSet({ 1,2 }, 3);
//
//	m_StreetManager.HouseOwnerSet({ 1,3 }, 1);
//	m_StreetManager.HouseTypeSet({ 1,3 }, Rich::FLAT);
//	m_StreetManager.HouseLevelSet({ 1,3 }, 4);
//
//	m_StreetManager.HouseOwnerSet({ 1,4 }, 1);
//	m_StreetManager.HouseTypeSet({ 1,4 }, Rich::FLAT);
//	m_StreetManager.HouseLevelSet({ 1,4 }, 5);
//
//	//m_StreetManager.HutongDestroy(1);
//	//m_StreetManager.HutongLevelChange(0, 2);
//	m_StreetManager.BarrierSet({ 1,3 }, true);
//	m_StreetManager.HouseOwnerSet({ 1,3 }, 2);
//	m_StreetManager.BarrierSet({ 1,1 }, true);
//	m_StreetManager.BarrierSet({ 1,1 }, false);
//	m_StreetManager.Rent({ 1,1 });
//#pragma endregion
}

void GameApp::InitFX()
{
	m_pFXBasic = nullptr;
	m_pFX2D = nullptr;
	// 初始化Effect
	BuildFX(m_pD3DDevice, L"HLSL/Basic.hlsl", &m_pFXBasic);
	BuildFX(m_pD3DDevice, L"HLSL/color.fx", &m_pFX2D);
	assert(m_pFXBasic);
}

void GameApp::InitLight()
{
	LightManager::Initialize(m_pFXBasic);
	m_pLightManager = LightManager::GetLightManager();
	// 环境光参数设置
	DirectionalLight DirLight;
	DirLight.Ambient = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	DirLight.Diffuse = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	DirLight.Specular = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	DirLight.Direction = XMFLOAT3(0.0f, -1.0f, 0.0f);
	DirLight.State = 1;
	// 环境光1个
	m_pLightManager->SetLight(L"Dir0", DirLight);
}

void GameApp::InitModel()
{
	//m_Sprite.Initialize(m_hMainWnd, m_pD3DImmediateContext, m_pFXBasic, "Sprite3D");
	//m_Sprite.InitModel(L"ice.txt");
	m_Sprite.Initialize(m_pD3DImmediateContext, m_pFXBasic);
	m_Sprite.InitModel("Object/房屋.obj", "Texture/Town_S_House.png", "Material/房屋.mtl");
}

void GameApp::InitDInput()
{
	m_pMKInput->Initialize(MainWnd());
	m_pMKInput = m_pMKInput->GetMKInput();
}

void GameApp::InitText()
{
	D2DText::Initialize(m_hMainWnd, m_pSwapChain);
	m_Font.Reset(L"微软雅黑", 40, D2D1::ColorF::Black, DWRITE_TEXT_ALIGNMENT_LEADING, DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
}

void GameApp::InitRenderState()
{

	D3D11_RASTERIZER_DESC noCullDesc;
	ZeroMemory(&noCullDesc, sizeof(D3D11_RASTERIZER_DESC));
	noCullDesc.FillMode = D3D11_FILL_SOLID;
	noCullDesc.CullMode = D3D11_CULL_NONE;
	noCullDesc.FrontCounterClockwise = false;
	noCullDesc.DepthClipEnable = true;

	HR(m_pD3DDevice->CreateRasterizerState(&noCullDesc, &m_NoCullRS));
	m_pD3DImmediateContext->RSSetState(m_NoCullRS);
}

void GameApp::InitCamera()
{
	XMVECTOR eyePos = XMVectorSet(0.0f, 10.0f, -20.0f, 1.0f);
	XMVECTOR targetPos = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	XMVECTOR upVec = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	m_Camera.Initialize(m_pFXBasic);
	m_Camera.SetView(eyePos, targetPos, upVec);
	m_Camera.SetCameraMode(CameraMode::TPS_ONLY);
}

void GameApp::InitBlend()
{
	D3D11_BLEND_DESC blendDesc;
	ZeroMemory(&blendDesc, sizeof(blendDesc));
	blendDesc.AlphaToCoverageEnable = true;
	blendDesc.IndependentBlendEnable = false;
	blendDesc.RenderTarget[0].BlendEnable = true;
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;//混合公式中的源因子
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;//混合公式中的目标因子
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;//混合公式的操作
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO;//混合公式中源混合百分比因子
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;//混合公式中的目标混合百分比因子
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;//混合公式中的混合百分比的操作
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;//混合公式掩码,一般如此
	m_pD3DDevice->CreateBlendState(&blendDesc, &m_AlphaToCoverageBS);
}

void GameApp::InitUI()
{
	//加载纹理
	std::ifstream file;
	std::string name;
	std::vector<std::string> fileName;
	file.open("Texture/UI/Texture.txt");
	while (!file.eof())
	{
		file >> name;
		fileName.emplace_back("Texture/UI/" + name);
		name.clear();
	}
	file.close();
	for (int i = 0; i < fileName.size(); i++)
	{
		m_pUITexture.emplace_back(nullptr);
		D3DX11CreateShaderResourceViewFromFileA(m_pD3DDevice, fileName[i].c_str(), nullptr, nullptr, &m_pUITexture[i], nullptr);
	}
	//描述2D布局
	m_pTech2D = m_pFX2D->GetTechniqueByName("TexTech");
	// Create the vertex input layout.
	D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD",0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	// Create the input layout
	D3DX11_PASS_DESC passDesc;
	m_pTech2D->GetPassByIndex(0)->GetDesc(&passDesc);
	HR(m_pD3DDevice->CreateInputLayout(vertexDesc, 2, passDesc.pIAInputSignature,
		passDesc.IAInputSignatureSize, &m_p2DInputLayout));
	//初始化UI
	DX11UI::UI::InitAll(m_pD3DDevice, m_pFX2D, m_pSwapChain, m_hMainWnd, m_pD3DImmediateContext, m_p2DInputLayout, m_AlphaToCoverageBS);
	//编辑UI布局
	//主菜单
	m_UI[0].Init(0, 1, 3);
	//背景
	m_UI[0].m_ImageBox[0].SetControl(2.0f, 2.0f, 1);
	m_UI[0].m_ImageBox[0].SetImage(m_pUITexture[0]);
	//创建房间
	m_UI[0].m_Button[0].SetControl(0.403125f, 0.211111f, 2);
	m_UI[0].m_Button[0].SetPicture_ButtonOut(m_pUITexture[1]);
	m_UI[0].m_Button[0].SetPicture_ButtonOn(m_pUITexture[2]);
	m_UI[0].m_Button[0].SetPicture_ButtonDown(m_pUITexture[3]);
	m_UI[0].m_Button[0].SetFont_All(L"", 1.0f);
	m_UI[0].m_Button[0].SetWords_All(L"", D2D1::ColorF::Black);
	m_UI[0].m_Button[0].Move(-0.0171875f, -0.108333f);
	//加入房间
	m_UI[0].m_Button[1].SetControl(0.403125f, 0.211111f, 2);
	m_UI[0].m_Button[1].SetPicture_ButtonOut(m_pUITexture[4]);
	m_UI[0].m_Button[1].SetPicture_ButtonOn(m_pUITexture[5]);
	m_UI[0].m_Button[1].SetPicture_ButtonDown(m_pUITexture[6]);
	m_UI[0].m_Button[1].SetFont_All(L"", 1.0f);
	m_UI[0].m_Button[1].SetWords_All(L"", D2D1::ColorF::Black);
	m_UI[0].m_Button[1].Move(-0.0171875f, -0.394444f);
	//退出游戏
	m_UI[0].m_Button[2].SetControl(0.403125f, 0.211111f, 2);
	m_UI[0].m_Button[2].SetPicture_ButtonOut(m_pUITexture[7]);
	m_UI[0].m_Button[2].SetPicture_ButtonOn(m_pUITexture[8]);
	m_UI[0].m_Button[2].SetPicture_ButtonDown(m_pUITexture[9]);
	m_UI[0].m_Button[2].SetFont_All(L"", 1.0f);
	m_UI[0].m_Button[2].SetWords_All(L"", D2D1::ColorF::Black);
	m_UI[0].m_Button[2].Move(-0.0171875f, -0.683333f);
	//创建房间
	m_UI[1].Init(3, 1, 8);
	//背景
	m_UI[1].m_ImageBox[0].SetControl(2.0f, 2.0f, 1);
	m_UI[1].m_ImageBox[0].SetImage(m_pUITexture[10]);
	//游戏人数-
	m_UI[1].m_Button[0].SetControl(0.0765625f, 0.158333f, 2);
	m_UI[1].m_Button[0].SetPicture_All(m_pUITexture[11]);
	m_UI[1].m_Button[0].SetPicture_ButtonDown(m_pUITexture[12]);
	m_UI[1].m_Button[0].SetFont_All(L"", 1.0f);
	m_UI[1].m_Button[0].SetWords_All(L"", D2D1::ColorF::Black);
	m_UI[1].m_Button[0].Move(0.310156f, 0.479167f);
	//游戏人数+
	m_UI[1].m_Button[1].SetControl(0.0765625f, 0.158333f, 2);
	m_UI[1].m_Button[1].SetPicture_All(m_pUITexture[13]);
	m_UI[1].m_Button[1].SetPicture_ButtonDown(m_pUITexture[14]);
	m_UI[1].m_Button[1].SetFont_All(L"", 1.0f);
	m_UI[1].m_Button[1].SetWords_All(L"", D2D1::ColorF::Black);
	m_UI[1].m_Button[1].Move(0.716406f, 0.479167f);
	//游戏人数
	m_UI[1].m_TextBox[0].SetControl(0.329688f, 0.158333f, 2);
	m_UI[1].m_TextBox[0].SetFont(L"黑体", 36.0f);
	m_UI[1].m_TextBox[0].SetWords(L"2", D2D1::ColorF::Black);
	m_UI[1].m_TextBox[0].Move(0.513281f, 0.479167f);
	//初始金额-
	m_UI[1].m_Button[2].SetControl(0.0765625f, 0.158333f, 2);
	m_UI[1].m_Button[2].SetPicture_All(m_pUITexture[11]);
	m_UI[1].m_Button[2].SetPicture_ButtonDown(m_pUITexture[12]);
	m_UI[1].m_Button[2].SetFont_All(L"", 1.0f);
	m_UI[1].m_Button[2].SetWords_All(L"", D2D1::ColorF::Black);
	m_UI[1].m_Button[2].Move(0.310156f, 0.131944f);
	//初始金额+
	m_UI[1].m_Button[3].SetControl(0.0765625f, 0.158333f, 2);
	m_UI[1].m_Button[3].SetPicture_All(m_pUITexture[13]);
	m_UI[1].m_Button[3].SetPicture_ButtonDown(m_pUITexture[14]);
	m_UI[1].m_Button[3].SetFont_All(L"", 1.0f);
	m_UI[1].m_Button[3].SetWords_All(L"", D2D1::ColorF::Black);
	m_UI[1].m_Button[3].Move(0.716406f, 0.131944f);
	//初始金额
	m_UI[1].m_TextBox[1].SetControl(0.329688f, 0.158333f, 2);
	m_UI[1].m_TextBox[1].SetFont(L"黑体", 36.0f);
	m_UI[1].m_TextBox[1].SetWords(L"100000", D2D1::ColorF::Black);
	m_UI[1].m_TextBox[1].Move(0.513281f, 0.131944f);
	//初始点劵-
	m_UI[1].m_Button[4].SetControl(0.0765625f, 0.158333f, 2);
	m_UI[1].m_Button[4].SetPicture_All(m_pUITexture[11]);
	m_UI[1].m_Button[4].SetPicture_ButtonDown(m_pUITexture[12]);
	m_UI[1].m_Button[4].SetFont_All(L"", 1.0f);
	m_UI[1].m_Button[4].SetWords_All(L"", D2D1::ColorF::Black);
	m_UI[1].m_Button[4].Move(0.310156f, -0.220833f);
	//初始点劵+
	m_UI[1].m_Button[5].SetControl(0.0765625f, 0.158333f, 2);
	m_UI[1].m_Button[5].SetPicture_All(m_pUITexture[13]);
	m_UI[1].m_Button[5].SetPicture_ButtonDown(m_pUITexture[14]);
	m_UI[1].m_Button[5].SetFont_All(L"", 1.0f);
	m_UI[1].m_Button[5].SetWords_All(L"", D2D1::ColorF::Black);
	m_UI[1].m_Button[5].Move(0.716406f, -0.220833f);
	//初始点劵
	m_UI[1].m_TextBox[2].SetControl(0.329688f, 0.158333f, 2);
	m_UI[1].m_TextBox[2].SetFont(L"黑体", 36.0f);
	m_UI[1].m_TextBox[2].SetWords(L"0", D2D1::ColorF::Black);
	m_UI[1].m_TextBox[2].Move(0.513281f, -0.220833f);
	//返回按钮
	m_UI[1].m_Button[6].SetControl(0.375f, 0.211111f, 2);
	m_UI[1].m_Button[6].SetFont_All(L"", 1.0f);
	m_UI[1].m_Button[6].SetWords_All(L"", D2D1::ColorF::Black);
	m_UI[1].m_Button[6].SetPicture_All(m_pUITexture[15]);
	m_UI[1].m_Button[6].SetPicture_ButtonDown(m_pUITexture[16]);
	m_UI[1].m_Button[6].Move(0.2625f, -0.841667f);
	//确认按钮
	m_UI[1].m_Button[7].SetControl(0.375f, 0.211111f, 2);
	m_UI[1].m_Button[7].SetFont_All(L"", 1.0f);
	m_UI[1].m_Button[7].SetWords_All(L"", D2D1::ColorF::Black);
	m_UI[1].m_Button[7].SetPicture_All(m_pUITexture[17]);
	m_UI[1].m_Button[7].SetPicture_ButtonDown(m_pUITexture[18]);
	m_UI[1].m_Button[7].Move(0.732813f, -0.841667f);
	//加入房间
	m_UI[2].Init(0, 1, 1);
	//背景
	m_UI[2].m_ImageBox[0].SetControl(2.0f, 2.0f, 1);
	m_UI[2].m_ImageBox[0].SetImage(m_pUITexture[19]);
	//准备按钮
	m_UI[2].m_Button[0].SetControl(0.5403125f, 0.211111f, 2);
	m_UI[2].m_Button[0].SetFont_All(L"", 1.0f);
	m_UI[2].m_Button[0].SetWords_All(L"", D2D1::ColorF::Black);
	m_UI[2].m_Button[0].SetPicture_ButtonOut(m_pUITexture[20]);
	m_UI[2].m_Button[0].SetPicture_ButtonOn(m_pUITexture[21]);
	m_UI[2].m_Button[0].SetPicture_ButtonDown(m_pUITexture[22]);
	m_UI[2].m_Button[0].Move(-0.0171875f, -0.708333f);
	//游戏内界面
	m_UI[3].Init(4, 7, 7);
	//倒计时
	m_UI[3].m_ImageBox[0].SetControl(0.24375f, 0.155556f, 999);
	m_UI[3].m_ImageBox[0].SetImage(m_pUITexture[23]);
	m_UI[3].m_ImageBox[0].Move(-0.000f, 0.891667f);
	//倒计时时间
	m_UI[3].m_TextBox[3].SetControl(0.24375f, 0.155556f, 2);
	m_UI[3].m_TextBox[3].SetFont(L"黑体", 40.0f);
	m_UI[3].m_TextBox[3].SetWords(L"60", D2D1::ColorF::White);
	m_UI[3].m_TextBox[3].Move(-0.000f, 0.891667f);
	//地图
	m_UI[3].m_ImageBox[1].SetControl(0.346875f, 0.65f, 999);
	m_UI[3].m_ImageBox[1].SetImage(m_pUITexture[24]);
	m_UI[3].m_ImageBox[1].Move(-0.807812f, -0.677778f);
	//头像
	m_UI[3].m_ImageBox[2].SetControl(0.198437f, 0.355556f, 999);
	m_UI[3].m_ImageBox[2].SetImage(m_pUITexture[25]);
	m_UI[3].m_ImageBox[2].Move(-0.382031f, -0.808333f);
	//名字
	m_UI[3].m_ImageBox[3].SetControl(0.229687f, 0.186111f, 999);
	m_UI[3].m_ImageBox[3].SetImage(m_pUITexture[29]);
	m_UI[3].m_ImageBox[3].Move(-0.374219f, -0.479167f);
	//名字文字
	m_UI[3].m_TextBox[0].SetControl(0.229687f, 0.186111f, 1);
	m_UI[3].m_TextBox[0].SetFont(L"黑体", 30.0f);
	m_UI[3].m_TextBox[0].SetWords(L"Black", D2D1::ColorF::Black);
	m_UI[3].m_TextBox[0].Move(-0.344219f, -0.479167f);
	//金币
	m_UI[3].m_ImageBox[4].SetControl(0.425f, 0.177778f, 999);
	m_UI[3].m_ImageBox[4].SetImage(m_pUITexture[30]);
	m_UI[3].m_ImageBox[4].Move(0.00781253f, -0.480556f);
	//金币文字
	m_UI[3].m_TextBox[1].SetControl(0.425f, 0.177778f, 1);
	m_UI[3].m_TextBox[1].SetFont(L"黑体", 30.0f);
	m_UI[3].m_TextBox[1].SetWords(L"100000", D2D1::ColorF::White);
	m_UI[3].m_TextBox[1].Move(0.00781253f, -0.480556f);
	//点券
	m_UI[3].m_ImageBox[5].SetControl(0.475f, 0.177778f, 999);
	m_UI[3].m_ImageBox[5].SetImage(m_pUITexture[31]);
	m_UI[3].m_ImageBox[5].Move(0.457812f, -0.480556f);
	//点券文字
	m_UI[3].m_TextBox[2].SetControl(0.475f, 0.177778f, 1);
	m_UI[3].m_TextBox[2].SetFont(L"黑体", 30.0f);
	m_UI[3].m_TextBox[2].SetWords(L"300", D2D1::ColorF::White);
	m_UI[3].m_TextBox[2].Move(0.457812f, -0.480556f);
	//卡牌按钮1
	m_UI[3].m_Button[0].SetControl(0.140625f, 0.355556f, 999);
	m_UI[3].m_Button[0].SetFont_All(L"", 1.0f);
	m_UI[3].m_Button[0].SetWords_All(L"", D2D1::ColorF::Black);
	m_UI[3].m_Button[0].SetPicture_All(m_pUITexture[83]);
	m_UI[3].m_Button[0].SetPicture_ButtonDown(m_pUITexture[84]);
	m_UI[3].m_Button[0].Move(-0.153125f, -0.808333f);
	//卡牌按钮2
	m_UI[3].m_Button[1].SetControl(0.140625f, 0.355556f, 999);
	m_UI[3].m_Button[1].SetFont_All(L"", 1.0f);
	m_UI[3].m_Button[1].SetWords_All(L"", D2D1::ColorF::Black);
	m_UI[3].m_Button[1].SetPicture_All(m_pUITexture[83]);
	m_UI[3].m_Button[1].SetPicture_ButtonDown(m_pUITexture[84]);
	m_UI[3].m_Button[1].Move(0.015625f, -0.808333f);
	//卡牌按钮3
	m_UI[3].m_Button[2].SetControl(0.140625f, 0.355556f, 999);
	m_UI[3].m_Button[2].SetFont_All(L"", 1.0f);
	m_UI[3].m_Button[2].SetWords_All(L"", D2D1::ColorF::Black);
	m_UI[3].m_Button[2].SetPicture_All(m_pUITexture[83]);
	m_UI[3].m_Button[2].SetPicture_ButtonDown(m_pUITexture[84]);
	m_UI[3].m_Button[2].Move(0.184375f, -0.808333f);
	//卡牌按钮4
	m_UI[3].m_Button[3].SetControl(0.140625f, 0.355556f, 999);
	m_UI[3].m_Button[3].SetFont_All(L"", 1.0f);
	m_UI[3].m_Button[3].SetWords_All(L"", D2D1::ColorF::Black);
	m_UI[3].m_Button[3].SetPicture_All(m_pUITexture[83]);
	m_UI[3].m_Button[3].SetPicture_ButtonDown(m_pUITexture[84]);
	m_UI[3].m_Button[3].Move(0.353125f, -0.808333f);
	//卡牌按钮5
	m_UI[3].m_Button[4].SetControl(0.140625f, 0.355556f, 999);
	m_UI[3].m_Button[4].SetFont_All(L"", 1.0f);
	m_UI[3].m_Button[4].SetWords_All(L"", D2D1::ColorF::Black);
	m_UI[3].m_Button[4].SetPicture_All(m_pUITexture[83]);
	m_UI[3].m_Button[4].SetPicture_ButtonDown(m_pUITexture[84]);
	m_UI[3].m_Button[4].Move(0.521875f, -0.808333f);
	//指针
	m_UI[3].m_ImageBox[6].SetControl(0.03125f, 0.219444f, 1000);
	m_UI[3].m_ImageBox[6].SetImage(m_pUITexture[33]);
	m_UI[3].m_ImageBox[6].Move(0.8375f, -0.6295f);
	//轮盘
	m_UI[3].m_Button[5].SetControl(0.284375f, 0.527778f, 999);
	m_UI[3].m_Button[5].SetFont_All(L"", 1.0f);
	m_UI[3].m_Button[5].SetWords_All(L"", D2D1::ColorF::Black);
	m_UI[3].m_Button[5].SetPicture_All(m_pUITexture[32]);
	m_UI[3].m_Button[5].Move(0.8375f, -0.708333f);
	//退出
	m_UI[3].m_Button[6].SetControl(0.0921875f, 0.161111f, 999);
	m_UI[3].m_Button[6].SetFont_All(L"", 1.0f);
	m_UI[3].m_Button[6].SetWords_All(L"", D2D1::ColorF::Black);
	m_UI[3].m_Button[6].SetPicture_ButtonOut(m_pUITexture[34]);
	m_UI[3].m_Button[6].SetPicture_ButtonOn(m_pUITexture[35]);
	m_UI[3].m_Button[6].SetPicture_ButtonDown(m_pUITexture[36]);
	m_UI[3].m_Button[6].Move(0.933594f, 0.891667f);
	//购买土地
	m_UI[4].Init(0, 1, 2);
	//背景
	m_UI[4].m_ImageBox[0].SetControl(0.746875f, 0.922222f, 999);
	m_UI[4].m_ImageBox[0].SetImage(m_pUITexture[37]);
	m_UI[4].m_ImageBox[0].Move(-0.00156251f, 0.075f);
	//1000Gold
	m_UI[4].m_Button[0].SetControl(0.225f, 0.188889f, 1000);
	m_UI[4].m_Button[0].SetFont_All(L"", 1.0f);
	m_UI[4].m_Button[0].SetWords_All(L"", D2D1::ColorF::Black);
	m_UI[4].m_Button[0].SetPicture_ButtonOut(m_pUITexture[38]);
	m_UI[4].m_Button[0].SetPicture_ButtonOn(m_pUITexture[39]);
	m_UI[4].m_Button[0].SetPicture_ButtonDown(m_pUITexture[40]);
	m_UI[4].m_Button[0].Move(-0.176563f, -0.227778f);
	//取消
	m_UI[4].m_Button[1].SetControl(0.225f, 0.188889f, 1000);
	m_UI[4].m_Button[1].SetFont_All(L"", 1.0f);
	m_UI[4].m_Button[1].SetWords_All(L"", D2D1::ColorF::Black);
	m_UI[4].m_Button[1].SetPicture_ButtonOut(m_pUITexture[41]);
	m_UI[4].m_Button[1].SetPicture_ButtonOn(m_pUITexture[42]);
	m_UI[4].m_Button[1].SetPicture_ButtonDown(m_pUITexture[43]);
	m_UI[4].m_Button[1].Move(0.175f, -0.227778f);
	//升级房屋
	m_UI[5].Init(0, 3, 3);
	//背景
	m_UI[5].m_ImageBox[0].SetControl(1.04375f, 1.00278f, 999);
	m_UI[5].m_ImageBox[0].SetImage(m_pUITexture[44]);
	m_UI[5].m_ImageBox[0].Move(-0.015625f, 0.284722f);
	//别墅图片
	m_UI[5].m_ImageBox[1].SetControl(0.285937f, 0.5f, 1000);
	m_UI[5].m_ImageBox[1].SetImage(m_pUITexture[45]);
	m_UI[5].m_ImageBox[1].Move(-0.242969f, 0.311111f);
	//升级别墅按钮
	m_UI[5].m_Button[0].SetControl(0.225f, 0.144444f, 1000);
	m_UI[5].m_Button[0].SetFont_All(L"", 18.0f);
	m_UI[5].m_Button[0].SetWords_All(L"", D2D1::ColorF::White);
	m_UI[5].m_Button[0].SetPicture_ButtonOut(m_pUITexture[46]);
	m_UI[5].m_Button[0].SetPicture_ButtonOn(m_pUITexture[47]);
	m_UI[5].m_Button[0].SetPicture_ButtonDown(m_pUITexture[48]);
	m_UI[5].m_Button[0].Move(-0.25625f, -0.0888889f);
	//住宅
	m_UI[5].m_ImageBox[2].SetControl(0.378125f, 0.522222f, 1000);
	m_UI[5].m_ImageBox[2].SetImage(m_pUITexture[49]);
	m_UI[5].m_ImageBox[2].Move(0.209375f, 0.305556f);
	//升级住宅按钮
	m_UI[5].m_Button[1].SetControl(0.225f, 0.144444f, 1000);
	m_UI[5].m_Button[1].SetFont_All(L"", 18.0f);
	m_UI[5].m_Button[1].SetWords_All(L"", D2D1::ColorF::White);
	m_UI[5].m_Button[1].SetPicture_ButtonOut(m_pUITexture[46]);
	m_UI[5].m_Button[1].SetPicture_ButtonOn(m_pUITexture[47]);
	m_UI[5].m_Button[1].SetPicture_ButtonDown(m_pUITexture[48]);
	m_UI[5].m_Button[1].Move(0.21875f, -0.0888889f);
	//X
	m_UI[5].m_Button[2].SetControl(0.0296875f, 0.0861111f, 1000);
	m_UI[5].m_Button[2].SetFont_All(L"", 1.0f);
	m_UI[5].m_Button[2].SetWords_All(L"", D2D1::ColorF::Black);
	m_UI[5].m_Button[2].SetPicture_All(m_pUITexture[50]);
	m_UI[5].m_Button[2].SetPicture_ButtonDown(m_pUITexture[51]);
	m_UI[5].m_Button[2].Move(0.436719f, 0.684722f);
	//商店
	m_UI[6].Init(8, 1, 9);
	//背景
	m_UI[6].m_ImageBox[0].SetControl(1.24219f, 1.13611f, 999);
	m_UI[6].m_ImageBox[0].SetImage(m_pUITexture[52]);
	m_UI[6].m_ImageBox[0].Move(-0.0554687f, 0.204167f);
	//X
	m_UI[6].m_Button[0].SetControl(0.0296875f, 0.075f, 1000);
	m_UI[6].m_Button[0].SetFont_All(L"", 1.0f);
	m_UI[6].m_Button[0].SetWords_All(L"", D2D1::ColorF::Black);
	m_UI[6].m_Button[0].SetPicture_All(m_pUITexture[50]);
	m_UI[6].m_Button[0].SetPicture_ButtonDown(m_pUITexture[51]);
	m_UI[6].m_Button[0].Move(0.507031f, 0.704167f);
	//1行1
	m_UI[6].m_Button[1].SetControl(0.140625f, 0.355556f, 1000);
	m_UI[6].m_Button[1].SetFont_All(L"", 1.0f);
	m_UI[6].m_Button[1].SetWords_All(L"", D2D1::ColorF::Black);
	m_UI[6].m_Button[1].SetPicture_All(m_pUITexture[90]);
	m_UI[6].m_Button[1].SetPicture_ButtonDown(m_pUITexture[94]);
	m_UI[6].m_Button[1].Move(-0.257813f, 0.425f);
	//1行1文字
	m_UI[6].m_TextBox[0].SetControl(0.140625f, 0.0333333f, 1000);
	m_UI[6].m_TextBox[0].SetFont(L"黑体", 18.0f);
	m_UI[6].m_TextBox[0].SetWords(L"100", D2D1::ColorF::White);
	m_UI[6].m_TextBox[0].Move(-0.26f, 0.211111f);
	//1行2
	m_UI[6].m_Button[2].SetControl(0.140625f, 0.355556f, 1000);
	m_UI[6].m_Button[2].SetFont_All(L"", 1.0f);
	m_UI[6].m_Button[2].SetWords_All(L"", D2D1::ColorF::Black);
	m_UI[6].m_Button[2].SetPicture_All(m_pUITexture[90]);
	m_UI[6].m_Button[2].SetPicture_ButtonDown(m_pUITexture[94]);
	m_UI[6].m_Button[2].Move(-0.06f, 0.425f);
	//1行2文字
	m_UI[6].m_TextBox[1].SetControl(0.140625f, 0.0333333f, 1000);
	m_UI[6].m_TextBox[1].SetFont(L"黑体", 18.0f);
	m_UI[6].m_TextBox[1].SetWords(L"100", D2D1::ColorF::White);
	m_UI[6].m_TextBox[1].Move(-0.06f, 0.211111f);
	//1行3
	m_UI[6].m_Button[3].SetControl(0.140625f, 0.355556f, 1000);
	m_UI[6].m_Button[3].SetFont_All(L"", 1.0f);
	m_UI[6].m_Button[3].SetWords_All(L"", D2D1::ColorF::Black);
	m_UI[6].m_Button[3].SetPicture_All(m_pUITexture[90]);
	m_UI[6].m_Button[3].SetPicture_ButtonDown(m_pUITexture[94]);
	m_UI[6].m_Button[3].Move(0.16f, 0.425f);
	//1行3文字
	m_UI[6].m_TextBox[2].SetControl(0.140625f, 0.0333333f, 1000);
	m_UI[6].m_TextBox[2].SetFont(L"黑体", 18.0f);
	m_UI[6].m_TextBox[2].SetWords(L"100", D2D1::ColorF::White);
	m_UI[6].m_TextBox[2].Move(0.16f, 0.211111f);
	//1行4
	m_UI[6].m_Button[4].SetControl(0.140625f, 0.355556f, 1000);
	m_UI[6].m_Button[4].SetFont_All(L"", 1.0f);
	m_UI[6].m_Button[4].SetWords_All(L"", D2D1::ColorF::Black);
	m_UI[6].m_Button[4].SetPicture_All(m_pUITexture[90]);
	m_UI[6].m_Button[4].SetPicture_ButtonDown(m_pUITexture[94]);
	m_UI[6].m_Button[4].Move(0.36f, 0.425f);
	//1行4文字
	m_UI[6].m_TextBox[3].SetControl(0.140625f, 0.0333333f, 1000);
	m_UI[6].m_TextBox[3].SetFont(L"黑体", 18.0f);
	m_UI[6].m_TextBox[3].SetWords(L"100", D2D1::ColorF::White);
	m_UI[6].m_TextBox[3].Move(0.36f, 0.211111f);
	//2行1
	m_UI[6].m_Button[5].SetControl(0.140625f, 0.355556f, 1000);
	m_UI[6].m_Button[5].SetFont_All(L"", 1.0f);
	m_UI[6].m_Button[5].SetWords_All(L"", D2D1::ColorF::Black);
	m_UI[6].m_Button[5].SetPicture_All(m_pUITexture[90]);
	m_UI[6].m_Button[5].SetPicture_ButtonDown(m_pUITexture[94]);
	m_UI[6].m_Button[5].Move(-0.26f, -0.0277778f);
	//2行1文字
	m_UI[6].m_TextBox[4].SetControl(0.140625f, 0.0333333f, 1000);
	m_UI[6].m_TextBox[4].SetFont(L"黑体", 18.0f);
	m_UI[6].m_TextBox[4].SetWords(L"100", D2D1::ColorF::White);
	m_UI[6].m_TextBox[4].Move(-0.26f, -0.255f);
	//2行2
	m_UI[6].m_Button[6].SetControl(0.140625f, 0.355556f, 1000);
	m_UI[6].m_Button[6].SetFont_All(L"", 1.0f);
	m_UI[6].m_Button[6].SetWords_All(L"", D2D1::ColorF::Black);
	m_UI[6].m_Button[6].SetPicture_All(m_pUITexture[90]);
	m_UI[6].m_Button[6].SetPicture_ButtonDown(m_pUITexture[94]);
	m_UI[6].m_Button[6].Move(-0.06f, -0.0277778f);
	//2行2文字
	m_UI[6].m_TextBox[5].SetControl(0.140625f, 0.0333333f, 1000);
	m_UI[6].m_TextBox[5].SetFont(L"黑体", 18.0f);
	m_UI[6].m_TextBox[5].SetWords(L"100", D2D1::ColorF::White);
	m_UI[6].m_TextBox[5].Move(-0.06f, -0.255f);
	//2行3
	m_UI[6].m_Button[7].SetControl(0.140625f, 0.355556f, 1000);
	m_UI[6].m_Button[7].SetFont_All(L"", 1.0f);
	m_UI[6].m_Button[7].SetWords_All(L"", D2D1::ColorF::Black);
	m_UI[6].m_Button[7].SetPicture_All(m_pUITexture[90]);
	m_UI[6].m_Button[7].SetPicture_ButtonDown(m_pUITexture[94]);
	m_UI[6].m_Button[7].Move(0.16f, -0.0277778f);
	//2行3文字
	m_UI[6].m_TextBox[6].SetControl(0.140625f, 0.0333333f, 1000);
	m_UI[6].m_TextBox[6].SetFont(L"黑体", 18.0f);
	m_UI[6].m_TextBox[6].SetWords(L"100", D2D1::ColorF::White);
	m_UI[6].m_TextBox[6].Move(0.16f, -0.255f);
	//2行4
	m_UI[6].m_Button[8].SetControl(0.140625f, 0.355556f, 1000);
	m_UI[6].m_Button[8].SetFont_All(L"", 1.0f);
	m_UI[6].m_Button[8].SetWords_All(L"", D2D1::ColorF::Black);
	m_UI[6].m_Button[8].SetPicture_All(m_pUITexture[90]);
	m_UI[6].m_Button[8].SetPicture_ButtonDown(m_pUITexture[94]);
	m_UI[6].m_Button[8].Move(0.36f, -0.0277778f);
	//2行4文字
	m_UI[6].m_TextBox[7].SetControl(0.140625f, 0.0333333f, 1000);
	m_UI[6].m_TextBox[7].SetFont(L"黑体", 18.0f);
	m_UI[6].m_TextBox[7].SetWords(L"100", D2D1::ColorF::White);
	m_UI[6].m_TextBox[7].Move(0.36f, -0.255f);
	//黑市
	m_UI[7].Init(0, 2, 2);
	//背景
	m_UI[7].m_ImageBox[0].SetControl(1.24219f, 1.13611f, 999);
	m_UI[7].m_ImageBox[0].SetImage(m_pUITexture[54]);
	m_UI[7].m_ImageBox[0].Move(-0.0554687f, 0.204167f);
	//X
	m_UI[7].m_Button[0].SetControl(0.0296875f, 0.075f, 1000);
	m_UI[7].m_Button[0].SetFont_All(L"", 1.0f);
	m_UI[7].m_Button[0].SetWords_All(L"", D2D1::ColorF::Black);
	m_UI[7].m_Button[0].SetPicture_All(m_pUITexture[55]);
	m_UI[7].m_Button[0].SetPicture_ButtonDown(m_pUITexture[56]);
	m_UI[7].m_Button[0].Move(0.507031f, 0.704167f);
	//10000Gold
	m_UI[7].m_Button[1].SetControl(0.225f, 0.144444f, 1000);
	m_UI[7].m_Button[1].SetFont_All(L"", 1.0f);
	m_UI[7].m_Button[1].SetWords_All(L"", D2D1::ColorF::Black);
	m_UI[7].m_Button[1].SetPicture_ButtonOut(m_pUITexture[57]);
	m_UI[7].m_Button[1].SetPicture_ButtonOn(m_pUITexture[58]);
	m_UI[7].m_Button[1].SetPicture_ButtonDown(m_pUITexture[59]);
	m_UI[7].m_Button[1].Move(-0.353125f, -0.1f);
	//剩余次数
	m_UI[7].m_ImageBox[1].SetControl(0.175f, 0.0722222f, 1000);
	m_UI[7].m_ImageBox[1].SetImage(m_pUITexture[60]);
	m_UI[7].m_ImageBox[1].Move(-0.1375f, -0.125f);
	//轮盘卡
	m_UI[8].Init(0, 0, 6);
	//1
	m_UI[8].m_Button[0].SetControl(0.142188f, 0.286111f, 999);
	m_UI[8].m_Button[0].SetFont_All(L"", 1.0f);
	m_UI[8].m_Button[0].SetWords_All(L"", D2D1::ColorF::Black);
	m_UI[8].m_Button[0].SetPicture_ButtonOut(m_pUITexture[64]);
	m_UI[8].m_Button[0].SetPicture_ButtonOn(m_pUITexture[65]);
	m_UI[8].m_Button[0].SetPicture_ButtonDown(m_pUITexture[66]);
	m_UI[8].m_Button[0].Move(-0.316406f, -0.120833f);
	//2
	m_UI[8].m_Button[1].SetControl(0.142188f, 0.286111f, 999);
	m_UI[8].m_Button[1].SetFont_All(L"", 1.0f);
	m_UI[8].m_Button[1].SetWords_All(L"", D2D1::ColorF::Black);
	m_UI[8].m_Button[1].SetPicture_ButtonOut(m_pUITexture[67]);
	m_UI[8].m_Button[1].SetPicture_ButtonOn(m_pUITexture[68]);
	m_UI[8].m_Button[1].SetPicture_ButtonDown(m_pUITexture[69]);
	m_UI[8].m_Button[1].Move(-0.185156f, -0.120833f);
	//3
	m_UI[8].m_Button[2].SetControl(0.142188f, 0.286111f, 999);
	m_UI[8].m_Button[2].SetFont_All(L"", 1.0f);
	m_UI[8].m_Button[2].SetWords_All(L"", D2D1::ColorF::Black);
	m_UI[8].m_Button[2].SetPicture_ButtonOut(m_pUITexture[70]);
	m_UI[8].m_Button[2].SetPicture_ButtonOn(m_pUITexture[71]);
	m_UI[8].m_Button[2].SetPicture_ButtonDown(m_pUITexture[72]);
	m_UI[8].m_Button[2].Move(-0.0539062f, -0.120833f);
	//4
	m_UI[8].m_Button[3].SetControl(0.142188f, 0.286111f, 999);
	m_UI[8].m_Button[3].SetFont_All(L"", 1.0f);
	m_UI[8].m_Button[3].SetWords_All(L"", D2D1::ColorF::Black);
	m_UI[8].m_Button[3].SetPicture_ButtonOut(m_pUITexture[73]);
	m_UI[8].m_Button[3].SetPicture_ButtonOn(m_pUITexture[74]);
	m_UI[8].m_Button[3].SetPicture_ButtonDown(m_pUITexture[75]);
	m_UI[8].m_Button[3].Move(0.0773438f, -0.120833f);
	//5
	m_UI[8].m_Button[4].SetControl(0.142188f, 0.286111f, 999);
	m_UI[8].m_Button[4].SetFont_All(L"", 1.0f);
	m_UI[8].m_Button[4].SetWords_All(L"", D2D1::ColorF::Black);
	m_UI[8].m_Button[4].SetPicture_ButtonOut(m_pUITexture[76]);
	m_UI[8].m_Button[4].SetPicture_ButtonOn(m_pUITexture[77]);
	m_UI[8].m_Button[4].SetPicture_ButtonDown(m_pUITexture[78]);
	m_UI[8].m_Button[4].Move(0.204687f, -0.120833f);
	//6
	m_UI[8].m_Button[5].SetControl(0.142188f, 0.286111f, 999);
	m_UI[8].m_Button[5].SetFont_All(L"", 1.0f);
	m_UI[8].m_Button[5].SetWords_All(L"", D2D1::ColorF::Black);
	m_UI[8].m_Button[5].SetPicture_ButtonOut(m_pUITexture[79]);
	m_UI[8].m_Button[5].SetPicture_ButtonOn(m_pUITexture[80]);
	m_UI[8].m_Button[5].SetPicture_ButtonDown(m_pUITexture[81]);
	m_UI[8].m_Button[5].Move(0.338281f, -0.120833f);
	//事件窗口
	m_UI[9].Init(1, 1, 1);
	//背景
	m_UI[9].m_ImageBox[0].SetControl(1.11094f, 1.07778f, 997);
	m_UI[9].m_ImageBox[0].SetImage(m_pUITexture[102]);
	m_UI[9].m_ImageBox[0].Move(0.0148438f, 0.2f);
	//事件描述
	m_UI[9].m_TextBox[0].SetControl(0.5f, 0.6f, 4);
	m_UI[9].m_TextBox[0].SetFont(L"黑体", 30.0f);
	m_UI[9].m_TextBox[0].SetWords(L"当作无事发生过", D2D1::ColorF::White);
	m_UI[9].m_TextBox[0].Move(0.25f, 0.3f);
	//确认按钮
	m_UI[9].m_Button[0].SetControl(0.225f, 0.188889f, 998);
	m_UI[9].m_Button[0].SetFont_All(L"", 1.0f);
	m_UI[9].m_Button[0].SetWords_All(L"", D2D1::ColorF::Black);
	m_UI[9].m_Button[0].SetPicture_ButtonOut(m_pUITexture[82]);
	m_UI[9].m_Button[0].SetPicture_ButtonOn(m_pUITexture[83]);
	m_UI[9].m_Button[0].SetPicture_ButtonDown(m_pUITexture[84]);
	m_UI[9].m_Button[0].Move(0.417188f, -0.175f);
	//选择玩家界面
	m_UI[10].Init(0, 0, 3);
	//第1个其它玩家
	m_UI[10].m_Button[0].SetControl(0.290625f, 0.2f, 999);
	m_UI[10].m_Button[0].SetFont_All(L"", 1.0f);
	m_UI[10].m_Button[0].SetWords_All(L"", D2D1::ColorF::Black);
	m_UI[10].m_Button[0].SetPicture_ButtonOut(m_pUITexture[85]);
	m_UI[10].m_Button[0].SetPicture_ButtonOn(m_pUITexture[86]);
	m_UI[10].m_Button[0].SetPicture_ButtonDown(m_pUITexture[87]);
	m_UI[10].m_Button[0].Move(-0.348437f, -0.1f);
	//第2个其它玩家
	m_UI[10].m_Button[1].SetControl(0.290625f, 0.2f, 999);
	m_UI[10].m_Button[1].SetFont_All(L"", 1.0f);
	m_UI[10].m_Button[1].SetWords_All(L"", D2D1::ColorF::Black);
	m_UI[10].m_Button[1].SetPicture_ButtonOut(m_pUITexture[85]);
	m_UI[10].m_Button[1].SetPicture_ButtonOn(m_pUITexture[86]);
	m_UI[10].m_Button[1].SetPicture_ButtonDown(m_pUITexture[87]);
	m_UI[10].m_Button[1].Move(0.048437f, -0.1f);
	//第3个其它玩家
	m_UI[10].m_Button[2].SetControl(0.290625f, 0.2f, 999);
	m_UI[10].m_Button[2].SetFont_All(L"", 1.0f);
	m_UI[10].m_Button[2].SetWords_All(L"", D2D1::ColorF::Black);
	m_UI[10].m_Button[2].SetPicture_ButtonOut(m_pUITexture[85]);
	m_UI[10].m_Button[2].SetPicture_ButtonOn(m_pUITexture[86]);
	m_UI[10].m_Button[2].SetPicture_ButtonDown(m_pUITexture[87]);
	m_UI[10].m_Button[2].Move(0.445312f, -0.1f);
}

void GameApp::InitUIWordsType()
{
	std::ifstream file;
	file.open("Texture/UI/UIWords.txt");
	std::string uiWords;
	while (!file.eof())
	{
		file >> uiWords;
		m_UIWords.push_back(CharToWstring(uiWords.c_str(), uiWords.size()));
	}
	file.close();
	m_UIDrawsBegin = { 0, 3,6 };
	m_UIDraws = { 0, 3,6 };
	m_UIDrawsBack = { 2, 5, 8 };
}


void GameApp::OnResize()
{
	D2DText::Discard();
	for (DX11UI::UI &ui : m_UI)
	{
		ui.BeforeReSize();
	}
	D3DApp::OnResize();
	D2DText::OnReset();
	for (DX11UI::UI &ui : m_UI)
	{
		ui.AfterResize(m_hMainWnd);
	}
	// 由于窗口大小重新调整，更新宽高比和重新计算出投影矩阵
	m_Camera.SetPerspective(0.25f * MathHelper::Pi, AspectRatio(), 1.0f, 1000.0f);
	Sprite3D::UpdateFrustum(m_Camera.GetProj());
}

void GameApp::UpdateScene(float dt)
{
	m_pMKInput->Update();
	UpdateCamera(dt);
	Sprite3D::UpdateViewMatrix(m_Camera.GetView());


	if (m_pMKInput->IsMouseDown(MKMouse::LEFT))
	{
		m_UIManager.back()->MouseLeftDown();
	}
	else if (m_pMKInput->IsMouseUp(MKMouse::LEFT))
	{
		m_UIManager.back()->MouseLeftUp();
		ButtonClick();
	}
	UpdateUI(dt);
}

void GameApp::UpdateCamera(float dt)
{
	if (m_pMKInput->Key_Down(DIK_W))
		m_Camera.MoveForward(8.0f * dt);
	if (m_pMKInput->Key_Down(DIK_S))
		m_Camera.MoveForward(-8.0f * dt);
	if (m_pMKInput->Key_Down(DIK_A))
		m_Camera.MoveRight(-8.0f * dt);
	if (m_pMKInput->Key_Down(DIK_D))
		m_Camera.MoveRight(8.0f * dt);

	m_Camera.LookRight(m_pMKInput->GetMouseLXY().x * 0.006f);
	m_Camera.LookUp(m_pMKInput->GetMouseLXY().y * 0.006f);
	if (m_GameStatus == GameStatus::MAIN_MENU || m_GameStatus == GameStatus::JOIN_ROOM || m_GameStatus == GameStatus::CREATE_ROOM)
		m_Camera.SetTarget(XMFLOAT3(0.0f, 0.0f, 0.0f));
	else
		m_Camera.SetTarget(m_PlayerManager.GetPlayerRealPosition());
	if (m_pMKInput->Mouse_LongClick(MKMouse::RIGHT))
		m_Camera.LookAroundRight(m_pMKInput->GetMouseLXY().x * 0.006f);
	if (m_pMKInput->Mouse_LongClick(MKMouse::RIGHT))
		m_Camera.LookAroundUp(m_pMKInput->GetMouseLXY().y * 0.006f);
	m_Camera.Zoom(m_pMKInput->GetMouseLRoll() * 10.0f, 40.0f, 120.0f);

	m_Camera.UpdateViewMatrix();

}

void GameApp::UpdateUI(float dt)
{
	switch (m_GameStatus)
	{
	case MAIN_MENU:
	{
		break;
	}
	case CREATE_ROOM:
	{
		break;
	}
	case JOIN_ROOM:
	{
		if (m_pMessageManager->GetState() == 1)
		{
			m_UIManager.clear();
			m_UIManager.push_back(&m_UI[GAME_USUAL]);
			m_UIManager.back()->m_TextBox[0].SetWords(CharToWstring(m_PlayerManager.GetMyPlayerName().c_str(), m_PlayerManager.GetMyPlayerName().size()).c_str(), D2D1::ColorF::White);
			m_UIManager.back()->m_TextBox[1].SetWords(std::to_wstring(m_PlayerManager.GetMyPlayerGold()).c_str(), D2D1::ColorF::White);
			m_UIManager.back()->m_TextBox[2].SetWords(std::to_wstring(m_PlayerManager.GetMyPlayerSilver()).c_str(), D2D1::ColorF::White);
			m_UI[GAME_USUAL].m_ImageBox[2].SetImage(m_pUITexture[m_PlayerManager.GetMyPlayerID() + 24]);
			for (int i = 0; i < 5; i++)
			{
				m_UIManager.back()->m_Button[i].Disable();
				m_UIManager.back()->m_Button[i].Invisible();
			}
			m_GameStatus = GAME_USUAL;
		}
		break;
	}
	default://游戏内容
	{
		int myID = m_PlayerManager.GetMyPlayerID();
		int curID = m_PlayerManager.GetCurrPlayerID();
		if (myID == m_PlayerManager.GetCurrPlayerID() && m_LastPlayerID != myID)
		{
			m_RoundTimeLess = 60000;
			m_IsRouletteRotate = true;
			int buttonSize = m_UI[GAME_USUAL].Button_Size();
			for (int i = 0; i < buttonSize; i++)
			{
				m_UI[GAME_USUAL].m_Button[i].Enable();
			}
			int cardSize = m_PlayerManager.GetMyPlayerCardType().size();
			for (int i = cardSize; i < 5; i++)
			{
				m_UI[GAME_USUAL].m_Button[i].Disable();
			}
		}
		else if (m_LastPlayerID == myID&&m_PlayerManager.GetCurrPlayerID() != myID)
		{
			m_RoundTimeLess = 0;
			m_IsRouletteRotate = false;
			int buttonSize = m_UI[GAME_USUAL].Button_Size();
			for (int i = 0; i < buttonSize; i++)
			{
				m_UI[GAME_USUAL].m_Button[i].Disable();
			}
		}
		m_LastPlayerID = m_PlayerManager.GetCurrPlayerID();
		//更新街道
		m_StreetManager.UpdateScene();
		//  更新人物管理类
		m_PlayerManager.Update(dt);
		//  更新人物下一步行动（需要跟街道管理类打交道）
		UpddatePlayerNextStep();

		//更新UI
		if (myID == m_LastPlayerID)
		{
			m_RoundTimeLess -= (int)(dt * 1000);
		}
		if (m_RoundTimeLess < 0)//回合时间到
		{
			m_RoundTimeLess = 0;
			m_UIManager.clear();
			int buttonSize = m_UI[GAME_USUAL].Button_Size();
			for (int i = 0; i < buttonSize; i++)
			{
				m_UI[GAME_USUAL].m_Button[i].Enable();
			}
			int cardSize = m_PlayerManager.GetMyPlayerCardType().size();
			for (int i = cardSize; i < 5; i++)
			{
				m_UI[GAME_USUAL].m_Button[i].Disable();
			}
			m_UIManager.push_back(&m_UI[GAME_USUAL]);
			m_GameStatus = GAME_USUAL;
			//强制结束回合
			{
				int buffer[16] = { 5 };
				m_PlayerManager.CalculateCurrPlayerInterests();
				m_pMessageManager->Send(buffer);
			}
		}
		switch (m_PlayerManager.GetCurrPlayerID())
		{
		case 1:
		{
			m_UI[GAME_USUAL].m_TextBox[3].SetWords(std::to_wstring(m_RoundTimeLess / 1000).c_str(), D2D1::ColorF::Red);//刷新回合剩余时间
			break;
		}
		case 2:
		{
			m_UI[GAME_USUAL].m_TextBox[3].SetWords(std::to_wstring(m_RoundTimeLess / 1000).c_str(), D2D1::ColorF::Yellow);//刷新回合剩余时间
			break;
		}
		case 3:
		{
			m_UI[GAME_USUAL].m_TextBox[3].SetWords(std::to_wstring(m_RoundTimeLess / 1000).c_str(), D2D1::ColorF::Blue);//刷新回合剩余时间
			break;
		}
		case 4:
		{
			m_UI[GAME_USUAL].m_TextBox[3].SetWords(std::to_wstring(m_RoundTimeLess / 1000).c_str(), D2D1::ColorF::Green);//刷新回合剩余时间
			break;
		}
		default:
			break;
		}
		
		if (m_IsRouletteRotate)
		{
			m_Roulette += 5;
			if (m_Roulette >= 360)
			{
				m_Roulette = 0;
			}
			m_UI[GAME_USUAL].m_Button[5].RotateElliptical(m_Roulette *MathHelper::Pi / 180);//轮盘旋转
		}
		if (m_pMessageManager->IsRecived())
		{
			m_UI[GAME_USUAL].m_TextBox[0].SetWords(CharToWstring(m_PlayerManager.GetMyPlayerName().c_str(), m_PlayerManager.GetMyPlayerName().size()).c_str(), D2D1::ColorF::White);//名字
			m_UI[GAME_USUAL].m_TextBox[1].SetWords(std::to_wstring(m_PlayerManager.GetMyPlayerGold()).c_str(), D2D1::ColorF::White);//金币
			m_UI[GAME_USUAL].m_TextBox[2].SetWords(std::to_wstring(m_PlayerManager.GetMyPlayerSilver()).c_str(), D2D1::ColorF::White);//点劵
			int i = 0;
			std::vector<Rich::CardType> cardType = m_PlayerManager.GetMyPlayerCardType();
			for (i; i < cardType.size(); i++)//显示有牌的按钮
			{
				m_UI[GAME_USUAL].m_Button[i].Enable();
				m_UI[GAME_USUAL].m_Button[i].Visible();
				m_UI[GAME_USUAL].m_Button[i].SetPicture_All(m_pUITexture[cardType[i] * 2 + 83]);//第85张图是第一张卡,卡牌枚举里面0是空卡
				m_UI[GAME_USUAL].m_Button[i].SetPicture_ButtonDown(m_pUITexture[cardType[i] * 2 + 84]);
			}
			for (i; i < 5; i++)//隐藏没牌的按钮
			{
				m_UI[GAME_USUAL].m_Button[i].Disable();
				m_UI[GAME_USUAL].m_Button[i].Invisible();
			}
		}
		myID = m_PlayerManager.GetMyPlayerID();//自己的ID
		if (m_PlayerManager.IsNeedDealWithEventAfterMove()&&m_PlayerManager.GetCurrPlayerID()==myID)//人物移动完之后的事件
		{
			Rich::StreetManager::BLOCK block = m_StreetManager.BlockGet(m_PlayerManager.GetBlockIndex());
			switch (block.FuncType)
			{
			case Rich::FUNCTYPE::HOUSE://房地
			{
				switch (block.HouseType)//更新升级房屋UI的信息
				{
				case Rich::HOUSETYPE::NOHOUSE:
				{
					if (block.owner == myID)
					{
						int gold = m_PlayerManager.GetMyPlayerGold();
						if (gold <= 1500)
						{
							m_UI[UPGRADE_LAND].m_Button[0].Invisible();
							m_UI[UPGRADE_LAND].m_Button[0].Disable();
							m_UI[UPGRADE_LAND].m_Button[1].Invisible();
							m_UI[UPGRADE_LAND].m_Button[1].Disable();
						}
						else if (gold <= 2000)
						{
							m_UI[UPGRADE_LAND].m_Button[0].Invisible();
							m_UI[UPGRADE_LAND].m_Button[0].Disable();
							m_UI[UPGRADE_LAND].m_Button[1].Visible();
							m_UI[UPGRADE_LAND].m_Button[1].Enable();
						}
						else
						{
							m_UI[UPGRADE_LAND].m_Button[0].Visible();
							m_UI[UPGRADE_LAND].m_Button[0].Enable();
							m_UI[UPGRADE_LAND].m_Button[1].Visible();
							m_UI[UPGRADE_LAND].m_Button[1].Enable();
						}
						m_UI[UPGRADE_LAND].m_ImageBox[1].Visible();
						m_UI[UPGRADE_LAND].m_ImageBox[2].Visible();
						m_UI[UPGRADE_LAND].m_Button[0].SetPicture_ButtonOut(m_pUITexture[46]);
						m_UI[UPGRADE_LAND].m_Button[0].SetPicture_ButtonOn(m_pUITexture[47]);
						m_UI[UPGRADE_LAND].m_Button[0].SetPicture_ButtonDown(m_pUITexture[48]);
						m_UI[UPGRADE_LAND].m_Button[0].SetWords_All(L"2000", D2D1::ColorF::White);
						m_UI[UPGRADE_LAND].m_Button[1].SetPicture_ButtonOut(m_pUITexture[46]);
						m_UI[UPGRADE_LAND].m_Button[1].SetPicture_ButtonOn(m_pUITexture[47]);
						m_UI[UPGRADE_LAND].m_Button[1].SetPicture_ButtonDown(m_pUITexture[48]);
						m_UI[UPGRADE_LAND].m_Button[1].SetWords_All(L"1500", D2D1::ColorF::White);
						m_NumButtonReturn[0] = 2000;
						m_NumButtonReturn[1] = 1500;
						int buttonSize = m_UIManager.back()->Button_Size();
						for (int i = 0; i < buttonSize; i++)//锁定这一层UI的button
						{
							m_UIManager.back()->m_Button[i].Disable();
						}
						m_UIManager.push_back(&m_UI[UPGRADE_LAND]);
						m_GameStatus = UPGRADE_LAND;
					}
					else if (block.owner == 0)
					{
						int buttonSize = m_UI[GAME_USUAL].Button_Size();
						for (int i = 0; i < buttonSize; i++)
						{
							m_UI[GAME_USUAL].m_Button[i].Disable();
						}
						m_GameStatus = BUY_LAND;
						m_UIManager.push_back(&m_UI[BUY_LAND]);
					}
					else
					{
						//空地缴费
						{
							POINT index = m_PlayerManager.GetBlockIndex();
							int price = m_StreetManager.Rent(index);
							{
								int buffer[16] = { 3, 3, m_PlayerManager.GetCurrPlayerID(), -price };
								m_pMessageManager->Send(buffer);
							}
							{
								int owner = m_StreetManager.HouseOwnerGet(index);
								int buffer[16] = { 3, 3, owner, price };
								m_pMessageManager->Send(buffer);
							}
						}
						// 强制结束回合
						{
							int buffer[16] = { 5 };
							m_PlayerManager.CalculateCurrPlayerInterests();
							m_pMessageManager->Send(buffer);
						}
					}
					break;
				}
				case Rich::HOUSETYPE::FLAT:
				{
					if (block.owner == myID)
					{
						if (block.HouseLevel < 5)
						{
							m_UI[UPGRADE_LAND].m_Button[0].Invisible();
							m_UI[UPGRADE_LAND].m_Button[0].Disable();
							m_UI[UPGRADE_LAND].m_Button[1].Visible();
							m_UI[UPGRADE_LAND].m_Button[1].Enable();
							m_UI[UPGRADE_LAND].m_ImageBox[1].Invisible();
							m_UI[UPGRADE_LAND].m_ImageBox[2].Visible();
							m_UI[UPGRADE_LAND].m_Button[1].SetPicture_ButtonOut(m_pUITexture[46]);
							m_UI[UPGRADE_LAND].m_Button[1].SetPicture_ButtonOn(m_pUITexture[47]);
							m_UI[UPGRADE_LAND].m_Button[1].SetPicture_ButtonDown(m_pUITexture[48]);
							m_NumButtonReturn[1] = block.HouseLevel * 500 + 1000;
							m_UI[UPGRADE_LAND].m_Button[1].SetWords_All(std::to_wstring(m_NumButtonReturn[1]).c_str(), D2D1::ColorF::White);
							int buttonSize = m_UIManager.back()->Button_Size();
							for (int i = 0; i < buttonSize; i++)//锁定这一层UI的button
							{
								m_UIManager.back()->m_Button[i].Disable();
							}
							m_UIManager.push_back(&m_UI[UPGRADE_LAND]);
							m_GameStatus = UPGRADE_LAND;
						}					
					}
					else
					{
						//公寓缴费
						{
							POINT index = m_PlayerManager.GetBlockIndex();
							int price = m_StreetManager.Rent(index);
							{
								int buffer[16] = { 3, 3, m_PlayerManager.GetCurrPlayerID(), -price };
								m_pMessageManager->Send(buffer);
							}
							// 房主拿到租金
							{
								int buffer[16] = { 3, 3, block.owner, price };
								m_pMessageManager->Send(buffer);
							}
							
						}
						// 强制结束回合
						{
							int buffer[16] = { 5 };
							m_PlayerManager.CalculateCurrPlayerInterests();
							m_pMessageManager->Send(buffer);
						}
					}
					break;
				}
				case Rich::HOUSETYPE::COTTAGE:
				{
					if (block.owner == myID)
					{
						if (block.HouseLevel < 3)
						{
							m_UI[UPGRADE_LAND].m_Button[1].Invisible();
							m_UI[UPGRADE_LAND].m_Button[1].Disable();
							m_UI[UPGRADE_LAND].m_Button[0].Visible();
							m_UI[UPGRADE_LAND].m_Button[0].Enable();
							m_UI[UPGRADE_LAND].m_ImageBox[2].Invisible();
							m_UI[UPGRADE_LAND].m_ImageBox[1].Visible();
							m_UI[UPGRADE_LAND].m_Button[0].SetPicture_ButtonOut(m_pUITexture[46]);
							m_UI[UPGRADE_LAND].m_Button[0].SetPicture_ButtonOn(m_pUITexture[47]);
							m_UI[UPGRADE_LAND].m_Button[0].SetPicture_ButtonDown(m_pUITexture[48]);
							m_NumButtonReturn[0] = block.HouseLevel * 1500 + 500;
							m_UI[UPGRADE_LAND].m_Button[0].SetWords_All(std::to_wstring(m_NumButtonReturn[0]).c_str(), D2D1::ColorF::White);
							int buttonSize = m_UIManager.back()->Button_Size();
							for (int i = 0; i < buttonSize; i++)//锁定这一层UI的button
							{
								m_UIManager.back()->m_Button[i].Disable();
							}
							m_UIManager.push_back(&m_UI[UPGRADE_LAND]);
							m_GameStatus = UPGRADE_LAND;
						}			
					}
					else
					{
						//别墅缴费
						{
							POINT index = m_PlayerManager.GetBlockIndex();
							int price = m_StreetManager.Rent(index);
							int buffer[16] = { 3, 3, m_PlayerManager.GetCurrPlayerID(), price };
							m_pMessageManager->Send(buffer);
						}
						// 强制结束回合
						{
							int buffer[16] = { 5 };
							m_PlayerManager.CalculateCurrPlayerInterests();
							m_pMessageManager->Send(buffer);
						}
					}
					break;
				}
				default:
					break;
				}
				break;
			}
			case Rich::FUNCTYPE::SILVER_20:
			{
				//该玩家获得20点劵
				{
					int buffer[16] = { 3, 8, 20 };
					m_pMessageManager->Send(buffer);
				}
				//切换玩家
				{
					int buffer[16] = { 5 };
					m_PlayerManager.CalculateCurrPlayerInterests();
					m_pMessageManager->Send(buffer);
				}
				break;
			}
			case Rich::FUNCTYPE::SILVER_30:
			{
				//该玩家获得30点劵
				{
					int buffer[16] = { 3, 8, 30 };
					m_pMessageManager->Send(buffer);
				}
				//切换玩家
				{
					int buffer[16] = { 5 };
					m_PlayerManager.CalculateCurrPlayerInterests();
					m_pMessageManager->Send(buffer);
				}
				break;
			}
			case Rich::FUNCTYPE::SILVER_50:
			{
				//该玩家获得50点劵
				{
					int buffer[16] = { 3, 8, 50 };
					m_pMessageManager->Send(buffer);
				}
				//切换玩家
				{
					int buffer[16] = { 5 };
					m_PlayerManager.CalculateCurrPlayerInterests();
					m_pMessageManager->Send(buffer);
				}
				break;
			}
			case Rich::FUNCTYPE::STEAM:
			{
				int buttonSize = m_UIManager.back()->Button_Size();
				for (int i = 0; i < buttonSize; i++)//锁定这一层UI的button
				{
					m_UIManager.back()->m_Button[i].Disable();
				}
				m_UI[STEAM].m_ImageBox[1].SetImage(m_pUITexture[63]);//剩余三次
				m_SteamLess = 63;
				if (m_PlayerManager.GetMyPlayerGold() > 10000)//玩家金钱小于10000锁定按钮
				{
					m_UI[STEAM].m_Button[1].Enable();
					m_UI[STEAM].m_Button[1].Visible();
				}
				else
				{
					m_UI[STEAM].m_Button[1].Disable();
					m_UI[STEAM].m_Button[1].Invisible();
				}
				m_GameStatus = STEAM;
				m_UIManager.push_back(&m_UI[STEAM]);
				break;
			}
			case Rich::FUNCTYPE::MARKET:
			{
				Rich::CARD card;
				int cardType;
				int silver = m_PlayerManager.GetMyPlayerSilver();
				for (int i = 0; i < 8; i++)//随机生成8张卡
				{
					cardType = rand() % 24 + 1;
					card = m_pCards->GetCardByEnum((Rich::CardType)(cardType));
					m_UI[STORE].m_TextBox[i].SetWords(std::to_wstring(card.silver).c_str(), D2D1::ColorF::White);
					m_UI[STORE].m_TextBox[i].Visible();
					m_UI[STORE].m_Button[i + 1].SetPicture_All(m_pUITexture[cardType * 2 + 83]);//第85张图是第一张卡,卡牌枚举里面0是空卡
					m_UI[STORE].m_Button[i + 1].SetPicture_ButtonDown(m_pUITexture[cardType * 2 + 84]);
					m_UI[STORE].m_Button[i + 1].Visible();
					m_NumButtonReturn[i] = cardType;
					if (silver < card.silver)//点劵不足情况,button是1~8，textbox是0~7
					{
						m_UI[STORE].m_Button[i + 1].Disable();
					}
					else
					{
						m_UI[STORE].m_Button[i + 1].Enable();
					}
				}
				int buttonSize = m_UI[GAME_USUAL].Button_Size();
				for (int i = 0; i < buttonSize; i++)//锁定上层按钮
				{
					m_UI[GAME_USUAL].m_Button[i].Disable();
				}
				m_UIManager.push_back(&m_UI[STORE]);
				m_GameStatus = STORE;
				break;
			}
			case Rich::FUNCTYPE::BANK:
			{
				//拿走自己存款
				int interest = m_PlayerManager.GetPlayerInterests();
				
				{
					int buffer[16] = { 3, 11, m_PlayerManager.GetCurrPlayerID(), 0 };
					m_pMessageManager->Send(buffer);
				}
				{
					int buffer[16] = { 3, 3, m_PlayerManager.GetCurrPlayerID(), interest };
					m_pMessageManager->Send(buffer);
				}
				//切换玩家
				{
					int buffer[16] = { 5 };
					m_PlayerManager.CalculateCurrPlayerInterests();
					m_pMessageManager->Send(buffer);
				}
				break;
			}
			case Rich::FUNCTYPE::DESTINY:
			{
				int destiny = rand() % 15;
				EventEffect(destiny);
				//切换玩家
				{
					int buffer[16] = { 5 };
					m_PlayerManager.CalculateCurrPlayerInterests();
					m_pMessageManager->Send(buffer);
				}
				m_GameStatus = EVENTS;
				m_UI[EVENTS].m_ImageBox[0].SetImage(m_pUITexture[141 + destiny]);
				m_UI[EVENTS].m_TextBox[0].SetWords(m_UIWords[destiny + 9].c_str(), D2D1::ColorF::White);
				int buttonSize = m_UI[GAME_USUAL].Button_Size();
				for (int i = 0; i < buttonSize; i++)
				{
					m_UI[GAME_USUAL].m_Button[i].Disable();
				}
				m_UIManager.push_back(&m_UI[EVENTS]);
				break;
			}
			case Rich::FUNCTYPE::FUNCTYPE_CARD:
			{
				//自己获得两张牌（最多5张）
				{
					int buffer[16] = { 3, 5, m_PlayerManager.GetCurrPlayerID(), rand() % 28 + 1 };
					m_pMessageManager->Send(buffer);
					buffer[3] = rand() % 28 + 1;
					m_pMessageManager->Send(buffer);
				}
				//切换玩家
				{
					int buffer[16] = { 5 };
					m_PlayerManager.CalculateCurrPlayerInterests();
					m_pMessageManager->Send(buffer);
				}
				break;
			}
			default:
				break;
			}
		}
		break;
	}
	}
	for (DX11UI::UI *&ui : m_UIManager)
	{
		ui->Updata();
	}
}

void GameApp::DrawScene()
{
	assert(m_pD3DImmediateContext);
	assert(m_pSwapChain);
	// **************************************
	// ** TODO：设置底色                  ***
	// **************************************
	m_pD3DImmediateContext->ClearRenderTargetView(m_pRenderTargetView, reinterpret_cast<const float*>(&Colors::LightSteelBlue));		//底色
	m_pD3DImmediateContext->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	// ******************************************
	// *** TODO：进行绘制                     ***
	// ******************************************
	switch(m_GameStatus)
	{
	case MAIN_MENU:
	{
		break;
	}
	case CREATE_ROOM:
	{
		break;
	}
	case JOIN_ROOM:
	{
		break;
	}
	default:
	{
		//  绘制人物
		m_PlayerManager.Draw();

		XMFLOAT4X4 ViewProj;
		XMStoreFloat4x4(&ViewProj, m_Camera.GetXMViewProj());
		m_Scene.draw(ViewProj, m_Camera.GetXMFloat3Pos());
		m_StreetManager.Draw();
		break;
	}
	}
	//m_Sprite.DrawInstances();
	// 绘制完3D后就要来到2D部分了
	Draw2D();
	HR(m_pSwapChain->Present(0, 0));
}

void GameApp::Draw2D()
{
	for (DX11UI::UI *&ui : m_UIManager)
	{
		ui->Draw();
	}

}

void GameApp::ButtonClick()				//放在鼠标UI鼠标放开的检测之后
{
	switch (m_GameStatus)
	{
	case MAIN_MENU:
	{
		switch (m_UIManager.back()->ButtonCheck())
		{
		case 0:
		{
			m_UIManager.back()->ResetButtonCheck();
			m_UIManager.clear();
			m_UIManager.push_back(&m_UI[CREATE_ROOM]);
			m_GameStatus = CREATE_ROOM;
			break;
		}
		case 1://加入房间
		{
			m_UIManager.back()->ResetButtonCheck();
			m_UIManager.clear();
			m_UIManager.push_back(&m_UI[JOIN_ROOM]);
			m_GameStatus = JOIN_ROOM;
			break;
		}
		case 2://退出游戏
		{
			m_UIManager.back()->ResetButtonCheck();
			PostQuitMessage(0);
			break;	
		}
		default:
			break;
		}
		break;
	}
	case CREATE_ROOM:
	{
		switch (m_UIManager.back()->ButtonCheck())
		{
		case 0:
		{
			m_UIManager.back()->ResetButtonCheck();
			if (m_UIDraws[0] > m_UIDrawsBegin[0])
			{
				m_UIDraws[0]--;
				m_UI[CREATE_ROOM].m_TextBox[0].SetWords(m_UIWords[m_UIDraws[0]].c_str(), D2D1::ColorF::White);
			}
			break;
		}
		case 1:
		{
			m_UIManager.back()->ResetButtonCheck();
			if (m_UIDraws[0] < m_UIDrawsBack[0])
			{
				m_UIDraws[0]++;
				m_UI[CREATE_ROOM].m_TextBox[0].SetWords(m_UIWords[m_UIDraws[0]].c_str(), D2D1::ColorF::White);
			}
			break;
		}
		case 2:
		{
			m_UIManager.back()->ResetButtonCheck();
			if (m_UIDraws[1] > m_UIDrawsBegin[1])
			{
				m_UIDraws[1]--;
				m_UI[CREATE_ROOM].m_TextBox[1].SetWords(m_UIWords[m_UIDraws[1]].c_str(), D2D1::ColorF::White);
			}
			break;
		}
		case 3:
		{
			m_UIManager.back()->ResetButtonCheck();
			if (m_UIDraws[1] < m_UIDrawsBack[1])
			{
				m_UIDraws[1]++;
				m_UI[CREATE_ROOM].m_TextBox[1].SetWords(m_UIWords[m_UIDraws[1]].c_str(), D2D1::ColorF::White);
			}
			break;
		}
		case 4:
		{
			m_UIManager.back()->ResetButtonCheck();
			if (m_UIDraws[2] > m_UIDrawsBegin[2])
			{
				m_UIDraws[2]--;
				m_UI[CREATE_ROOM].m_TextBox[2].SetWords(m_UIWords[m_UIDraws[2]].c_str(), D2D1::ColorF::White);
			}
			break;
		}
		case 5:
		{
			m_UIManager.back()->ResetButtonCheck();
			if (m_UIDraws[2] < m_UIDrawsBack[2])
			{
				m_UIDraws[2]++;
				m_UI[CREATE_ROOM].m_TextBox[2].SetWords(m_UIWords[m_UIDraws[2]].c_str(), D2D1::ColorF::White);
			}
			break;
		}
		case 6://返回
		{
			m_UIManager.back()->ResetButtonCheck();
			m_UIManager.clear();
			m_UIManager.push_back(&m_UI[MAIN_MENU]);
			m_UI[CREATE_ROOM].m_TextBox[0].SetWords(m_UIWords[m_UIDrawsBegin[0]].c_str(), D2D1::ColorF::White);
			m_UI[CREATE_ROOM].m_TextBox[1].SetWords(m_UIWords[m_UIDrawsBegin[1]].c_str(), D2D1::ColorF::White);
			m_UI[CREATE_ROOM].m_TextBox[2].SetWords(m_UIWords[m_UIDrawsBegin[2]].c_str(), D2D1::ColorF::White);
			m_GameStatus = MAIN_MENU;
			break;
		}
		case 7://确认，并向服务器发送消息
		{
			m_UIManager.back()->ResetButtonCheck();
			m_UIManager.clear();
			m_UIManager.push_back(&m_UI[JOIN_ROOM]);
			m_pMessageManager->HostConnect(
				m_UIDraws[0] + 2,																												//人数索引（0~2）对应 2~4人
				(m_UIDraws[1] - 2) * 100000,																							//金币索引（3~5）对应100000~3000000金币
				(m_UIDraws[2] - 6) * 50																										//点券索引（6~8）对应0~100点券
			);
			m_UI[JOIN_ROOM].m_Button[0].Invisible();
			m_UI[JOIN_ROOM].m_Button[0].Disable();
			m_GameStatus = JOIN_ROOM;
			break;
		}
		default:
			break;
		}
		break;
	}
	case JOIN_ROOM:
	{
		switch (m_UIManager.back()->ButtonCheck())
		{
		case 0://准备按钮，并向服务器发送消息
		{
			m_UIManager.back()->ResetButtonCheck();
			m_UI[JOIN_ROOM].m_Button[0].Disable();
			m_UI[JOIN_ROOM].m_Button[0].Invisible();
			m_pMessageManager->PlayerConnect();
			break;
		}
		default:
			break;
		}
		break;
	}
	case GAME_USUAL:
	{
		int numButton = m_UIManager.back()->ButtonCheck();
		if (numButton > -1)
		{
			m_UIManager.back()->ResetButtonCheck();
			int cardSize = m_PlayerManager.GetMyPlayerCardType().size();
			for (int i = 0; i < cardSize; i++)//5张牌按钮
			{
				if (numButton == i&&m_PlayerManager.GetMyPlayerID()==m_PlayerManager.GetCurrPlayerID())
				{
					int card = m_PlayerManager.GetMyPlayerCardType()[i];
					{
						int buffer[16] = { 3, 4, m_PlayerManager.GetCurrPlayerID(), i };
						m_pMessageManager->Send(buffer);
					}
					CardEffect(card);
					break;
				}
			}
			if (numButton == 5 && m_PlayerManager.GetMyPlayerID() == m_PlayerManager.GetCurrPlayerID())			//轮盘
			{
				m_IsRouletteRotate = false;
				m_UI[GAME_USUAL].m_Button[5].Disable();//按了轮盘的时候先锁住轮盘
				int steps = 1 + (m_Roulette + 210) % 360 / 60;//移动的步数
				//发送行走步数
				int buffer[16] = { 3, 0, steps };
				m_pMessageManager->Send(buffer);

			}
			else if (numButton == 6)	//退出
			{
				PostQuitMessage(0);
			}
		}	
		break;
	}
	case BUY_LAND:
	{
		switch (m_UIManager.back()->ButtonCheck())
		{
		case 0://买地
		{
			m_UIManager.back()->ResetButtonCheck();
			m_UIManager.pop_back();
			int numButton = m_UIManager.back()->Button_Size();
			for (int i = 0; i < numButton; i++)
			{
				m_UIManager.back()->m_Button[i].Enable();
			}
			int i, size = m_PlayerManager.GetMyPlayerCardType().size();
			for (i = size; i < 5; i++)//隐藏没牌的按钮
			{
				m_UI[GAME_USUAL].m_Button[i].Disable();
				m_UI[GAME_USUAL].m_Button[i].Invisible();
			}
			m_GameStatus = GAME_USUAL;
			//发送消息当前玩家-1000Gold	
			{
				int buffer[16] = { 3, 3, m_PlayerManager.GetCurrPlayerID(), -1000 };
				m_pMessageManager->Send(buffer);
			}
			//玩家获得这块地
			{
				POINT index = m_PlayerManager.GetBlockIndex();
				int buffer[16] = {4, 5, index.x, index.y, m_PlayerManager.GetCurrPlayerID() };
				m_pMessageManager->Send(buffer);
			}
			//切换玩家
			{
				int buffer[16] = { 5 };
				m_PlayerManager.CalculateCurrPlayerInterests();
				m_pMessageManager->Send(buffer);
			}
			break;
		}
		case 1://不买
		{
			m_UIManager.back()->ResetButtonCheck();
			m_UIManager.pop_back();
			int numButton = m_UIManager.back()->Button_Size();
			for (int i = 0; i < numButton; i++)
			{
				m_UIManager.back()->m_Button[i].Enable();
			}
			int i, size = m_PlayerManager.GetMyPlayerCardType().size();
			for (i = size; i < 5; i++)//隐藏没牌的按钮
			{
				m_UI[GAME_USUAL].m_Button[i].Disable();
				m_UI[GAME_USUAL].m_Button[i].Invisible();
			}
			m_GameStatus = GAME_USUAL;
			//切换玩家
			{
				int buffer[16] = { 5 };
				m_PlayerManager.CalculateCurrPlayerInterests();
				m_pMessageManager->Send(buffer);
			}
			break;
		}
		default:
			break;
		}
		break;
	}
	case UPGRADE_LAND:
	{
		switch (m_UIManager.back()->ButtonCheck())
		{
		case 0://升级别墅
		{
			m_UIManager.back()->ResetButtonCheck();
			m_UIManager.pop_back();
			int numButton = m_UIManager.back()->Button_Size();
			for (int i = 0; i < numButton; i++)
			{
				m_UIManager.back()->m_Button[i].Enable();
			}
			int i, size = m_PlayerManager.GetMyPlayerCardType().size();
			for (i = size; i < 5; i++)//隐藏没牌的按钮
			{
				m_UI[GAME_USUAL].m_Button[i].Disable();
				m_UI[GAME_USUAL].m_Button[i].Invisible();
			}
			m_GameStatus = GAME_USUAL;
			//Message!
			// 该玩家扣 m_NumButtonReturn[0]这个量的Gold
			{
				int buffer[16] = { 3, 3, m_PlayerManager.GetCurrPlayerID(), -m_NumButtonReturn[0] };
				m_pMessageManager->Send(buffer);
			}
			// 改变房屋类型
			{
				POINT index = m_PlayerManager.GetBlockIndex();
				int buffer[16] = { 4, 8, index.x, index.y, Rich::HOUSETYPE::COTTAGE };
				m_pMessageManager->Send(buffer);
			}
			// 升级该地方房屋等级
			{
				POINT index = m_PlayerManager.GetBlockIndex();
				int buffer[16] = { 4, 4, index.x, index.y, 1 };
				m_pMessageManager->Send(buffer);
			}
			// 切换玩家
			{
				int buffer[16] = { 5 };
				m_PlayerManager.CalculateCurrPlayerInterests();
				m_pMessageManager->Send(buffer);
			}
			break;
		}
		case 1://升级住宅
		{
			m_UIManager.back()->ResetButtonCheck();
			m_UIManager.pop_back();
			int numButton = m_UIManager.back()->Button_Size();
			for (int i = 0; i < numButton; i++)
			{
				m_UIManager.back()->m_Button[i].Enable();
			}
			int i, size = m_PlayerManager.GetMyPlayerCardType().size();
			for (i = size; i < 5; i++)//隐藏没牌的按钮
			{
				m_UI[GAME_USUAL].m_Button[i].Disable();
				m_UI[GAME_USUAL].m_Button[i].Invisible();
			}
			m_GameStatus = GAME_USUAL;
			//扣玩家 m_NumButtonReturn[1]的金币
			{
				int buffer[16] = { 3, 3, m_PlayerManager.GetCurrPlayerID(), -m_NumButtonReturn[1] };
				m_pMessageManager->Send(buffer);
			}
			// 改变房屋类型
			{
				POINT index = m_PlayerManager.GetBlockIndex();
				int buffer[16] = { 4, 8, index.x, index.y, Rich::HOUSETYPE::FLAT };
				m_pMessageManager->Send(buffer);
			}
			//当前地方房屋升一级
			{
				POINT index = m_PlayerManager.GetBlockIndex();
				int buffer[16] = { 4, 4, index.x, index.y, 1 };
				m_pMessageManager->Send(buffer);
			}
			// 切换玩家
			{
				int buffer[16] = { 5 };
				m_PlayerManager.CalculateCurrPlayerInterests();
				m_pMessageManager->Send(buffer);
			}
			break;
		}
		default:
			break;
		}
		break;
	}
	case STORE:
	{
		int theButton = m_UIManager.back()->ButtonCheck();
		m_UIManager.back()->ResetButtonCheck();
		if (theButton > -1)
		{
			if (theButton == 0)//X
			{
				m_UIManager.pop_back();
				int numButton = m_UIManager.back()->Button_Size();
				for (int i = 0; i < numButton; i++)
				{
					m_UIManager.back()->m_Button[i].Enable();
				}
				int i, size = m_PlayerManager.GetMyPlayerCardType().size();
				for (i = size; i < 5; i++)//隐藏没牌的按钮
				{
					m_UI[GAME_USUAL].m_Button[i].Disable();
					m_UI[GAME_USUAL].m_Button[i].Invisible();
				}
				m_GameStatus = GAME_USUAL;
				//切换玩家
				{
					int buffer[16] = { 5 };
					m_PlayerManager.CalculateCurrPlayerInterests();
					m_pMessageManager->Send(buffer);
				}
			}
			for (int i = 1; i < 9; i++)//购买事件
			{
				if (theButton == i)
				{
					//用户点卷减少 m_NumButtonReturn[i]
					{
						int buffer[16] = { 3, 8, -m_pCards->GetCardByEnum(static_cast<Rich::CardType>(m_NumButtonReturn[i - 1])).silver};
						m_pMessageManager->Send(buffer);
					}
					//用户获得卡牌
					{						
						int buffer[16] = { 3, 5, m_PlayerManager.GetCurrPlayerID(), m_NumButtonReturn[i - 1] };
						m_pMessageManager->Send(buffer);
					}

					m_UI[STORE].m_Button[i].Invisible();
					m_UI[STORE].m_Button[i].Disable();
					m_UI[STORE].m_TextBox[i - 1].Invisible();
					int silver = m_PlayerManager.GetMyPlayerSilver();
					for (int j = 0; j < 8; j++)
					{
						if (silver < m_pCards->GetCardByEnum((Rich::CardType)m_NumButtonReturn[j]).silver)
						{
							m_UI[STORE].m_Button[j + 1].Disable();
						}
					}
					break;
				}
			}
		}
		break;
	}
	case STEAM:
	{
		switch (m_UIManager.back()->ButtonCheck())
		{
		case 0://X
		{
			m_UIManager.back()->ResetButtonCheck();
			m_UIManager.pop_back();
			int numButton = m_UIManager.back()->Button_Size();
			for (int i = 0; i < numButton; i++)
			{
				m_UIManager.back()->m_Button[i].Enable();
			}
			int i, size = m_PlayerManager.GetMyPlayerCardType().size();
			for (i = size; i < 5; i++)//隐藏没牌的按钮
			{
				m_UI[GAME_USUAL].m_Button[i].Disable();
				m_UI[GAME_USUAL].m_Button[i].Invisible();
			}
			m_GameStatus = GAME_USUAL;
			//切换玩家
			{
				int buffer[16] = { 5 };
				m_PlayerManager.CalculateCurrPlayerInterests();
				m_pMessageManager->Send(buffer);
			}
			break;
		}
		case 1:
		{
			m_UIManager.back()->ResetButtonCheck();
			//玩家金钱-=10000
			{
				int buffer[16] = { 3, 3, m_PlayerManager.GetCurrPlayerID(), -10000 };
				m_pMessageManager->Send(buffer);
			}
			{
				int randCard = rand() % 20 + 9;
				int buffer[16] = { 3, 5, m_PlayerManager.GetCurrPlayerID(), randCard };
				m_pMessageManager->Send(buffer);
			}
			m_SteamLess--;
			m_UI[STEAM].m_ImageBox[1].SetImage(m_pUITexture[m_SteamLess]);

			if (m_SteamLess > 60)
			{
				m_UI[STEAM].m_Button[1].Visible();
				m_UI[STEAM].m_Button[1].Enable();
			}
			else
			{
				m_UI[STEAM].m_Button[1].Invisible();
				m_UI[STEAM].m_Button[1].Disable();
			}
			break;
		}
		default:
			break;
		}
		break;
	}
	case ROULETTE:
	{
		int numButton = m_UIManager.back()->ButtonCheck();
		m_UIManager.back()->ResetButtonCheck();
		for (int i = 0; i < 6; i++)
		{
			if (numButton == i)
			{
				m_UIManager.pop_back();
				int buttonSize = m_UIManager.back()->Button_Size();
				for (int j = 0; j < buttonSize; j++)
				{
					m_UI[GAME_USUAL].m_Button[j].Enable();
				}
				int  size = m_PlayerManager.GetMyPlayerCardType().size();
				for (int j = size; j < 5; j++)//隐藏没牌的按钮
				{
					m_UI[GAME_USUAL].m_Button[j].Disable();
					m_UI[GAME_USUAL].m_Button[j].Invisible();
				}
				//玩家移动 i +1 格
				{
					int buffer[16] = { 3, 0, i + 1 };
					m_pMessageManager->Send(buffer);
				}
				m_GameStatus = GAME_USUAL;
				break;
			}
		}
		break;
	}
	case EVENTS:
	{
		switch (m_UIManager.back()->ButtonCheck())
		{
		case 0:
		{
			m_UIManager.back()->ResetButtonCheck();
			m_UIManager.pop_back();
			int numButton = m_UIManager.back()->Button_Size();
			for (int i = 0; i < numButton; i++)
			{
				m_UIManager.back()->m_Button[i].Enable();
			}
			int i, size = m_PlayerManager.GetMyPlayerCardType().size();
			for (i = size; i < 5; i++)//隐藏没牌的按钮
			{
				m_UI[GAME_USUAL].m_Button[i].Disable();
				m_UI[GAME_USUAL].m_Button[i].Invisible();
			}
			m_GameStatus = GAME_USUAL;
			break;
		}
		default:
			break;
		}
		break;
	}
	case CHOOSE_PLAYER:
	{
		int button = m_UIManager.back()->ButtonCheck();
		m_UIManager.back()->ResetButtonCheck();
		if ( button> -1)
		{
			for (int i = 0; i < 3; i++)
			{
				if (button == i)
				{
					switch (m_CardNeedUI)
					{
					case Rich::CARD_EXILE:		//放逐卡
					{
						//对m_NumButtonReturn[i]指向的玩家进行对应操作
						{
							int buffer[16] = {3, 4, m_NumButtonReturn[i]};
							m_pMessageManager->Send(buffer);
						}
						break;
					}
					case Rich::CARD_CHIEF:	//小偷卡
					{
						//对m_NumButtonReturn[i]指向的玩家进行对应操作
						{
							int buffer[16] = { 3, 4, m_NumButtonReturn[i], 0 };
							m_pMessageManager->Send(buffer);
							auto cards = m_PlayerManager.GetPlayerCardType(m_NumButtonReturn[i]);
							if (!cards.empty())
							{
								int buffer[16] = { 3, 5, m_PlayerManager.GetCurrPlayerID(), cards[0] };
								m_pMessageManager->Send(buffer);
							}

						}
						break;
					}
					case Rich::CARD_DECREASE_INTERESTS://减息卡
					{
						//对m_NumButtonReturn[i]指向的玩家进行对应操作
						int buffer[16] = { 3, 6, m_NumButtonReturn[i], -1 };
						m_pMessageManager->Send(buffer);
						break;
					}
					case Rich::CARD_AVERAGE_GOLD://均贫卡
					{
						//对m_NumButtonReturn[i]指向的玩家进行对应操作
						int buffer[16] = { 3, 9, m_NumButtonReturn[i] };
						m_pMessageManager->Send(buffer);
						break;
					}
					default:
						break;
					}
					int buttonSize = m_UI[GAME_USUAL].Button_Size();
					for (int i = 0; i < buttonSize; i++)
					{
						m_UI[GAME_USUAL].m_Button[i].Enable();
					}
					int cardSize = m_PlayerManager.GetMyPlayerCardType().size();
					for (int i = cardSize; i < 5; i++)
					{
						m_UI[GAME_USUAL].m_Button[i].Disable();
					}
					m_GameStatus = GAME_USUAL;
					m_UIManager.pop_back();
					break;
				}
			}
		}
		break;
	}
	default:
		break;
	}
	for (DX11UI::UI *& ui : m_UIManager)//更新UI界面
	{
		ui->Updata();
	}
}

void GameApp::CardEffect(int card)
{
	switch (card)
	{
	case Rich::CARD_STEP_ONE:
	{
		int buffer[16] = { 3, 0, 1 };
		m_pMessageManager->Send(buffer);
		break;
	}
	case Rich::CARD_STEP_TWO:
	{
		int buffer[16] = { 3, 0, 2 };
		m_pMessageManager->Send(buffer);
		break;
	}
	case Rich::CARD_STEP_THREE:
	{
		int buffer[16] = { 3, 0, 3 };
		m_pMessageManager->Send(buffer);
		break;
	}
	case Rich::CARD_STEP_FOUR:
	{
		int buffer[16] = { 3, 0, 4 };
		m_pMessageManager->Send(buffer);
		break;
	}
	case Rich::CARD_STEP_FIVE:
	{
		int buffer[16] = { 3, 0, 5 };
		m_pMessageManager->Send(buffer);
		break; 
	}
	case Rich::CARD_STEP_SIX:
	{
		int buffer[16] = { 3, 0, 6 };
		m_pMessageManager->Send(buffer);
		break;
	}
	case Rich::CARD_TURN_BACK:
	{
		int buffer[16] = { 3, 1 };
		m_pMessageManager->Send(buffer);
		break;
	}
	case Rich::CARD_STAY:
	{
		int buffer[16] = { 3, 0, 0 };
		m_pMessageManager->Send(buffer);
		break;
	}
	case Rich::CARD_ROULETTE:
	{
		int buttonSize = m_UI[GAME_USUAL].Button_Size();
		for (int i = 0; i < buttonSize; i++)
		{
			m_UI[GAME_USUAL].m_Button[i].Disable();
		}
		m_UIManager.push_back(&m_UI[ROULETTE]);
		m_GameStatus = ROULETTE;
		break;
	}
	case Rich::CARD_BARRIER:
	{
		int buffer[16] = { 3, 2 };
		m_pMessageManager->Send(buffer);
		break;
	}
	case Rich::CARD_DESTINY:
	{
		int events = rand() % 15;
		EventEffect(events);
		m_UI[EVENTS].m_ImageBox[0].SetImage(m_pUITexture[141 + events]);
		m_UI[EVENTS].m_TextBox[0].SetWords(m_UIWords[events + 9].c_str(), D2D1::ColorF::White);
		m_UIManager.push_back(&m_UI[EVENTS]);
		int buttonSize = m_UI[GAME_USUAL].Button_Size();
		for (int i = 0; i < buttonSize; i++)
		{
			m_UI[GAME_USUAL].m_Button[i].Disable();
		}
		m_GameStatus = EVENTS;
		break;
	}
	case Rich::CARD_GOLD:
	{
		int buffer[16] = { 3, 3, m_PlayerManager.GetCurrPlayerID(), rand() % 29900 + 100 };
		m_pMessageManager->Send(buffer);
		break;
	}
	case Rich::CARD_UPGRADE_HOUSE:
	{
		POINT index = m_PlayerManager.GetBlockIndex();
		int buffer[16] = { 4, 4, index.x, index.y, 1 };
		m_pMessageManager->Send(buffer);
		break;
	}
	case Rich::CARD_APPREICATE_STREET:
	{
		int buffer[16] = { 4, 1, m_PlayerManager.GetBlockIndex().x, 1 };
		m_pMessageManager->Send(buffer);
		break;
	}
	case Rich::CARD_DISMANTLE:
	{
		POINT index = m_PlayerManager.GetBlockIndex();
		int buffer[16] = { 4, 4, index.x, index.y, -1 };
		m_pMessageManager->Send(buffer);
		break;
	}//拆迁卡
	case Rich::CARD_DEPREICATE_STREET:
	{
		int buffer[16] = { 4, 1, m_PlayerManager.GetBlockIndex().x, -1 };
		m_pMessageManager->Send(buffer);
		break;
	}
	case Rich::CARD_EXILE:
	{
		m_CardNeedUI = Rich::CARD_EXILE;
		std::vector<int> alives = m_PlayerManager.GetAlivePlayersID();
		int myID = m_PlayerManager.GetCurrPlayerID();
		int buttonID = 0;
		for (int i = 0; i < alives.size(); i++)//存活ID
		{
			if (alives[i] == myID)//ID是自己就跳过
			{
				continue;
			}
			else
			{
				m_NumButtonReturn[buttonID] = alives[i];
				m_UI[CHOOSE_PLAYER].m_Button[buttonID].Visible();
				m_UI[CHOOSE_PLAYER].m_Button[buttonID].Enable();
				m_UI[CHOOSE_PLAYER].m_Button[buttonID].SetPicture_ButtonOut(m_pUITexture[153 + alives[i] * 3]);
				m_UI[CHOOSE_PLAYER].m_Button[buttonID].SetPicture_ButtonOn(m_pUITexture[154 + alives[i] * 3]);
				m_UI[CHOOSE_PLAYER].m_Button[buttonID].SetPicture_ButtonDown(m_pUITexture[155 + alives[i] * 3]);
				buttonID++;//切换到下一个按钮
			}
		}
		for (buttonID; buttonID < 3; buttonID++)//剩下的按钮不可见
		{
			m_UI[CHOOSE_PLAYER].m_Button[buttonID].Invisible();
			m_UI[CHOOSE_PLAYER].m_Button[buttonID].Disable();
		}
		int buttonSize = m_UI[GAME_USUAL].Button_Size();
		for (int i = 0; i < buttonSize; i++)
		{
			m_UI[GAME_USUAL].m_Button[i].Disable();
		}
		m_UIManager.push_back(&m_UI[CHOOSE_PLAYER]);
		m_GameStatus = CHOOSE_PLAYER;
		break;
	}
	case Rich::CARD_CHIEF:
	{
		m_CardNeedUI = Rich::CARD_CHIEF;
		std::vector<int> alives = m_PlayerManager.GetAlivePlayersID();
		int myID = m_PlayerManager.GetCurrPlayerID();
		int buttonID = 0;
		for (int i = 0; i < alives.size(); i++)//存活ID
		{
			if (alives[i] == myID)//ID是自己就跳过
			{
				continue;
			}
			else
			{
				m_NumButtonReturn[buttonID] = alives[i];
				m_UI[CHOOSE_PLAYER].m_Button[buttonID].Visible();
				m_UI[CHOOSE_PLAYER].m_Button[buttonID].Enable();
				m_UI[CHOOSE_PLAYER].m_Button[buttonID].SetPicture_ButtonOut(m_pUITexture[153 + alives[i] * 3]);
				m_UI[CHOOSE_PLAYER].m_Button[buttonID].SetPicture_ButtonOn(m_pUITexture[154 + alives[i] * 3]);
				m_UI[CHOOSE_PLAYER].m_Button[buttonID].SetPicture_ButtonDown(m_pUITexture[155 + alives[i] * 3]);
				buttonID++;//切换到下一个按钮
			}
		}
		for (buttonID; buttonID < 3; buttonID++)//剩下的按钮不可见
		{
			m_UI[CHOOSE_PLAYER].m_Button[buttonID].Invisible();
			m_UI[CHOOSE_PLAYER].m_Button[buttonID].Disable();
		}
		int buttonSize = m_UI[GAME_USUAL].Button_Size();
		for (int i = 0; i < buttonSize; i++)
		{
			m_UI[GAME_USUAL].m_Button[i].Disable();
		}
		m_UIManager.push_back(&m_UI[CHOOSE_PLAYER]);
		m_GameStatus = CHOOSE_PLAYER;
		break;
	}
	case Rich::CARD_DECREASE_INTERESTS:
	{
		m_CardNeedUI = Rich::CARD_DECREASE_INTERESTS;
		std::vector<int> alives = m_PlayerManager.GetAlivePlayersID();
		int myID = m_PlayerManager.GetCurrPlayerID();
		int buttonID = 0;
		for (int i = 0; i < alives.size(); i++)//存活ID
		{
			if (alives[i] == myID)//ID是自己就跳过
			{
				continue;
			}
			else
			{
				m_NumButtonReturn[buttonID] = alives[i];
				m_UI[CHOOSE_PLAYER].m_Button[buttonID].Visible();
				m_UI[CHOOSE_PLAYER].m_Button[buttonID].Enable();
				m_UI[CHOOSE_PLAYER].m_Button[buttonID].SetPicture_ButtonOut(m_pUITexture[153 + alives[i] * 3]);
				m_UI[CHOOSE_PLAYER].m_Button[buttonID].SetPicture_ButtonOn(m_pUITexture[154 + alives[i] * 3]);
				m_UI[CHOOSE_PLAYER].m_Button[buttonID].SetPicture_ButtonDown(m_pUITexture[155 + alives[i] * 3]);
				buttonID++;//切换到下一个按钮
			}
		}
		for (buttonID; buttonID < 3; buttonID++)//剩下的按钮不可见
		{
			m_UI[CHOOSE_PLAYER].m_Button[buttonID].Invisible();
			m_UI[CHOOSE_PLAYER].m_Button[buttonID].Disable();
		}
		int buttonSize = m_UI[GAME_USUAL].Button_Size();
		for (int i = 0; i < buttonSize; i++)
		{
			m_UI[GAME_USUAL].m_Button[i].Disable();
		}
		m_UIManager.push_back(&m_UI[CHOOSE_PLAYER]);
		m_GameStatus = CHOOSE_PLAYER;
		break;
	}//减息卡
	case Rich::CARD_INCREASE_INTERESTS:
	{
		int buffer[16] = { 3, 6, 1 };
		m_pMessageManager->Send(buffer);
		break;
	}
	case Rich::CARD_PURCASE:
	{
		POINT index = m_PlayerManager.GetBlockIndex();
		if (m_StreetManager.FuncTypeGet(index) == Rich::FUNCTYPE::HOUSE)
		{
			int buffer[16] = { 4, 5, index.x, index.y, m_PlayerManager.GetCurrPlayerID() };
			m_pMessageManager->Send(buffer);
		}
		break;
	}
	case Rich::CARD_AVERAGE_GOLD:
	{
		m_CardNeedUI = Rich::CARD_AVERAGE_GOLD;
		std::vector<int> alives = m_PlayerManager.GetAlivePlayersID();
		int myID = m_PlayerManager.GetCurrPlayerID();
		int buttonID = 0;
		for (int i = 0; i < alives.size(); i++)//存活ID
		{
			if (alives[i] == myID)//ID是自己就跳过
			{
				continue;
			}
			else
			{
				m_NumButtonReturn[buttonID] = alives[i];
				m_UI[CHOOSE_PLAYER].m_Button[buttonID].Visible();
				m_UI[CHOOSE_PLAYER].m_Button[buttonID].Enable();
				m_UI[CHOOSE_PLAYER].m_Button[buttonID].SetPicture_ButtonOut(m_pUITexture[153 + alives[i] * 3]);
				m_UI[CHOOSE_PLAYER].m_Button[buttonID].SetPicture_ButtonOn(m_pUITexture[154 + alives[i] * 3]);
				m_UI[CHOOSE_PLAYER].m_Button[buttonID].SetPicture_ButtonDown(m_pUITexture[155 + alives[i] * 3]);

				buttonID++;//切换到下一个按钮
			}
		}
		for (buttonID; buttonID < 3; buttonID++)//剩下的按钮不可见
		{
			m_UI[CHOOSE_PLAYER].m_Button[buttonID].Invisible();
			m_UI[CHOOSE_PLAYER].m_Button[buttonID].Disable();
		}
		int buttonSize = m_UI[GAME_USUAL].Button_Size();
		for (int i = 0; i < buttonSize; i++)
		{
			m_UI[GAME_USUAL].m_Button[i].Disable();
		}
		m_UIManager.push_back(&m_UI[CHOOSE_PLAYER]);
		m_GameStatus = CHOOSE_PLAYER;
		break;
	}//均贫卡
	case Rich::CARD_MONSTER:
	{
		int buffer[16] = { 4, 0, m_PlayerManager.GetBlockIndex().x };
		m_pMessageManager->Send(buffer);
		break;
	}
	case Rich::CARD_RUBAN:
	{
		POINT point = m_PlayerManager.GetBlockIndex();
		int buffer[16] = { 4, 7, point.x, point.y };
		m_pMessageManager->Send(buffer);
		break;
	}
	case Rich::CARD_ROB_TAX:
	{
		int buffer[16] = { 3, 10 };
		m_pMessageManager->Send(buffer);
		break;
	}
	case Rich::CARD_DEVIL:
	{
		int buffer[16] = { 4, 0, m_PlayerManager.GetBlockIndex().x };
		m_pMessageManager->Send(buffer);
		break;
	}
	case Rich::CARD_LAND:
	{
		POINT index = m_PlayerManager.GetBlockIndex();
		int buffer[16] = { 4, 5, index.x, index.y, m_PlayerManager.GetCurrPlayerID() };
		m_pMessageManager->Send(buffer);
		
		POINT nextIndex = m_StreetManager.BlockNextGet(index, 1);
		buffer[2] = nextIndex.x;
		buffer[3] = nextIndex.y;
		m_pMessageManager->Send(buffer);

		POINT prevIndex = m_StreetManager.BlockNextGet(index, -1);
		buffer[2] = prevIndex.x;
		buffer[3] = prevIndex.y;
		m_pMessageManager->Send(buffer);
		break;
	}
	case Rich::CARD_SAMSALA:
	{
		for (int i = 0; i < 7; ++i)
			EventEffect(rand() % 15);
		break;
	}
	default:
		break;
	}
}

void GameApp::EventEffect(int events)
{
	switch (events)
	{
		case EVENT_PEACE_NIGHT:
		{
			break;
		}															//平安夜
		case EVENT_RANDOM_HOUSE_UPGRADE:
		{
			POINT index = m_PlayerManager.GetBlockIndex();
			do {
				int randNum = rand() % 200;
				for (int i = 0; i < randNum; ++i)
					index = m_StreetManager.BlockNextGet(index, 1);
			} while (m_StreetManager.FuncTypeGet(index) != Rich::FUNCTYPE::HOUSE);
			int buffer[16] = { 4, 4, index.x, index.y, 1 };
			m_pMessageManager->Send(buffer);
			
			break;
		}							//随机房屋升级
		case EVENT_RANDOM_HOUSE_DEGRADE:
		{
			POINT index = m_PlayerManager.GetBlockIndex();
			do {
				int randNum = rand() % 200;
				for (int i = 0; i < randNum; ++i)
					index = m_StreetManager.BlockNextGet(index, 1);
			} while (m_StreetManager.FuncTypeGet(index) != Rich::FUNCTYPE::HOUSE);
			int buffer[16] = { 4, 4, index.x, index.y, -1 };
			m_pMessageManager->Send(buffer);
			break;
		}							//随机房屋降级
		case EVENT_AWARD_IN_ROTTERY:
		{
			int buffer[16] = { 3, 3, m_PlayerManager.GetCurrPlayerID(), rand() % 29900 + 100 };
			m_pMessageManager->Send(buffer);
			break;
		}											//买彩票中
		case EVENT_INVEST_FAILED:
		{
			int buffer[16] = { 3, 3, m_PlayerManager.GetCurrPlayerID(), -(rand() % 29900 + 100) };
			m_pMessageManager->Send(buffer);
			break;
		}														//投资失败
		case EVENT_EARTHQUAKE:
		{
			int buffer[16] = { 4, 2, rand() % 23, -1 };
			m_pMessageManager->Send(buffer);
			break;
		}															//地震灾害
		case EVENT_POLICY_ENCOURAGE:
		{
			int buffer[16] = { 4, 2, rand() % 23, 1 };
			m_pMessageManager->Send(buffer);
			break;
		}											//政策鼓励
		case EVENT_INFLATION:
		{
			auto alivePlayers = m_PlayerManager.GetAlivePlayersID();
			for (int i = 0; i < alivePlayers.size(); ++i)
			{
				int buffer[16] = { 3, 6, alivePlayers[i], 1 };
				m_pMessageManager->Send(buffer);
			}
			break;
		}																//通货膨胀
		case EVENT_POLICY_RETRENCH:
		{
			auto alivePlayers = m_PlayerManager.GetAlivePlayersID();
			for (int i = 0; i < alivePlayers.size(); ++i)
			{
				int buffer[16] = { 3, 6, alivePlayers[i], -1 };
				m_pMessageManager->Send(buffer);
			}
			break;
		}												//政策紧缩
		case EVENT_SELL_ALL_CARDS:
		{
			int buffer[16] = { 3, 7 };
			m_pMessageManager->Send(buffer);
			break;
		}													//变卖所有卡牌
		case EVENT_ENVIRONMENT_POLLUTION:
		{
			int buffer[16] = { 4, 1, rand() % 23, -1 };
			m_pMessageManager->Send(buffer);
			break;
		}							//环境污染
		case EVENT_HEATING_HOUSE_SELL:
		{
			int buffer[16] = { 4, 1, rand() % 23, 1 };
			m_pMessageManager->Send(buffer);
			break;
		}										//炒房热潮
		case EVENT_RECHARGE_SUCCESS:
		{
			int buffer[16] = { 3, 8, rand() % 40 + 10 };
			m_pMessageManager->Send(buffer);
			break;
		}											//充值成功
		case EVENT_HELP_MYSTERY:
		{
			int buffer[16] = { 3, 5, m_PlayerManager.GetCurrPlayerID(), rand() % 28 + 1};
			m_pMessageManager->Send(buffer);
			buffer[3] = rand() % 28 + 1;
			m_pMessageManager->Send(buffer);
			break;
		}														//救助神秘人
		case EVENT_HELP_OLDS: 
		{
			
			int buffer[16] = { 3, 4, m_PlayerManager.GetCurrPlayerID(), 0 };
			m_pMessageManager->Send(buffer);
			break; 
		}															//救助老人
	default:
		break;
	}
}


//  根据当前道路索引与方向确定人物的当前世界坐标与下一步的世界坐标
void GameApp::UpDatePlayerPosition(int ID)
{
	//  设置玩家当前位置
	POINT blockIndex = m_PlayerManager.GetBlockIndex(ID);
	XMFLOAT3 pos = m_StreetManager.BlockTransFloat3Get(blockIndex);
	m_PlayerManager.SetPlayerPosition(ID, pos);

	//  设置玩家下一步位置
	POINT nextPosIndex = m_StreetManager.BlockNextGet(
		m_PlayerManager.GetBlockIndex(ID),
		m_PlayerManager.GetDirection(ID));
	XMFLOAT3 nextPos = m_StreetManager.BlockTransFloat3Get(nextPosIndex);
	m_PlayerManager.SetTargetPosition(ID, nextPos);
}

//  玩家移动后把原来的道路索引设置为下一步的街道索引
void GameApp::UpDatePlayerBlockIndex(int ID)
{
	POINT blockIndex = m_StreetManager.BlockNextGet(
		m_PlayerManager.GetBlockIndex(ID),
		m_PlayerManager.GetDirection(ID)
	);
	m_PlayerManager.SetBlockIndex(ID, blockIndex);
}

//  人物行走后，更新人物下一步信息
void GameApp::UpddatePlayerNextStep()
{
	std::vector<int> aliveID = m_PlayerManager.GetAlivePlayersID();
	for (int ID : aliveID)
	{
		//  人物走完一步后的操作
		if (m_PlayerManager.IsUpdateNextStep(ID) == true)
		{
			UpDatePlayerBlockIndex(ID);
			UpDatePlayerPosition(ID);
	
			int playerID = m_PlayerManager.IsStandOnSamePosition(ID);
			//  移动的位置已经有玩家
			if (playerID != Rich::PLAYER_ID_DEFAULT)
			{
				m_PlayerManager.JustMove(playerID, 1);
			}

			//  移动的位置是否有路障
			POINT blockIndex = m_PlayerManager.GetBlockIndex(ID);
			if (m_StreetManager.BarrierGet(blockIndex) == true)
			{
				m_PlayerManager.StopPlayer(ID);
				m_StreetManager.BarrierSet(blockIndex, false);
			}
		}
	}
}

std::wstring GameApp::CharToWstring(LPCSTR pszSrc, int nLen)
{
	int nSize = MultiByteToWideChar(CP_ACP, 0, (LPCSTR)pszSrc, nLen, 0, 0);
	if (nSize <= 0) return NULL;

	WCHAR *pwszDst = new WCHAR[nSize + 1];
	if (NULL == pwszDst) return NULL;

	MultiByteToWideChar(CP_ACP, 0, (LPCSTR)pszSrc, nLen, pwszDst, nSize);
	pwszDst[nSize] = 0;

	if (pwszDst[0] == 0xFEFF) // skip Oxfeff  
		for (int i = 0; i < nSize; i++)
			pwszDst[i] = pwszDst[i + 1];

	std::wstring wcharString(pwszDst);
	delete pwszDst;

	return wcharString;

}
