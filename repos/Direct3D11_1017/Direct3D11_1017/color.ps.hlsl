struct PixelInputType
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
};

float4 ColorPixelShader(PixelInputType Input) : SV_TARGET
{
	return Input.color;
}