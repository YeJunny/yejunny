Texture2D Texture : register(t0);
SamplerState Sampler : register(s0);

cbuffer CBufferMatrix : register(b0)
{
	matrix gWorldMat;
	matrix gViewMat;
	matrix gProjectionMat;
}

cbuffer CBufferLight : register(b1)
{
	float4 gWorldLightPos;
	float4 gWorldCameraPos;
}

struct VS_INPUT
{
	float4 mPos : POSITION;
	float2 mTex : TEXCOORD0;
	float3 mNormal : NORMAL;
};

struct PS_INPUT
{
	float4 mPos : SV_POSITION;
	float2 mTex : TEXCOORD0;
	float3 mDiffuse : TEXCOORD1;
	float3 mViewDirection : TEXCOORD2;
	float3 mReflection : TEXCOORD3;
};

PS_INPUT VS(VS_INPUT Input)
{
	PS_INPUT Output;

	Output.mPos = mul(Input.mPos, gWorldMat);
	float3 lightDirection = normalize(Output.mPos - gWorldLightPos);
	Output.mViewDirection = normalize(Output.mPos - gWorldCameraPos);

	float3 worldNormal = normalize(mul(Input.mNormal, (float3x3)gWorldMat));
	Output.mDiffuse = dot(-lightDirection, worldNormal);

	Output.mPos = mul(Output.mPos, gViewMat);
	Output.mPos = mul(Output.mPos, gProjectionMat);

	Output.mTex = Input.mTex;

	Output.mReflection = reflect(lightDirection, worldNormal);

	return Output;
}

float4 PS(PS_INPUT Input) : SV_Target
{
	float3 diffuse = saturate(Input.mDiffuse);

	float3 reflection = normalize(Input.mReflection);
	float3 viewDirection = normalize(Input.mViewDirection);
	float3 specular = 0;
	if (diffuse.x > 0)
	{
		specular = saturate( dot(reflection, -viewDirection) );
		specular = pow(specular, 20.0f);
	}

	float3 ambient = float3(0.2f, 0.2f, 0.2f);

	float3 texColor = Texture.Sample(Sampler, Input.mTex);

	float3 outColor = (ambient + diffuse + specular) * texColor;

	return float4(outColor, 1.0f);
}