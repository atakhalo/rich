#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "d3dUtil.h"

/*
* Camera��
* ���ߣ�X_Jun
*
* ���£�
* 2017-8-25������TPS_ONLYģʽ�����������Զ���Լ����������������⡣  -By X_Jun
*
* �������ڣ�2017-8-24
* ������
*     Camera��û���κεľ�̬��Ա/�����������Ա����������ʵ����
*     Ҫʹ��Camera����������Ҫ�����²�����ж��������
*     �ٳ�ʼ������GameApp::InitCamera()�����ȵ��÷���
* Camera::Initialize();   ������Ҫʹ�õ�FX��
*     �����������ģʽ����GameApp::InitCamera()��ʼ����������
* �÷���Camera::SetCameraMode();   
*     ����  CameraMode::FPS_ONLY ֻ����ʹ�õ�һ�˳�
*           CameraMode::TPS_ONLY ֻ����ʹ�õ����˳�
*           CameraMode::FPS      ��һ/���˳ƿ��л�����ǰΪ��һ�˳�
*           CameraMode::TPS      ��һ/���˳ƿ��л�����ǰΪ�����˳�
*     �����ù۲�ռ䣺��GameApp::InitCamera()��ʼ�����������÷���
* Camera::SetView();   ���ú��������ʼ״̬���۲����
*     �ܳ�ʼ��/���������ͶӰ������GameApp::InitCamera()��ʼ����
* ������Լ�GameApp::OnResize()�ڶ���Ҫ���÷���
* Camera::SetPerspective();   ���ú�ͶӰ����
*     �ݸ����������غ����� ��GameApp::UpdateCamera()����Ҫ���ݵ�ǰ
* �����ģʽ��ѡ�������Ӧ�ķ���
*     FPS �� FPS_ONLY
*     Camera::MoveForward();		// ��ɫ��ǰ/���ƶ�
*     Camera::MoveRight();			// ��ɫ����/���ƶ�
*     Camera::MoveUp();				// ��ɫ��ֱ��/���ƶ�
*     Camera::LookRight();			// ��ɫ����/�ҹ۲�
*     Camera::LookUp();				// ��ɫ����/�¹۲�
*
*     TPS �� TPS_ONLY
*     ���ȱ������Camera::SetTarget(); ������Ŀ�꣬Ȼ��ſ��Ե�������ĺ���
*     Camera::LookAroundRight();	// ���������������/����ת
*     Camera::LookAroundUp();       // ���������������/����ת
*
*     FPS �� TPS��
*     Camera::Zoom();               // ���������/Զ��������һ���̶ȱ��һ�˳�
*                                   // ��һ�˳������������˳�
*     ��Ӧ�ø��£� ��GameApp::UpdateCamera()����ɲ���ݺ�Ӧ���÷���
* GameApp::UpdateViewMatrix(); ��Ӧ�����в���ݵĵ��á�
*/


enum class CameraMode { 
	FPS_ONLY,	// ֻ�����һ�˳�
	TPS_ONLY,	// ֻ��������˳�
	FPS,		// �ڿ��л��ӽǵ�����£���ǰΪFPS
	TPS			// �ڿ��л��ӽǵ�����£���ǰΪTPS
};	

class Camera {
public:
	Camera();
	~Camera();

	// ��ʼ��Effect
	void Initialize(ID3DX11Effect * effect);
	// ���������ģʽ
	void SetCameraMode(CameraMode mode) { m_Mode = mode; }

	// ��ȡ������������е�λ��(XMVECTOR)
	XMVECTOR GetXMVectorPos() const { return XMLoadFloat3(&m_Position); }
	// ��ȡ������������е�λ��(XMFLOAT3)
	XMFLOAT3 GetXMFloat3Pos() const { return m_Position; }
	// ����������������е�λ��
	void SetPosition(float x, float y, float z) { m_Position = XMFLOAT3(x, y, z); }
	// ����������������е�λ��
	void SetPosition(const XMFLOAT3& v) { m_Position = v; }

	// ��ȡ��������ҷ�������(XMVECTOR)
	XMVECTOR GetXMVectorRight() const { return XMLoadFloat3(&m_Right); }
	// ��ȡ��������ҷ�������(XMFLOAT3)
	XMFLOAT3 GetXMFloat3Right() const { return m_Right; }
	// ��ȡ��������Ϸ�������(XMVECTOR)
	XMVECTOR GetXMVectorUp() const { return XMLoadFloat3(&m_Up); }
	// ��ȡ��������Ϸ�������(XMFLOAT3)
	XMFLOAT3 GetXMFloat3Up() const { return m_Up; }
	// ��ȡ�������Ŀ�귽������(XMVECTOR)
	XMVECTOR GetXMVectorLook() const { return XMLoadFloat3(&m_Look); }
	// ��ȡ�������Ŀ�귽������(XMFLOAT3)
	XMFLOAT3 GetXMFloat3Look() const { return m_Look; }

