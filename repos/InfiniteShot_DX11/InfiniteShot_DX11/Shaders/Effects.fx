struct Light
{
    float3 mDir;
    float4 mAmbient;
    float4 mDiffuse;
};

struct VS_INPUT
{
    float4 mPos : POSITION;
    float2 mTexCoord : TEXCOORD;
    float3 mNormal : NORMAL;
};

Texture2D ObjTexture;
SamplerState ObjSamplerState;

struct PS_INPUT
{
    float4 mPos : SV_POSITION;
    float2 mTexCoord : TEXCOORD;
    float3 mNormal : NORMAL;
};

cbuffer cbPerFrame
{
    Light mLight;
    float4x4 mRotation;
};

cbuffer cbPerObject
{
    float4x4 mWVP;
    float4x4 mWorld;
};

PS_INPUT VS(VS_INPUT Input)
{
    PS_INPUT Output;
    Output.mPos = mul(Input.mPos, mWVP);
    Output.mNormal = mul(Input.mNormal, mWorld);
    Output.mTexCoord = Input.mTexCoord;
    return Output;
}

float4 PS(PS_INPUT Input) : SV_TARGET
{
    Input.mNormal = normalize(Input.mNormal);

    float4 diffuse = ObjTexture.Sample(ObjSamplerState, Input.mTexCoord);

    float3 finalColor = diffuse * mLight.mAmbient;
    finalColor += saturate(dot(mul(mLight.mDir, mRotation), Input.mNormal) * mLight.mDiffuse * diffuse);

    return float4(finalColor, diffuse.a);
}

float4 D2D_PS(PS_INPUT Input) : SV_TARGET
{
    float4 diffuse = ObjTexture.Sample(ObjSamplerState, Input.mTexCoord);
    return diffuse;
}