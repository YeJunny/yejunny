struct VS_INPUT
{
    float3 Pos : POSITION;
    float2 TexCoord : TEXCOORD;
    float3 Normal : NORMAL;
};

struct SKYMAP_VS_OUPUT
{
    float4 Pos : SV_POSITION;
    float3 TexCoord : TEXCOORD;
};

cbuffer cbPerObject
{
    float4x4 WVP;
    float4x4 World;
};

TextureCube Skymap;
SamplerState ObjSamplerState;

SKYMAP_VS_OUPUT SKYMAP_VS(VS_INPUT Input)
{
    SKYMAP_VS_OUPUT Output = (SKYMAP_VS_OUPUT)0;

    Output.Pos = mul(float4(Input.Pos, 1.0f), WVP).xyww;
    Output.TexCoord = Input.Pos;

    return Output;
}

float4 SKYMAP_PS(SKYMAP_VS_OUPUT Input) : SV_TARGET
{
    return Skymap.Sample(ObjSamplerState, Input.TexCoord);
}