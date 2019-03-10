#include "Camera.h"


Camera::Camera()
	: m_Position(0.0f, 0.0f, 0.0f),
	m_Right(1.0f, 0.0f, 0.0f),
	m_Up(0.0f, 1.0f, 0.0f),
	m_Look(0.0f, 0.0f, 1.0f),
	m_pFX(nullptr),
	m_pFXProj(nullptr),
	m_pFXView(nullptr),
	m_pFXEyePosW(nullptr),
	m_Mode(CameraMode::FPS_ONLY)
{
}

Camera::~Camera()
{
}

void Camera::Initialize(ID3DX11Effect * effect)
{
	m_pFX = effect;
	if (m_pFX)
	{
		m_pFXEyePosW = m_pFX->GetVariableByName("gEyePosW")->AsVector();
		m_pFXView = m_pFX->GetVariableByName("gView")->AsMatrix();
		m_pFXProj = m_pFX->GetVariableByName("gProj")->AsMatrix();
		m_pFXView->SetMatrix((const float *)&m_View);
		m_pFXProj->SetMatrix((const float *)&m_Proj);
		XMFLOAT3 zero(0.0f, 0.0f, 0.0f);
		m_pFXEyePosW->SetRawValue(&zero, 0, sizeof(zero));
	}
	

}

void Camera::SetPerspective(float fovY, float aspect, float nearZ, float farZ)
{
	m_FovY = fovY;
	m_Aspect = aspect;
	m_NearZ = nearZ;
	m_FarZ = farZ;

	m_NearWindowHeight = 2.0f * m_NearZ * tanf(0.5 * m_FovY);
	m_FarWindowHeight = 2.0f * m_FarZ * tanf(0.5 * m_FovY);

	XMMATRIX P = XMMatrixPerspectiveFovLH(m_FovY, m_Aspect, m_NearZ, m_FarZ);
	XMStoreFloat4x4(&m_Proj, P);

	if (m_pFXProj)
		m_pFXProj->SetMatrix((const float *)&m_Proj);
}

void Camera::SetView(FXMVECTOR pos, FXMVECTOR target, FXMVECTOR worldUp)
{
	XMMATRIX xmView = XMMatrixLookAtLH(pos, target, worldUp);
	// 更新向量
	m_Right = XMFLOAT3(xmView(0, 0), xmView(1, 0), xmView(2, 0));
	m_Up = XMFLOAT3(xmView(0, 1), xmView(1, 1), xmView(2, 1));
	m_Look = XMFLOAT3(xmView(0, 2), xmView(1, 2), xmView(2, 2));
	XMStoreFloat3(&m_Position, pos);
	// 更新半径
	m_Radius = XMVectorGetX(XMVector3Length(target - pos));
	// 更新观察矩阵
	XMStoreFloat4x4(&m_View, xmView);
	// 更新角度
	m_Phi = acosf(XMVectorGetX(XMVector3Dot(XMLoadFloat3(&m_Up), g_XMIdentityR1) / XMVector3Length(XMLoadFloat3(&m_Up))));
	m_Theta = acosf(XMVectorGetX(XMVector3Dot(XMLoadFloat3(&m_Right), g_XMIdentityR0) / XMVector3Length(XMLoadFloat3(&m_Right))));
}

void Camera::SetView(const XMFLOAT3 & pos, const XMFLOAT3 & target, const XMFLOAT3 & worldUp)
{
	XMVECTOR P = XMLoadFloat3(&pos);
	XMVECTOR T = XMLoadFloat3(&target);
	XMVECTOR U = XMLoadFloat3(&worldUp);

	SetView(P, T, U);
}

void Camera::MoveForward(float distance)
{
	// 仅可用于第一人称
	if (m_Mode == CameraMode::FPS || m_Mode == CameraMode::FPS_ONLY)
	{
		// m_Position += distance * m_Look
		XMVECTOR dist = XMVectorReplicate(distance);
		XMVECTOR look = XMLoadFloat3(&m_Look);
		XMVECTOR pos = XMLoadFloat3(&m_Position);
		XMStoreFloat3(&m_Position, XMVectorMultiplyAdd(dist, look, pos));
	}
}

