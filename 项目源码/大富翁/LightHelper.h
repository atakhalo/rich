//***************************************************************************************
// LightHelper.h by Frank Luna (C) 2011 All Rights Reserved.
//
// Helper classes for lighting.
//***************************************************************************************

#ifndef LIGHTHELPER_H
#define LIGHTHELPER_H

#include <Windows.h>
#include <xnamath.h>

// Note: Make sure structure alignment agrees with HLSL structure padding rules. 
//   Elements are packed into 4D vectors with the restriction that an element
//   cannot straddle a 4D vector boundary.

struct DirectionalLight
{
	DirectionalLight() { ZeroMemory(this, sizeof(*this)); }

	XMFLOAT4 Ambient;	// �����ⷴ��
	XMFLOAT4 Diffuse;	// ������
	XMFLOAT4 Specular;	// ���淴��
	XMFLOAT3 Direction;	// λ��
	int	State;			// ����״̬
};

struct PointLight
{
	PointLight() { ZeroMemory(this, sizeof(*this)); }

	XMFLOAT4 Ambient;	// �����ⷴ��
	XMFLOAT4 Diffuse;	// ������
	XMFLOAT4 Specular;	// ���淴��

	// �����4D����: (Position, Range)
	XMFLOAT3 Position;	// λ��
	float Range;		// ����������

	// �����4D����: (A0, A1, A2, Pad)
	XMFLOAT3 Att;		// ˥��ϵ��
	int	State;			// ����״̬
};

struct SpotLight
{
	SpotLight() { ZeroMemory(this, sizeof(*this)); }

	XMFLOAT4 Ambient;	// �����ⷴ��
	XMFLOAT4 Diffuse;	// ������
	XMFLOAT4 Specular;	// ���淴��

	// �����4D����: (Position, Range)
	XMFLOAT3 Position;	// λ��
	float Range;		// ����������

	// �����4D����: (Direction, Spot)
	XMFLOAT3 Direction;	// ����
	float Spot;			// ��۳̶�

	// �����4D����: (Att, Pad)
	XMFLOAT3 Att;		// ˥��ϵ��
	int	State;			// ����״̬
};

struct Material
{
	Material() { ZeroMemory(this, sizeof(*this)); }

	XMFLOAT4 Ambient;
	XMFLOAT4 Diffuse;
	XMFLOAT4 Specular; // w = SpecPower
	XMFLOAT4 Reflect;
};

#endif // LIGHTHELPER_H