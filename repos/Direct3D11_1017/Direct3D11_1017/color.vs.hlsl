cbuffer MatrixBuffer
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};

struct VertexInputType
{
	float4 position : POSITION;
	float4 color : COLOR;
};

struct PixelInputType
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
};

PixelInputType ColorVertexShader(VertexInputType Input)
{
	PixelInputType Output;

	Input.position.w = 1.0f;

	Output.position = mul(Input.position, worldMatrix);
	Output.position = mul(Output.position, viewMatrix);
	Output.position = mul(Output.position, projectionMatrix);

	Output.color = Input.color;

	return Output;
}