	// ��ȡ��ƽ�洰�ھ���
	float GetNearZ() const { return m_NearZ; }
	// ��ȡԶƽ�洰�ھ���
	float GetFarZ() const { return m_FarZ; }
	// ��ȡ��߱�
	float GetAspect() const { return m_Aspect; }
	// ��ȡ��ֱ��Ұ����
	float GetFovY() const { return m_FovY; }
	// ��ȡˮƽ��Ұ����
	float GetFovX() const { return 2.0f * atanf(0.5f * GetNearWindowWidth() / m_NearZ); }

	// ��ȡ��ƽ�洰�ڿ��
	float GetNearWindowWidth() const { return m_Aspect * m_NearWindowHeight; }
	// ��ȡ��ƽ�洰�ڸ߶�
	float GetNearWindowHeight() const { return m_NearWindowHeight; }
	// ��ȡԶƽ�洰�ڿ��
	float GetFarWindowWidth() const { return m_Aspect * m_FarWindowHeight; }
	// ��ȡԶƽ�洰�ڿ��
	float GetFarWindowHeight() const { return m_FarWindowHeight; }

	// ����ͶӰ����
	void SetPerspective(float fovY, float aspect, float nearZ, float farZ);
	// ���ù۲����
	void SetView(FXMVECTOR pos, FXMVECTOR target, FXMVECTOR worldUp);
	// ���ù۲����
	void SetView(const XMFLOAT3& pos, const XMFLOAT3& target, const XMFLOAT3& worldUp);

	// ��ȡ�۲����
	XMFLOAT4X4 GetView() const { return m_View; }
	// ��ȡͶӰ����
	XMFLOAT4X4 GetProj() const { return m_Proj; }

	// ��ȡ�۲����
	XMMATRIX GetXMView() const { return XMLoadFloat4x4(&m_View); }
	// ��ȡͶӰ����
	XMMATRIX GetXMProj() const { return XMLoadFloat4x4(&m_Proj); }
	// ��ȡ�۲�ͶӰ����
	XMMATRIX GetXMViewProj() const { return GetXMView() * GetXMProj(); }

	// ǰ(W)��(S)�ƶ�����ֵ��ǰ
	void MoveForward(float distance);
	// ��(A)��(D)ƽ�ƣ���ֵ����
	void MoveRight(float distance);
	// ��ֱ����ƽ�ƣ���ֵ����
	void MoveUp(float distance);
	// ��Ұ������ת(�������ƽ��)����ֵ����
	void LookRight(float radian);
	// ��Ұ������ת(�������ƽ��)����ֵ����
	void LookUp(float radian);

	// ���ù۲�Ŀ��(���������ڷ�FPS_ONLYģʽ)
	void SetTarget(const XMFLOAT3& target) { if (m_Mode != CameraMode::FPS_ONLY) m_Target = target; }
	// ��ҰΧ������������ת
	void LookAroundRight(float radian);
	// ��ҰΧ������������ת����ֵ��ʱ��
	void LookAroundUp(float radian);
	// ��������Զ
	void Zoom(float offset, float minDist, float maxDist);
	
	// ���¹۲����
	void UpdateViewMatrix();
	

private:
	// �����ģʽ
	CameraMode m_Mode;
	// ���������ϵ����������ϵ�Ĺ�ϵ
	XMFLOAT3 m_Position;
	XMFLOAT3 m_Right;
	XMFLOAT3 m_Up;
	XMFLOAT3 m_Look;
	
	// ����������˳�ģʽ����������ϵ��Ϣ
	float m_Theta;
	float m_Phi;
	float m_Radius;
	XMFLOAT3 m_Target;

	// ͶӰ�������
	float m_NearZ;
	float m_FarZ;
	float m_Aspect;
	float m_FovY;
	float m_NearWindowHeight;
	float m_FarWindowHeight;

	// �۲����/ͶӰ����
	XMFLOAT4X4 m_View;
	XMFLOAT4X4 m_Proj;

	// Effect ����
	ID3DX11Effect* m_pFX;
	ID3DX11EffectVectorVariable* m_pFXEyePosW;
	ID3DX11EffectMatrixVariable* m_pFXView;
	ID3DX11EffectMatrixVariable* m_pFXProj;
};

#endif