cbuffer cbPerObject
{
	float4x4 WVP;
};

struct VS_OUTPUT
{
	float4 Pos : SV_POSITION;
	float2 TexCoord : TEXCOORD;
	float4 color : COLOR;
};

Texture2D shaderTexture;
SamplerState SampleType;

VS_OUTPUT VS(float4 inPos : POSITION, float2 inTexCoord : TEXCOORD, float4 color : COLOR)
{
	VS_OUTPUT output;

	inPos.w = 1.0f;

	output.Pos = mul(inPos, WVP);

	output.TexCoord = inTexCoord;

	output.color = color;

	return output;
}

float4 PS(VS_OUTPUT input) : SV_TARGET
{
	float4 textureColor;
	float4 finalColor;

	// Sample the pixel color from the texture using the sampler at this texture coordinate location.
	textureColor = shaderTexture.Sample(SampleType, input.TexCoord);

	// Combine the texture color and the particle color to get the final color result.
	finalColor = textureColor * input.color;

	return finalColor;
}