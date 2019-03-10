#include "GameApp.h"
#include "XUtility.h"
GameApp::GameApp(HINSTANCE hInstance)
	: D3DApp(hInstance), m_pFXBasic(nullptr), m_pTech(nullptr)
{
	m_MainWndCaption = L"融合测试";
}

GameApp::~GameApp()
{
	ReleaseCOM(m_pFXBasic);
	ReleaseCOM(m_NoCullRS);
	ReleaseStatic(D2DText);
	ReleaseStatic(Rich::Sprite3D);
	ReleaseStatic(MKInput);
	ReleaseStatic(LightManager);
}

bool GameApp::Init()
{
	if (!D3DApp::Init())
		return false;

	InitScene();
	InitFX();
	InitLight();
	InitModel();
	InitDInput();
	InitText();
	InitCamera();
	InitRenderState();
	InitStreet();

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
	m_StreetManager.ModelScaleSet(0.03f);
#pragma region 示例
	m_StreetManager.HouseOwnerSet({ 0,0 }, 1);
	m_StreetManager.HouseTypeSet({ 0,0 }, Rich::COTTAGE);
	m_StreetManager.HouseLevelSet({ 0,0 }, 1);

	m_StreetManager.HouseOwnerSet({ 0,1 }, 1);
	m_StreetManager.HouseTypeSet({ 0,1 }, Rich::COTTAGE);
	m_StreetManager.HouseLevelSet({ 0,1 }, 2);

	m_StreetManager.HouseOwnerSet({ 2,0 }, 1);
	m_StreetManager.HouseTypeSet({ 2,0 }, Rich::COTTAGE);
	m_StreetManager.HouseLevelSet({ 2,0 }, 1);

	m_StreetManager.HouseOwnerSet({ 3,1 }, 1);
	m_StreetManager.HouseTypeSet({ 3,1 }, Rich::COTTAGE);
	m_StreetManager.HouseLevelSet({ 3,1 }, 2);

	m_StreetManager.HouseOwnerSet({ 1,0 }, 1);
	m_StreetManager.HouseTypeSet({ 1,0 }, Rich::COTTAGE);
	m_StreetManager.HouseLevelSet({ 1,0 }, 1);

	m_StreetManager.HouseOwnerSet({ 1,1 }, 1);
	m_StreetManager.HouseTypeSet({ 1,1 }, Rich::COTTAGE);
	m_StreetManager.HouseLevelSet({ 1,1 }, 2);

	m_StreetManager.HouseOwnerSet({ 1,2 }, 1);
	m_StreetManager.HouseTypeSet({ 1,2 }, Rich::FLAT);
	m_StreetManager.HouseLevelSet({ 1,2 }, 3);

	//m_StreetManager.HutongDestroy(1);
	//m_StreetManager.HutongLevelChange(0, 2);
	m_StreetManager.BarrierSet({ 1,0 }, true);
	m_StreetManager.HouseOwnerSet({ 1,3 }, 2);
	m_StreetManager.BarrierSet({ 1,1 }, true);
	m_StreetManager.BarrierSet({ 1,1 }, false);
	m_StreetManager.Rent({ 1,1 });
#pragma endregion
}

void GameApp::InitFX()
{
	m_pFXBasic = nullptr;
	// 初始化Effect
	BuildFX(m_pD3DDevice, L"HLSL/Basic.hlsl", &m_pFXBasic);
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
	XMVECTOR eyePos = XMVectorSet(100.0f, 0.1f, -100.0f, 1.0f);
	XMVECTOR targetPos = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	XMVECTOR upVec = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	m_Camera.Initialize(m_pFXBasic);
	m_Camera.SetView(eyePos, targetPos, upVec);
	m_Camera.SetPerspective(0.25f * MathHelper::Pi, AspectRatio(), 1.0f, 1000.0f);
}


void GameApp::OnResize()
{
	D2DText::Discard();
	D3DApp::OnResize();
	D2DText::OnReset();

	// 由于窗口大小重新调整，更新宽高比和重新计算出投影矩阵
	m_Camera.SetPerspective(0.25f * MathHelper::Pi, AspectRatio(), 1.0f, 1000.0f);
	Rich::Sprite3D::UpdateFrustum(m_Camera.GetProj());
}

void GameApp::UpdateScene(float dt)
{
	m_pMKInput->Update();
	UpdateCamera(dt);
	Rich::Sprite3D::UpdateViewMatrix(m_Camera.GetView());

	XMFLOAT4X4 s;
	XMStoreFloat4x4(&s, XMMatrixIdentity() * XMMatrixScaling(0.05f, 0.05f, 0.05f));
	m_Sprite.UpdateWorldMatrixes({ s });

	m_StreetManager.UpdateScene();
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
	m_Camera.LookAroundRight(XMFLOAT3(0.0f, 0.0f, 0.0f), m_pMKInput->GetMouseLXY().x * 0.006f);
	m_Camera.LookAroundUp(XMFLOAT3(0.0f, 0.0f, 0.0f), m_pMKInput->GetMouseLXY().y * 0.006f);
	m_Camera.Zoom(XMFLOAT3(0.0f, 0.0f, 0.0f), m_pMKInput->GetMouseLRoll() * 1.0f, 4.0f, 8.0f);

	m_Camera.UpdateViewMatrix();
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

	//场景
	XMFLOAT4X4 ViewProj;
	XMStoreFloat4x4(&ViewProj, m_Camera.GetXMViewProj());
	m_Scene.draw(ViewProj, m_Camera.GetXMFloat3Pos());
	//街道
	m_StreetManager.Draw();

	m_Sprite.DrawInstances();
	// 绘制完3D后就要来到2D部分了
	Draw2D();
	HR(m_pSwapChain->Present(0, 0));


}

void GameApp::Draw2D()
{


}




