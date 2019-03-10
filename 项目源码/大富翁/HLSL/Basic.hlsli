#include "LightHelper.hlsl"


#define TECH(name) technique11 name

cbuffer cbPerFrame
{
    DirectionalLight gDirLight[16];
    PointLight gPointLight[16];
    SpotLight gSpotLight[16];
    int gDirNums;
    int gPointNums;
    int gSpotNums;
    float3 gEyePosW;
};

cbuffer cbPerObject
{
    float4x4 gWorld;             // 世界矩阵
    float4x4 gWorldInvTranspose; // 世界矩阵的逆转置
    float4x4 gView;              // 观察变换
    float4x4 gProj;              // 投影变换
    float4x4 gWorld2D;            // 对裁剪空间坐标点的变换矩阵
    float4x4 gTexTransform;      // 对纹理坐标点的世界变换矩阵
    Material gMaterial;          // 材质
};
Texture2D gTexture;

SamplerState samAnisotropic
{
    Filter = ANISOTROPIC;
    MaxAnisotropy = 4;

    AddressU = WRAP;
    AddressV = WRAP;
};



struct Vertex3DIn
{
    // 顶点部分
    float3 PosL : POSITION;
    float3 NormalL : NORMAL;
    float2 Tex : TEXCOORD;
    // 实例部分
    row_major float4x4 World : WORLD;
    row_major float4x4 WorldInvTranspose : WORLDINVT;
    uint InstanceId : SV_InstanceID;
};

struct Vertex3DOut
{
    float4 PosH : SV_POSITION;
    float3 PosW : POSITION;
    float3 NormalW : NORMAL;
    float2 Tex : TEXCOORD;
};

struct Vertex2DIn
{
    float2 PosH : POSITION;
    float2 Tex : TEXCOORD;
};

struct Vertex2DOut
{
    float4 PosH : SV_POSITION;
    float2 Tex : TEXCOORD;
};