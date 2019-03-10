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

	XMFLOAT4 Ambient;	// 环境光反射
	XMFLOAT4 Diffuse;	// 漫反射
	XMFLOAT4 Specular;	// 镜面反射
	XMFLOAT3 Direction;	// 位置
	int	State;			// 开关状态
};

struct PointLight
{
	PointLight() { ZeroMemory(this, sizeof(*this)); }

	XMFLOAT4 Ambient;	// 环境光反射
	XMFLOAT4 Diffuse;	// 漫反射
	XMFLOAT4 Specular;	// 镜面反射

	// 打包成4D向量: (Position, Range)
	XMFLOAT3 Position;	// 位置
	float Range;		// 最大照射距离

	// 打包成4D向量: (A0, A1, A2, Pad)
	XMFLOAT3 Att;		// 衰弱系数
	int	State;			// 开关状态
};

struct SpotLight
{
	SpotLight() { ZeroMemory(this, sizeof(*this)); }

	XMFLOAT4 Ambient;	// 环境光反射
	XMFLOAT4 Diffuse;	// 漫反射
	XMFLOAT4 Specular;	// 镜面反射

	// 打包成4D向量: (Position, Range)
	XMFLOAT3 Position;	// 位置
	float Range;		// 最大照射距离

	// 打包成4D向量: (Direction, Spot)
	XMFLOAT3 Direction;	// 方向
	float Spot;			// 汇聚程度

	// 打包成4D向量: (Att, Pad)
	XMFLOAT3 Att;		// 衰弱系数
	int	State;			// 开关状态
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