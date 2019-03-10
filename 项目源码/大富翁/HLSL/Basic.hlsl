//=============================================================================
// Lighting.fx by Frank Luna (C) 2011 All Rights Reserved.
//
// Transforms and lights geometry.
//=============================================================================

#include "Basic.hlsli"



Vertex3DOut VS_3D(Vertex3DIn vin)
{
	Vertex3DOut vout;

    // ����۲�ͶӰ�任
    float4x4 gWorldViewProj = mul(mul(vin.World, gView), gProj);
	// ת������������ϵ
	vout.PosW = mul(float4(vin.PosL, 1.0f), vin.World).xyz;
	vout.NormalW = mul(vin.NormalL, (float3x3)vin.WorldInvTranspose);

	// ת�����ü��ռ�
	vout.PosH = mul(float4(vin.PosL, 1.0f), gWorldViewProj);
    vout.Tex = vin.Tex;

	return vout;
}

float4 PS_3D(Vertex3DOut pin) : SV_Target
{
	// ��׼��������
	pin.NormalW = normalize(pin.NormalW);

    // ���㵽�۾�������
	float3 toEyeW = normalize(gEyePosW - pin.PosW);

    // �������
    float4 texColor = gTexture.Sample(samAnisotropic, pin.Tex);
    // ����alphaֵ����0.1�����ص㣬���������ơ�
    clip(texColor.a - 0.1f);

	// ��ʼ��Ϊ0 
	float4 ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 spec = float4(0.0f, 0.0f, 0.0f, 0.0f);

	// ���㻷���⡢������⡢�����Ĺ��׳̶ȡ�
	float4 A, D, S;
	
    int i;
    for (i = 0; i < gDirNums; ++i)
    {
        
        if (gDirLight[i].State)
        {
            ComputeDirectionalLight(gMaterial, gDirLight[i], pin.NormalW, toEyeW, A, D, S);
            ambient += A;
            diffuse += D;
            spec += S;
        }
    }

    for (i = 0; i < gPointNums; ++i)
    {
        if (gPointLight[i].State)
        {
            ComputePointLight(gMaterial, gPointLight[i], pin.PosW, pin.NormalW, toEyeW, A, D, S);
            ambient += A;
            diffuse += D;
            spec += S;
        }
    }

    for (i = 0; i < gSpotNums; ++i)
     {
        if (gSpotLight[i].State)
        {
            ComputeSpotLight(gMaterial, gSpotLight[i], pin.PosW, pin.NormalW, toEyeW, A, D, S);
            ambient += A;
            diffuse += D;
            spec += S;
        }
    }

	float4 litColor = texColor * (ambient + diffuse) + spec;
	
	// Common to take alpha from diffuse material and texture.
	litColor.a = gMaterial.Diffuse.a * texColor.a;
	
	return litColor;
}


TECH(Sprite3D)
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VS_3D()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PS_3D()));
	}
}
