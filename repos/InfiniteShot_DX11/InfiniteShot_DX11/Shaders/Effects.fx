struct Light
{
    float3 mDir;
    float3 mPos;
    float mRange;
    float3 mAtt;
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
    float4 mWorldPos : POSITION;
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
    Output.mWorldPos = mul(Input.mPos, mWorld);
    Output.mNormal = mul(Input.mNormal, mWorld);
    Output.mTexCoord = Input.mTexCoord;
    return Output;
}

float4 PS(PS_INPUT Input) : SV_TARGET
{
    Input.mNormal = normalize(Input.mNormal);

    float4 diffuse = ObjTexture.Sample(ObjSamplerState, Input.mTexCoord);

    float3 finalColor = float3(0.0f, 0.0f, 0.0f);

    float3 lightToPixel = mLight.mPos - Input.mWorldPos;

    float d = length(lightToPixel);

    float3 finalAmbient = diffuse * mLight.mAmbient;

    if (d > mLight.mRange)
    {
        return float4(finalAmbient, diffuse.a);
    }

    lightToPixel /= d;

    float lightIndencity = dot(lightToPixel, Input.mNormal);

    if (lightIndencity > 0.0f)
    {
        finalColor += lightIndencity * diffuse * mLight.mDiffuse;

        finalColor /= mLight.mAtt[0] + (mLight.mAtt[1] * d) + (mLight.mAtt[2] * d * d);
    }

    finalColor = saturate(finalColor + finalAmbient);

    return float4(finalColor, diffuse.a);
}

float4 D2D_PS(PS_INPUT Input) : SV_TARGET
{
    float4 diffuse = ObjTexture.Sample(ObjSamplerState, Input.mTexCoord);
    return diffuse;
}