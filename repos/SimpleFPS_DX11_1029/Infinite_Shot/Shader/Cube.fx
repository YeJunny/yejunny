cbuffer ConstantBuffer : register(b0)
{
	matrix World;
	matrix View;
	matrix Projection;
}

struct VS_INPUT
{
	float4 mPos : POSITION;
	float4 mColor : COLOR;
};

struct PS_INPUT
{
	float4 mPos : SV_POSITION;
	float4 mColor : COLOR;
};


PS_INPUT VS(VS_INPUT input)
{
	PS_INPUT output;
	output.mPos = mul(input.mPos, World);
	output.mPos = mul(output.mPos, View);
	output.mPos = mul(output.mPos, Projection);
	output.mColor = input.mColor;

	return output;
}


float4 PS(PS_INPUT input) : SV_Target
{
	return input.mColor;
}