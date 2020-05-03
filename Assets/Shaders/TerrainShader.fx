
struct Light
{
	float3 dir;
	float pad;
	float4 ambient;
	float4 diffuse;
};

cbuffer cbPerFrame
{
	Light light;
};

cbuffer cbPerObject
{
	float4x4 WVP;
    float4x4 World;
};

Texture2D shaderTexture : register(t0);
Texture2D normalTexture : register(t1);
SamplerState ObjSamplerState;

struct VS_OUTPUT
{
	float4 Pos : SV_POSITION;
	float2 TexCoord : TEXCOORD;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float3 binormal : BINORMAL;
	float4 color : COLOR;
};

VS_OUTPUT VS(float4 inPos : POSITION, float2 inTexCoord : TEXCOORD, float3 normal : NORMAL, float3 color : COLOR, float3 tangent : TANGENT, float3 binormal : BINORMAL)
{
    VS_OUTPUT output;

    output.Pos = mul(inPos, WVP);

	output.normal = mul(normal, World);

    output.TexCoord = inTexCoord;

	output.tangent = mul(tangent, World);
	output.tangent = normalize(output.tangent);

	output.binormal = mul(binormal, World);
	output.binormal = normalize(output.binormal);

	output.color = float4(color, 1.0f);

    return output;
}

float4 PS(VS_OUTPUT input) : SV_TARGET
{
	float4 textureColor;
	float3 lightDir;
	float4 bumpMap;
	float3 bumpNormal;
	float lightIntensity;
	float4 color;

	input.normal = normalize(input.normal);

	textureColor = shaderTexture.Sample( ObjSamplerState, input.TexCoord );

	// Combine the color map value into the texture color.
	textureColor = saturate(textureColor * input.color * 2.0f);

	lightDir = -light.dir;

	lightIntensity = saturate(dot(input.normal, lightDir));

	// Calculate the amount of light on this pixel using the normal map.
	bumpMap = normalTexture.Sample(ObjSamplerState, input.TexCoord);
	bumpMap = (bumpMap * 2.0f) - 1.0f;
	bumpNormal = (bumpMap.x * input.tangent) + (bumpMap.y * input.binormal) + (bumpMap.z * input.normal);
	bumpNormal = normalize(bumpNormal);

	lightIntensity = saturate(dot(bumpNormal, lightDir));

	color = textureColor * light.ambient;

	color += saturate(lightIntensity * light.diffuse);

	color = color * textureColor;

	return color;
}