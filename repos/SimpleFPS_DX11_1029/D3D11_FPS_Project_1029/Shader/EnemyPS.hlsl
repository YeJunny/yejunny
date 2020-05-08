Texture2D Texture : register(t0);
SamplerState Sampler : register(s0);

struct PS_INPUT
{
	float4 mPos : SV_POSITION;
	float2 mTex : TEXCOORD0;
	float3 mDiffuse : TEXCOORD1;
	float3 mViewDirection : TEXCOORD2;
	float3 mReflection : TEXCOORD3;
};

float4 PS(PS_INPUT Input) : SV_Target
{
	float3 diffuse = saturate(Input.mDiffuse);

	float3 reflection = normalize(Input.mReflection);
	float3 viewDirection = normalize(Input.mViewDirection);
	float3 specular = 0;
	if (diffuse.x > 0)
	{
		specular = saturate(dot(reflection, -viewDirection));
		specular = pow(specular, 20.0f);
	}

	float3 ambient = float3(0.2f, 0.2f, 0.2f);

	float3 texColor = Texture.Sample(Sampler, Input.mTex);

	float3 outColor = (ambient + diffuse + specular) * texColor;

	return float4(outColor, 1.0f);
}