Texture2D texture0;
SamplerState sampler0;
TextureCube skybox;

cbuffer CB0
{
	matrix WorldViewProjection; // 64 bytes

}; // total = 64 bytes


struct VOut
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
	float2 texcoord : TEXCOORD;
};

VOut VShader(float4 position: POSITION, float2 texcoord : TEXCOORD, float3 normal : NORMAL)
{
	VOut output;

	float4 default_color = { 1.0,1.0,1.0,1.0 };
	output.position = mul(WorldViewProjection, position);
	output.texcoord = texcoord;
	output.color = default_color;

	return output;
}

float4 PShader(float4 position : SV_POSITION, float4 color : COLOR, float2 texcoord : TEXCOORD) : SV_TARGET
{
	return color * texture0.Sample(sampler0,texcoord);
}