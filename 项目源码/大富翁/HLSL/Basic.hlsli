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
    float4x4 gWorld;             // �������
    float4x4 gWorldInvTranspose; // ����������ת��
    float4x4 gView;              // �۲�任
    float4x4 gProj;              // ͶӰ�任
    float4x4 gWorld2D;            // �Բü��ռ������ı任����
    float4x4 gTexTransform;      // ����������������任����
    Material gMaterial;          // ����
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
    // ���㲿��
    float3 PosL : POSITION;
    float3 NormalL : NORMAL;
    float2 Tex : TEXCOORD;
    // ʵ������
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