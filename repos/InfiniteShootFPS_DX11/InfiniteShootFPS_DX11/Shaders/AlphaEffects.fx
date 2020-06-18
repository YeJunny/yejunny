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

cbuffer cbAlpha
{
    float mAlpha;
};

PS_INPUT VS(VS_INPUT Input)
{
    PS_INPUT Output;
    Output.mPos = Input.mPos;
    Output.mNormal = 0;
    Output.mTexCoord = Input.mTexCoord;
    return Output;
}

float4 PS(PS_INPUT Input) : SV_TARGET
{
    float4 diffuse = ObjTexture.Sample( ObjSamplerState, Input.mTexCoord );
    
    float alpha = mAlpha - diffuse.a;

    if (alpha > 1.0f)
    {
        alpha = 1.0f;
    }
    else if (alpha < 0.0f)
    {
        alpha = 0.0f;
    }

    return float4(diffuse.rgb, alpha);
}