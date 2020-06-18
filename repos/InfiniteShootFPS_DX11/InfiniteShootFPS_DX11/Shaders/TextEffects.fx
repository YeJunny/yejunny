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

cbuffer cbPerObject
{
    float4x4 mWVP;
    float4x4 mWorld;
};

PS_INPUT D2D_VS(VS_INPUT Input)
{
    PS_INPUT Output;
    Output.mPos = mul(Input.mPos, mWVP);
    Output.mTexCoord = Input.mTexCoord;
    return Output;
}

float4 D2D_PS(PS_INPUT Input) : SV_TARGET
{
    float4 diffuse = ObjTexture.Sample(ObjSamplerState, Input.mTexCoord);
    return diffuse;
}