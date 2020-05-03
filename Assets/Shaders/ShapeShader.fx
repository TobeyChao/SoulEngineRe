cbuffer cbPerObject: register(b0)
{
    float4x4 WVP;
    float4x4 World;
};

struct Light
{
    float4 ambient;
    float4 diffuse;
	float4 specular;
	float3 dir;
	float pad;
};

cbuffer cbPerFrame: register(b1)
{
    Light light;
};

Texture2D ObjTexture;
Texture2D ObjNormMap;
SamplerState ObjSamplerState;

struct VS_OUTPUT
{
    float4 Pos : SV_POSITION;
	float4 worldPos : POSITION;
    float2 TexCoord : TEXCOORD;
    float3 normal : NORMAL;
	float3 tangent : TANGENT;
};

VS_OUTPUT VS(float4 inPos : POSITION, float2 inTexCoord : TEXCOORD, float3 normal : NORMAL, float3 tangent : TANGENT)
{
    VS_OUTPUT output;

    output.Pos = mul(inPos, WVP);

	output.worldPos = mul(inPos, World);

    output.normal = mul(normal, World);

	output.tangent = mul(tangent, World);

    output.TexCoord = inTexCoord;

    return output;
}

float4 PS(VS_OUTPUT input):SV_Target
{
    input.normal = normalize(input.normal);

    float4 diffuse = ObjTexture.Sample( ObjSamplerState, input.TexCoord );

	float4 normalMap = ObjNormMap.Sample(ObjSamplerState, input.TexCoord);

	float4 specular = ObjNormMap.Sample(ObjSamplerState, input.TexCoord);

	normalMap = (2.0f*normalMap) - 1.0f;

	input.tangent = normalize(input.tangent - dot(input.tangent, input.normal)*input.normal);

	float3 biTangent = cross(input.normal, input.tangent);

	float3x3 texSpace = float3x3(input.tangent, biTangent, input.normal);

	input.normal = normalize(mul(normalMap, texSpace));

    float3 finalColor;

    finalColor = diffuse * light.ambient;

    finalColor += saturate(dot(light.dir, input.normal) * light.diffuse * diffuse);
    
    return float4(finalColor, diffuse.a);
}