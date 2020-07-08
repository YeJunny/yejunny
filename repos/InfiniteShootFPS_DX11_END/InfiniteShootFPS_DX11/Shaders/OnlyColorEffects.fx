struct VS_INPUT
{
    float4 mPos : POSITION;
    float3 mColor : COLOR;
};

struct PS_INPUT
{
    float4 mPos : SV_POSITION;
    float3 mColor : COLOR;
};

PS_INPUT VS(VS_INPUT Input)
{
    PS_INPUT Output;
    Output.mPos = Input.mPos;
    Output.mColor = Input.mColor;
    return Output;
}

float4 PS(PS_INPUT Input) : SV_TARGET
{
    return float4(Input.mColor, 1.0f);
}