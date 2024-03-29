cbuffer cbPerObject
{
	float4x4 WVP;
};

//Texture2D ObjTexture;
SamplerState ObjSamplerState : register(s0);
TextureCube Sky : register(t0);

struct VS_OUTPUT
{
	float4 Pos : SV_POSITION;
	float3 texCoord : TEXCOORD;
};

VS_OUTPUT VS(float3 inPos : POSITION)
{
	VS_OUTPUT output = (VS_OUTPUT)0;

	output.Pos = mul(float4(inPos, 1.0f), WVP).xyww;

	output.texCoord = inPos;

	return output;
}

float4 PS(VS_OUTPUT input) :SV_Target
{
	return Sky.Sample(ObjSamplerState,input.texCoord);
}