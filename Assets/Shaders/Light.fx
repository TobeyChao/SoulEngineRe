#include "LightHelper.fx"
#include "TangentHelper.fx"

cbuffer CBChangesEveryDrawing : register(b0)
{
	matrix gWorld;
	matrix gView;
	matrix gProj;
	Material gMaterial;
};

cbuffer cbPerFrame : register(b1)
{
	float3 gEyePosW;
};

cbuffer CBChangesRarely : register(b2)
{
	DirectionalLight gDirLight[3];
	PointLight gPointLight[3];
	SpotLight gSpotLight[3];
    int gNumDirLight;
	int gNumPointLight;
	int gNumSpotLight;
	float pad1 = 0;
	matrix gShadow;
	bool gIsShadow;
	bool gUseTexture;
	bool gUseNormal;
	float pad2 = 0;
	float4 gShadowplane;
}

cbuffer CBRenderStates : register(b3)
{
	float4 gFogColor;
	bool gFogEnabled;
	float gFogStart;
	float gFogRange;
}

Texture2D gTex : register(t0);
Texture2D gNormalMap : register(t1);
SamplerState gSamLinear : register(s0);

struct VertexIn
{
	float3 PosL : POSITION;
    float3 NormalL : NORMAL;
	float3 TangentL : TANGENT;
	float2 Tex : TEXCOORD;
	float4 Color : COLOR;
};

struct VertexOut
{
	float4 PosH : SV_POSITION;
    float3 PosW : POSITION;     // 在世界中的位置
    float3 NormalW : NORMAL;    // 法向量在世界中的方向
	float3 TangentW : TANGENT;
	float4 Color : COLOR;
	float2 Tex : TEXCOORD;
};

VertexOut VS(VertexIn vertIn)
{
    VertexOut vertOut;
	float4 posw = mul(float4(vertIn.PosL, 1.0f), gWorld);
	float t = 1;
	if(gIsShadow)
	{
		t = dot(posw, gShadowplane);
		posw = mul(posw, gShadow);
	}
	float4x4 viewProj = mul(gView, gProj);
	vertOut.PosH = mul(posw, viewProj);
	if(t <= 0.0f)
	{
		vertOut.PosH.w = 0;
	}
	vertOut.PosW = posw.xyz;
	vertOut.NormalW = mul(vertIn.NormalL, (float3x3)gWorld);
	vertOut.TangentW = mul(float4(vertIn.TangentL, 1.0f), gWorld).xyz;
    vertOut.Color = vertIn.Color;
	vertOut.Tex = vertIn.Tex;
    return vertOut;
}

float4 PS(VertexOut vertIn) : SV_Target
{
    float4 originColor = float4(1.0f, 1.0f, 1.0f, 1.0f);
	if(gUseTexture)
	{
		originColor = gTex.Sample(gSamLinear, vertIn.Tex);
		clip(originColor.a - 0.1f);
	}

    // 归一化法线
    vertIn.NormalW = normalize(vertIn.NormalW); 
    // 物体顶点到摄像机眼睛，用来计算镜面高光
	float3 toEyeW = normalize(gEyePosW - vertIn.PosW);
	float distToEye = distance(gEyePosW, vertIn.PosW);
	float3 NormalW = vertIn.NormalW;

	// 如果使用法线贴图更新法线值
	if(gUseNormal)
	{
		float3 normalMapSample = gNormalMap.Sample(gSamLinear, vertIn.Tex).rgb;
    	NormalW = NormalSampleToWorldSpace(normalMapSample, vertIn.NormalW, vertIn.TangentW);
	}
    
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
		ComputeDirectionalLight(gMaterial, gDirLight[i], NormalW, toEyeW, A, D, S);
		ambient += A;  
		diffuse += D;
		spec += S;
	}

	[unroll]
    for (i = 0; i < gNumPointLight; ++i)
    {
		ComputePointLight(gMaterial, gPointLight[i], vertIn.PosW, NormalW, toEyeW, A, D, S);
		ambient += A;
		diffuse += D;
		spec += S;
	}

	[unroll]
    for (i = 0; i < gNumSpotLight; ++i)
    {
		ComputeSpotLight(gMaterial, gSpotLight[i], vertIn.PosW, NormalW, toEyeW, A, D, S);
		ambient += A;
		diffuse += D;
		spec += S;
	}

	float4 finalColor = originColor * (ambient + diffuse) + spec;

	// 雾效部分
    [flatten]
    if (gFogEnabled)
    {
        // 限定在0.0f到1.0f范围
        float fogLerp = saturate((distToEye - gFogStart) / gFogRange);
        // 根据雾色和光照颜色进行线性插值
        finalColor = lerp(finalColor, gFogColor, fogLerp);
    }

	// Common to take alpha from diffuse material.
	finalColor.a = originColor.a * gMaterial.Diffuse.a;
    return finalColor;
}