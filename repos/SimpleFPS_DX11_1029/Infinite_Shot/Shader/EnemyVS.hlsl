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

cbuffer CBufferAnimMatrix : register(b2)
{
	matrix gBoneAnimMat[50];
}

struct VS_INPUT
{
	float4 mPos : POSITION;
	float2 mTex : TEXCOORD0;
	float3 mNormal : NORMAL;
	float4 mBoneWeights : BLENDWEIGHT;
	int4 mBoneIndices : BLENDINDICES;
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

	float3 blendPos = 0;
	for (int i = 0; i < 4; ++i)
	{
		blendPos += Input.mBoneWeights[i] * mul(Input.mPos, gBoneAnimMat[Input.mBoneIndices[i]]);
	}

	Output.mPos = mul(float4(blendPos, 1.0f), gWorldMat);
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