void Camera::MoveRight(float distance)
{
	// 仅可用于第一人称
	if (m_Mode == CameraMode::FPS || m_Mode == CameraMode::FPS_ONLY)
	{
		// m_Position += distance * m_Right
		XMVECTOR dist = XMVectorReplicate(distance);
		XMVECTOR right = XMLoadFloat3(&m_Right);
		XMVECTOR pos = XMLoadFloat3(&m_Position);
		XMStoreFloat3(&m_Position, XMVectorMultiplyAdd(dist, right, pos));
	}
	
}

void Camera::MoveUp(float distance)
{
	// 仅可用于第一人称
	if (m_Mode == CameraMode::FPS || m_Mode == CameraMode::FPS_ONLY)
		m_Position.y += distance;
}


void Camera::LookRight(float radian)
{
	// 仅可用于第一人称
	if (m_Mode == CameraMode::FPS || m_Mode == CameraMode::FPS_ONLY)
	{
		XMMATRIX R = XMMatrixRotationY(radian);

		XMStoreFloat3(&m_Right, XMVector3TransformNormal(XMLoadFloat3(&m_Right), R));
		XMStoreFloat3(&m_Look, XMVector3TransformNormal(XMLoadFloat3(&m_Look), R));
	}
	
}

void Camera::LookUp(float radian)
{
	// 仅可用于第一人称
	if (m_Mode == CameraMode::FPS || m_Mode == CameraMode::FPS_ONLY)
	{
		// 绕右轴旋转，求出旋转㐇
		XMMATRIX R = XMMatrixRotationAxis(XMLoadFloat3(&m_Right), radian);

		XMVECTOR look = XMVector3TransformNormal(XMLoadFloat3(&m_Look), R);
		// xoz投影
		XMVECTOR proj_xoz = XMVectorSwizzle(look, 0, 3, 2, 3);
		// Y轴和Look向量的夹角余弦
		XMVECTOR XMCos = XMVector3Dot(proj_xoz, look) / XMVector3Length(proj_xoz) / XMVector3Length(look);
		// 仰俯角最大为80度
		float Flimit = cosf(MathHelper::Pi / 9 * 4);
		float Fcos = MathHelper::Clamp(XMVectorGetX(XMCos), -Flimit, Flimit);
		if (Fcos == -Flimit || Fcos == Flimit)
		{
			XMStoreFloat3(&m_Up, XMVector3TransformNormal(XMLoadFloat3(&m_Up), R));
			XMStoreFloat3(&m_Look, look);
		}
	}
}

void Camera::LookAroundRight(float radian)
{
	// 仅可用于第三人称
	if (m_Mode == CameraMode::TPS || m_Mode == CameraMode::TPS_ONLY)
	{
		m_Theta = fmod(m_Theta - radian, MathHelper::Pi * 2);
	}
}


void Camera::LookAroundUp(float radian)
{
	// 仅可用于第三人称
	if (m_Mode == CameraMode::TPS || m_Mode == CameraMode::TPS_ONLY)
	{
		m_Phi -= radian;
		//控制Phi在10度到80度之间
		if (m_Phi < MathHelper::Pi / 18)
			m_Phi = MathHelper::Pi / 18;
		else if (m_Phi > MathHelper::Pi / 18 * 8)
			m_Phi = MathHelper::Pi / 18 * 8;
	}
}


