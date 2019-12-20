Texture2D Texture : TEXTURE: register(t0);
SamplerState Sampler : SAMPLER: register(s0);

cbuffer ConstantBuffer : register(b0)
{
	matrix World;
	matrix View;
	matrix Projection;
}

struct VS_INPUT
{
	float4 mPos : POSITION;
	float2 mTex : TEXCOORD;
};

struct PS_INPUT
{
	float4 mPos : SV_POSITION;
	float2 mTex : TEXCOORD;
};


PS_INPUT VS(VS_INPUT input)
{
	PS_INPUT output;
	output.mPos = mul(input.mPos, World);
	output.mPos = mul(output.mPos, View);
	output.mPos = mul(output.mPos, Projection);
	output.mTex = input.mTex;

	return output;
}


float4 PS(PS_INPUT input) : SV_Target
{
	return float4(Texture.Sample(Sampler, input.mTex).rgb, 1.0f);
}