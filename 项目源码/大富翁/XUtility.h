
#pragma once

#include "d3dUtil.h"

#define BYTE_TO_UINT(a,b,c,d) (((unsigned)(a) << 24) | ((unsigned)(b) << 16) | ((unsigned)(c) << 8) | ((unsigned)(d)))
#define ReleaseStatic(Type) { Type::Release(); }

// 获取图片像素宽度和高度
void GetPictureInfo(LPCWSTR fileName, LONG * pOutWidth, LONG * pOutHeight);

// 生成Effect
void BuildFX(ID3D11Device* dev, LPCWSTR fileName, ID3DX11Effect** ppEffect);


