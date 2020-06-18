struct Light // PS
{
    float3 mDir;
    float4 mAmbient;
    float4 mDiffuse;
};

struct VS_INPUT
{
    float3 mPos : POSITION;
    float2 mTexCoord : TEXCOORD;
    float3 mNormal : NORMAL;
    float4 mWeight : BLENDWEIGHT;
    int4 mIndices : BLENDINDICES;
};

Texture2D ObjTexture;
SamplerState ObjSamplerState;

struct PS_INPUT
{
    float4 mPos : SV_POSITION;
    float2 mTexCoord : TEXCOORD;
    float3 mNormal : NORMAL;
};

cbuffer cbPerFrame // PS
{
    Light gLight;
};

cbuffer cbPerObject : register(b0) // VS
{
    float4x4 gWVP;
    float4x4 gWorld;
};

cbuffer CBAnimMat : register(b1) // VS
{
    float4x4 gAnimMat[50];
}

PS_INPUT VS(VS_INPUT Input)
{
    PS_INPUT Output;

    float3 blendPos = 0;
    for (int num = 0; num < 4; ++num)
    {
        blendPos += Input.mWeight[num] * mul(Input.mPos, gAnimMat[Input.mIndices[num]]);
    }

    Output.mPos = mul(float4(blendPos, 1.0f), gWVP);
    Output.mNormal = mul(Input.mNormal, gWorld);
    Output.mTexCoord = Input.mTexCoord;
    return Output;
}

float4 PS(PS_INPUT Input) : SV_TARGET
{
    Input.mNormal = normalize(Input.mNormal);

    float4 diffuse = ObjTexture.Sample( ObjSamplerState, Input.mTexCoord );

    float3 finalColor;

    finalColor = diffuse * gLight.mAmbient;
    finalColor += saturate(dot(gLight.mDir, Input.mNormal) * gLight.mDiffuse * diffuse);
    
    return diffuse;//float4(finalColor, diffuse.a);
}