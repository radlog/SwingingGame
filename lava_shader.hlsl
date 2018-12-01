Texture2D texture0;
Texture2D normal;
SamplerState sampler0;

cbuffer CB0
{
	matrix WVPMatrix; // 64 bytes
    float4 directional_light; // 16 bytes
    float4 directional_light_colour; // 16 bytes
    float4 ambient_light_colour; // 16 bytes
    float TotalTime; // 4 bytes
	float3 packing; // 12 bytes
}; // 128 bytes


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


VOut VShader(VIn input)
{
	VOut output;

    output.position = mul(WVPMatrix, input.position);
    output.texcoord = input.texcoord;

    float diffuse_amount = dot(directional_light, input.normal);
    diffuse_amount = saturate(diffuse_amount);
    output.color = ambient_light_colour + (directional_light_colour * diffuse_amount) + input.color;

	return output;
}

float4 PShader(VOut input) : SV_TARGET
{
    return input.color * texture0.Sample(sampler0, input.texcoord);
}