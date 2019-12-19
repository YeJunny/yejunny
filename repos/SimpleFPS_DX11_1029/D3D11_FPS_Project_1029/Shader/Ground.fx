Texture2D Texture : register(t0);
SamplerState Sampler : register(s0);

cbuffer ConstantBuffer : register(b0)
{
	matrix WorldMat;
	matrix ViewMat;
	matrix ProjectionMat;
    float3 LightingPosition;
}

struct VS_INPUT
{
	float4 mPos : POSITION;
	float2 mTex : TEXCOORD;
	float3 mNormal : NORMAL;
};

struct PS_INPUT
{
	float4 mPos : SV_POSITION;
	float2 mTex : TEXCOORD0;
    float3 mDiffuse : TEXCOORD1;
};


PS_INPUT VS(VS_INPUT input)
{
	PS_INPUT output;

	output.mPos = mul(input.mPos, WorldMat);

    float3 lightDir = output.mPos.xyz - LightingPosition.xyz;
    lightDir = normalize(lightDir);


	output.mPos = mul( output.mPos, ViewMat );
	output.mPos = mul( output.mPos, ProjectionMat );

    float3 worldNormal = mul( input.mNormal, (float3x3)WorldMat );
    worldNormal = normalize( worldNormal );

    output.mDiffuse = dot(-lightDir, worldNormal);
	output.mTex = input.mTex;

	return output;
}


float4 PS(PS_INPUT input) : SV_Target
{
    float3 sampleColor = Texture.Sample(Sampler, input.mTex);
    float3 diffuse = sampleColor * saturate(input.mDiffuse);
	return float4(diffuse, 1.0f);
}