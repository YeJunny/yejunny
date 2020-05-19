struct VS_INPUT
{
    float4 mPos : POSITION;
    float2 mTexCoord : TEXCOORD;
};

Texture2D ObjTexture;
SamplerState ObjSamplerState;

struct PS_INPUT
{
    float4 mPos : SV_POSITION;
    float2 mTexCoord : TEXCOORD;
};

cbuffer cbPerObject
{
    float4x4 mWVP;
};

PS_INPUT VS(VS_INPUT Input)
{
    PS_INPUT Output;
    Output.mPos = mul(Input.mPos, mWVP);
    Output.mTexCoord = Input.mTexCoord;
    return Output;
}

float4 PS(PS_INPUT Input) : SV_TARGET
{
    //return float4(1.0f, 1.0f, 1.0f, 1.0f);
    return ObjTexture.Sample(ObjSamplerState, Input.mTexCoord);
}