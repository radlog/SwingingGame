Texture2D texture0;
SamplerState sampler0;

cbuffer CBuffer0
{
	float4x4 WVPMatrix; // 64 bytes
	float4x4 ViewProjection; // 64 bytes
	float4 directional_light_vector;
	float4 directional_light_colour;
	float4 ambient_light_colour;
	float red_fraction; // 4 bytes
	float green_fraction; // 4 bytes
	float blue_fraction; // 4 bytes
	float scale; // 4 bytes
	//float3 packing; // 12 bytes
}; // total 80 bytes


struct VOut
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
	float2 texcoord : TEXCOORD;
};

VOut VShader(float4 position : POSITION, float4 color : COLOR, float2 texcoord : TEXCOORD, float3 normal: NORMAL)
{
	VOut output;


	output.position = mul(WVPMatrix, position);
	
	color.r *= red_fraction;
	color.g *= green_fraction;
	color.b *= blue_fraction;

	float diffuse_amount = dot(directional_light_vector, normal);
	diffuse_amount = saturate(diffuse_amount);
	output.color = ambient_light_colour + (directional_light_colour * diffuse_amount) + color;




	output.texcoord = texcoord;

	return output;
}

// NOTE: try to change the name to see if it still works
float4 PShader(float4 position : SV_POSITION, float4 color : COLOR, float2 texcoord : TEXCOORD) : SV_TARGET
{

	return color * texture0.Sample(sampler0,texcoord);
}