#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "d3dUtil.h"

/*
* Camera类
* 作者：X_Jun
*
* 更新：
* 2017-8-25：修正TPS_ONLY模式下鼠标无限拉远，以及拉到最近报错的问题。  -By X_Jun
*
* 创建日期：2017-8-24
* 详述：
*     Camera类没有任何的静态成员/方法，它可以被声明出多份实例。
*     要使用Camera对象，首先需要按如下步骤进行对象操作：
*     ①初始化：在GameApp::InitCamera()中首先调用方法
* Camera::Initialize();   传入需要使用的FX。
*     ②设置摄像机模式：在GameApp::InitCamera()初始化摄像机后调
* 用方法Camera::SetCameraMode();   
*     其中  CameraMode::FPS_ONLY 只允许使用第一人称
*           CameraMode::TPS_ONLY 只允许使用第三人称
*           CameraMode::FPS      第一/三人称可切换，当前为第一人称
*           CameraMode::TPS      第一/三人称可切换，当前为第三人称
*     ③设置观察空间：在GameApp::InitCamera()初始化摄像机后调用方法
* Camera::SetView();   设置好摄像机初始状态（观察矩阵）
*     ④初始化/更新摄像机投影矩阵：在GameApp::InitCamera()初始化摄
* 像机后以及GameApp::OnResize()内都需要调用方法
* Camera::SetPerspective();   设置好投影矩阵
*     ⑤更新摄像机相关函数： 在GameApp::UpdateCamera()中需要根据当前
* 摄像机模式来选择调用相应的方法
*     FPS 或 FPS_ONLY
*     Camera::MoveForward();		// 角色向前/后移动
*     Camera::MoveRight();			// 角色向左/右移动
*     Camera::MoveUp();				// 角色垂直上/下移动
*     Camera::LookRight();			// 角色向左/右观察
*     Camera::LookUp();				// 角色向上/下观察
*
*     TPS 或 TPS_ONLY
*     首先必须调用Camera::SetTarget(); 来跟踪目标，然后才可以调用下面的函数
*     Camera::LookAroundRight();	// 摄像机绕物体向左/右旋转
*     Camera::LookAroundUp();       // 摄像机绕物体向上/下旋转
*
*     FPS 或 TPS：
*     Camera::Zoom();               // 摄像机拉近/远，拉近到一定程度变第一人称
*                                   // 第一人称拉出则变第三人称
*     ⑥应用更新： 在GameApp::UpdateCamera()中完成步骤⑤后应调用方法
* GameApp::UpdateViewMatrix(); 来应用所有步骤⑤的调用。
*/


enum class CameraMode { 
	FPS_ONLY,	// 只允许第一人称
	TPS_ONLY,	// 只允许第三人称
	FPS,		// 在可切换视角的情况下，当前为FPS
	TPS			// 在可切换视角的情况下，当前为TPS
};	

class Camera {
public:
	Camera();
	~Camera();

	// 初始化Effect
	void Initialize(ID3DX11Effect * effect);
	// 设置摄像机模式
	void SetCameraMode(CameraMode mode) { m_Mode = mode; }

	// 获取摄像机在世界中的位置(XMVECTOR)
	XMVECTOR GetXMVectorPos() const { return XMLoadFloat3(&m_Position); }
	// 获取摄像机在世界中的位置(XMFLOAT3)
	XMFLOAT3 GetXMFloat3Pos() const { return m_Position; }
	// 设置摄像机在世界中的位置
	void SetPosition(float x, float y, float z) { m_Position = XMFLOAT3(x, y, z); }
	// 设置摄像机在世界中的位置
	void SetPosition(const XMFLOAT3& v) { m_Position = v; }

	// 获取摄像机的右方向向量(XMVECTOR)
	XMVECTOR GetXMVectorRight() const { return XMLoadFloat3(&m_Right); }
	// 获取摄像机的右方向向量(XMFLOAT3)
	XMFLOAT3 GetXMFloat3Right() const { return m_Right; }
	// 获取摄像机的上方向向量(XMVECTOR)
	XMVECTOR GetXMVectorUp() const { return XMLoadFloat3(&m_Up); }
	// 获取摄像机的上方向向量(XMFLOAT3)
	XMFLOAT3 GetXMFloat3Up() const { return m_Up; }
	// 获取摄像机的目标方向向量(XMVECTOR)
	XMVECTOR GetXMVectorLook() const { return XMLoadFloat3(&m_Look); }
	// 获取摄像机的目标方向向量(XMFLOAT3)
	XMFLOAT3 GetXMFloat3Look() const { return m_Look; }

