Texture2D texture0;
Texture2D normal;
SamplerState sampler0;

cbuffer CB0
{
	matrix WVPMatrix; // 64 bytes
	float TotalTime; // 4 bytes
	float3 packing; // 12 bytes
}; // total = 80 bytes


struct VOut
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
	float2 texcoord : TEXCOORD;
	float3 normal: NORMAL;
	float3 binormal: BINORMAL;
	float3 tangent: TANGENT;
};

struct VIn
{
	float4 position: POSITION;
	float2 texcoord : TEXCOORD;
	float3 normal : NORMAL;
	float4 color: COLOR;
};


VOut VShader(VIn vin)
{
	VOut output;

	//float4 default_color = { 1.0,1.0,1.0,1.0 };
	output.position = mul(WVPMatrix, vin.position);
	output.position.y  = vin.position.y * 2;
	output.texcoord = vin.texcoord;
	//output.texcoord.x += 0.1f;
	output.color = vin.color;

	return output;
}

float4 PShader(VOut pin) : SV_TARGET
{
	return pin.color * texture0.Sample(sampler0,pin.texcoord);
}