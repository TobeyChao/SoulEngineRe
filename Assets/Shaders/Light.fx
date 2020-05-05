#include "LightHelper.fx"

cbuffer cbPerFrame : register(b0)
{
	DirectionalLight gDirLight[3];
	PointLight gPointLight[3];
	SpotLight gSpotLight[3];
    int gNumDirLight;
	int gNumPointLight;
	int gNumSpotLight;
    bool gUseTexture;
	float3 gEyePosW;
};

cbuffer cbPerObject : register(b1)
{
	float4x4 gWorld;
	float4x4 gWorldViewProj;
	Material gMaterial;
};

Texture2D gTex : register(t0);
SamplerState gSamLinear : register(s0);

struct VertexIn
{
	float3 PosL : POSITION;
	float2 Tex : TEXCOORD;
    float3 NormalL : NORMAL;
	float4 Color : COLOR;
};

struct VertexOut
{
	float4 PosH : SV_POSITION;
    float3 PosW : POSITION;     // 在世界中的位置
    float3 NormalW : NORMAL;    // 法向量在世界中的方向
	float4 Color : COLOR;
	float2 Tex : TEXCOORD;
};

VertexOut VS(VertexIn vertIn)
{
    VertexOut vertOut;
    vertOut.PosW = mul(float4(vertIn.PosL, 1.0f), gWorld).xyz;
	vertOut.PosH = mul(float4(vertIn.PosL, 1.0f), gWorldViewProj);
	vertOut.NormalW = mul(vertIn.NormalL, (float3x3)gWorld);
    vertOut.Color = vertIn.Color;
	vertOut.Tex = vertIn.Tex;
    return vertOut;
}

float4 PS(VertexOut vertIn) : SV_Target
{
    float4 originColor = float4(1.0f, 1.0f, 1.0f, 1.0f);
	if(gUseTexture == true)
	{
		originColor = gTex.Sample(gSamLinear, vertIn.Tex);
		clip(originColor.a - 0.1f);
	}

    // 归一化法线
    vertIn.NormalW = normalize(vertIn.NormalW); 
    // 物体顶点到摄像机眼睛，用来计算镜面高光
	float3 toEyeW = normalize(gEyePosW - vertIn.PosW);

	// Start with a sum of zero. 
	float4 ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 spec    = float4(0.0f, 0.0f, 0.0f, 0.0f);

	// Sum the light contribution from each light source.
	float4 A, D, S;
	int i;
	[unroll]
    for (i = 0; i < gNumDirLight; ++i)
    {
		ComputeDirectionalLight(gMaterial, gDirLight[i], vertIn.NormalW, toEyeW, A, D, S);
		ambient += A;  
		diffuse += D;
		spec += S;
	}

	[unroll]
    for (i = 0; i < gNumPointLight; ++i)
    {
		ComputePointLight(gMaterial, gPointLight[i], vertIn.PosW, vertIn.NormalW, toEyeW, A, D, S);
		ambient += A;
		diffuse += D;
		spec += S;
	}

	[unroll]
    for (i = 0; i < gNumSpotLight; ++i)
    {
		ComputeSpotLight(gMaterial, gSpotLight[i], vertIn.PosW, vertIn.NormalW, toEyeW, A, D, S);
		ambient += A;
		diffuse += D;
		spec += S;
	}

	float4 finalColor = originColor * (ambient + diffuse) + spec;

	// Common to take alpha from diffuse material.
	finalColor.a = originColor.a * gMaterial.Diffuse.a;
    return finalColor;
}