	// 获取近平面窗口距离
	float GetNearZ() const { return m_NearZ; }
	// 获取远平面窗口距离
	float GetFarZ() const { return m_FarZ; }
	// 获取宽高比
	float GetAspect() const { return m_Aspect; }
	// 获取垂直视野弧度
	float GetFovY() const { return m_FovY; }
	// 获取水平视野弧度
	float GetFovX() const { return 2.0f * atanf(0.5f * GetNearWindowWidth() / m_NearZ); }

	// 获取近平面窗口宽度
	float GetNearWindowWidth() const { return m_Aspect * m_NearWindowHeight; }
	// 获取近平面窗口高度
	float GetNearWindowHeight() const { return m_NearWindowHeight; }
	// 获取远平面窗口宽度
	float GetFarWindowWidth() const { return m_Aspect * m_FarWindowHeight; }
	// 获取远平面窗口宽度
	float GetFarWindowHeight() const { return m_FarWindowHeight; }

	// 设置投影矩阵
	void SetPerspective(float fovY, float aspect, float nearZ, float farZ);
	// 设置观察矩阵
	void SetView(FXMVECTOR pos, FXMVECTOR target, FXMVECTOR worldUp);
	// 设置观察矩阵
	void SetView(const XMFLOAT3& pos, const XMFLOAT3& target, const XMFLOAT3& worldUp);

	// 获取观察矩阵
	XMFLOAT4X4 GetView() const { return m_View; }
	// 获取投影矩阵
	XMFLOAT4X4 GetProj() const { return m_Proj; }

	// 获取观察矩阵
	XMMATRIX GetXMView() const { return XMLoadFloat4x4(&m_View); }
	// 获取投影矩阵
	XMMATRIX GetXMProj() const { return XMLoadFloat4x4(&m_Proj); }
	// 获取观察投影矩阵
	XMMATRIX GetXMViewProj() const { return GetXMView() * GetXMProj(); }

	// 前(W)后(S)移动，正值向前
	void MoveForward(float distance);
	// 左(A)右(D)平移，正值向左
	void MoveRight(float distance);
	// 垂直上下平移，正值向上
	void MoveUp(float distance);
	// 视野左右旋转(鼠标左右平移)，正值向左
	void LookRight(float radian);
	// 视野上下旋转(鼠标上下平移)，正值向上
	void LookUp(float radian);

	// 设置观察目标(仅可以用于非FPS_ONLY模式)
	void SetTarget(const XMFLOAT3& target) { if (m_Mode != CameraMode::FPS_ONLY) m_Target = target; }
	// 视野围绕物体左右旋转
	void LookAroundRight(float radian);
	// 视野围绕物体上下旋转，正值逆时针
	void LookAroundUp(float radian);
	// 拉近或拉远
	void Zoom(float offset, float minDist, float maxDist);
	
	// 更新观察矩阵
	void UpdateViewMatrix();
	

private:
	// 摄像机模式
	CameraMode m_Mode;
	// 摄像机坐标系与世界坐标系的关系
	XMFLOAT3 m_Position;
	XMFLOAT3 m_Right;
	XMFLOAT3 m_Up;
	XMFLOAT3 m_Look;
	
	// 摄像机第三人称模式下球面坐标系信息
	float m_Theta;
	float m_Phi;
	float m_Radius;
	XMFLOAT3 m_Target;

	// 投影矩阵参数
	float m_NearZ;
	float m_FarZ;
	float m_Aspect;
	float m_FovY;
	float m_NearWindowHeight;
	float m_FarWindowHeight;

	// 观察矩阵/投影矩阵
	XMFLOAT4X4 m_View;
	XMFLOAT4X4 m_Proj;

	// Effect 变量
	ID3DX11Effect* m_pFX;
	ID3DX11EffectVectorVariable* m_pFXEyePosW;
	ID3DX11EffectMatrixVariable* m_pFXView;
	ID3DX11EffectMatrixVariable* m_pFXProj;
};

#endif