void Camera::Zoom(float offset, float minDist, float maxDist)
{
	// 对距离为0.0f、FPS_ONLY模式和FPS滚轮(上)不执行
	if (offset == 0.0f || m_Mode == CameraMode::FPS_ONLY || m_Mode == CameraMode::FPS && offset > 0.0f)
		return;
	// 若在FPS模式下使用滚轮(下)，转为TPS并拉出一段距离
	if (m_Mode == CameraMode::FPS && offset < 0.0f)
	{
		m_Mode = CameraMode::TPS;
		// 取得半径
		m_Radius = (minDist + maxDist) / 2;
		// 求出摄像机到目标点的向量
		XMVECTOR posToTarget = XMVectorReplicate(m_Radius) * XMLoadFloat3(&m_Look);
		// 求出Phi 
		m_Phi = acosf(XMVectorGetX(XMVector3Dot(posToTarget, g_XMIdentityR1) / XMVector3Length(posToTarget)));
		// 求出Theta
		m_Theta = acosf(XMVectorGetX(XMVector3Dot(posToTarget, g_XMIdentityR0) / XMVector3Length(posToTarget)));
		if (XMVector4Greater(g_XMZero, posToTarget))
			m_Theta *= -1;
		return;
	}
	
	// TPS和TPS_ONLY模式下都支持滚轮
		m_Radius += -offset;
	if ((m_Mode == CameraMode::TPS || m_Mode == CameraMode::TPS_ONLY) && m_Radius > maxDist)
		m_Radius = maxDist;
	// 若在TPS模式下使用滚轮(上)，离物体近到一定程度时转为FPS
	if (m_Radius <= minDist)
	{
		if (m_Mode == CameraMode::TPS)
		{
			m_Mode = CameraMode::FPS;
			m_Position = m_Target;
		}
		else if (m_Mode == CameraMode::TPS_ONLY)
		{
			m_Radius = minDist;
		}
	}

}




void Camera::UpdateViewMatrix()
{
	assert(m_pFX);
	if (m_Mode == CameraMode::TPS || m_Mode == CameraMode::TPS_ONLY)
	{
		// 球面坐标系
		float x = m_Target.x + m_Radius * sinf(m_Phi) * cosf(m_Theta);
		float z = m_Target.z + m_Radius * sinf(m_Phi) * sinf(m_Theta);
		float y = m_Target.y + m_Radius * cosf(m_Phi);
		
		
		XMVECTOR pos = XMVectorSet(x, y, z, 1.0f);
		XMMATRIX xmView = XMMatrixLookAtLH(pos, XMLoadFloat3(&m_Target), g_XMIdentityR1);
		
		// 更新向量
		m_Right = XMFLOAT3(m_View(0, 0), m_View(1, 0), m_View(2, 0));
		m_Up = XMFLOAT3(m_View(0, 1), m_View(1, 1), m_View(2, 1));
		m_Look = XMFLOAT3(m_View(0, 2), m_View(1, 2), m_View(2, 2));
		m_Position = XMFLOAT3{ x, y, z };
		// 更新矩阵
		XMStoreFloat4x4(&m_View, xmView);
		
		m_pFXView->SetMatrix((const float*)&xmView);
		m_pFXEyePosW->SetRawValue(&pos, 0, sizeof(pos));
	}



	if (m_Mode == CameraMode::FPS || m_Mode == CameraMode::FPS_ONLY)
	{
		XMVECTOR R = XMVector3Normalize(XMLoadFloat3(&m_Right));
		XMVECTOR L = XMVector3Normalize(XMLoadFloat3(&m_Look));
		XMVECTOR P = XMLoadFloat3(&m_Position);
		// 求出真正的Up向量
		XMVECTOR U = XMVector3Normalize(XMVector3Cross(L, R));

		float Qx = -XMVectorGetX(XMVector3Dot(P, R));
		float Qy = -XMVectorGetX(XMVector3Dot(P, U));
		float Qz = -XMVectorGetX(XMVector3Dot(P, L));

		XMStoreFloat3(&m_Right, R);
		XMStoreFloat3(&m_Up, U);
		XMStoreFloat3(&m_Look, L);

		m_View(0, 0) = m_Right.x;
		m_View(1, 0) = m_Right.y;
		m_View(2, 0) = m_Right.z;
		m_View(3, 0) = Qx;

		m_View(0, 1) = m_Up.x;
		m_View(1, 1) = m_Up.y;
		m_View(2, 1) = m_Up.z;
		m_View(3, 1) = Qy;

		m_View(0, 2) = m_Look.x;
		m_View(1, 2) = m_Look.y;
		m_View(2, 2) = m_Look.z;
		m_View(3, 2) = Qz;

		m_View(0, 3) = 0.0f;
		m_View(1, 3) = 0.0f;
		m_View(2, 3) = 0.0f;
		m_View(3, 3) = 1.0f;

		m_pFXView->SetMatrix((const float *)&m_View);
		m_pFXEyePosW->SetRawValue(&m_Position, 0, sizeof(m_Position));
	}
	
}

