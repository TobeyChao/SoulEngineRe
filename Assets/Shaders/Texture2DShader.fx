cbuffer cbPerObject
{
	float4x4 WVP;
};

Texture2D ObjTexture;
SamplerState ObjSamplerState;

struct VertexIn
{
	float3 inPos : POSITION;
	float2 inTexCoord : TEXCOORD;
};

struct VertexOut
{
	float4 Pos : SV_POSITION;
	float2 TexCoord : TEXCOORD;
};

VertexOut VS(VertexIn vertIn)
{
	VertexOut output;

	output.Pos = mul(float4(vertIn.inPos, 1.0), WVP);
	output.TexCoord = vertIn.inTexCoord;

	return output;
}

float4 PS(VertexOut input) : SV_TARGET
{
	float4 texColor = ObjTexture.Sample(ObjSamplerState, input.TexCoord);
	
	return texColor;
}