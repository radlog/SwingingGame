Texture2D texture0;
SamplerState sampler0;

cbuffer CBuffer0
{
    float4x4 WVPMatrix; // 64 bytes
    float4x4 ViewProjection; // 64 bytes
    float4 directional_light_vector;
    float4 directional_light_colour;
    float4 ambient_light_colour;
    float4 rgb_amount;
    float game_time;
	float3 packing; // 12 bytes
    //float scale; // 4 bytes
}; // total 80 bytes


struct VOut
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
    float2 texcoord : TEXCOORD;
};

VOut VShader(float4 position : POSITION, float4 color : COLOR, float2 texcoord : TEXCOORD, float3 normal : NORMAL)
{
    VOut output;

    output.position = mul(WVPMatrix, position);
	
	//color.r *= red_fraction;
	//color.g *= green_fraction;
	//color.b *= blue_fraction;

    float diffuse_amount = dot(directional_light_vector, normal);
    diffuse_amount = saturate(diffuse_amount);
    output.color = ambient_light_colour + (directional_light_colour * diffuse_amount);//    +color;

    output.texcoord = texcoord;

    return output;
}

// NOTE: try to change the name to see if it still works
float4 PShader(float4 position : SV_POSITION, float4 color : COLOR, float2 texcoord : TEXCOORD) : SV_TARGET
{

    //return color; //+ float4(1.0, 1.0, 1.0, 0);
    //texture0.Sample(sampler0, texcoord);
    return color * texture0.Sample(sampler0, texcoord);;
}