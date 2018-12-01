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

	float aplitude = 1.0f;
	float frequency = 1.0f;

	output.position = mul(WVPMatrix, input.position);
	output.texcoord = input.texcoord;
	//output.texcoord.y += sin(TotalTime/100);
	output.color = input.color;
    output.position.y += abs(sin((TotalTime * 10) + input.position.z) * sin(TotalTime) / 6); 
	output.position.z += sin(TotalTime  + input.position.y)/100;


//float diffuse_amount = dot(directional_light, input.normal);
//diffuse_amount = saturate(diffuse_amount);
//output.color = ambient_light_colour + (directional_light_colour * diffuse_amount) + input.color;

	return output;
}

float4 PShader(VOut input) : SV_TARGET
{
	input.texcoord.x /= 100;// input.texcoord.x*20.0f + sin(TotalTime * 3 + 10) / 2560;
input.texcoord.y /= 100;
	//input.texcoord.y = input.texcoord.y*20.0f;
	//input.texcoord.y += sin(TotalTime * 300) + TotalTime;
	return input.color * texture0.Sample(sampler0, input.texcoord);
}