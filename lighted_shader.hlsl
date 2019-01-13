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

    float3 world_eye_position = position;
    float3 view_position = normalize(mul(position, WVPMatrix));
    float3 look_direction = world_eye_position - view_position;
    //output.color = ambient_light_colour + (directional_light_colour * diffuse_amount);//+color;
    float4 diffuse_amount = saturate(dot(normalize(normal), normalize(directional_light_vector)));
    float3 reflection = normalize(2 * diffuse_amount * normalize(normal) - normalize(directional_light_vector));
    float4 specular = pow(saturate(dot(reflection, look_direction)), 6); // R.V^n

    output.color = ambient_light_colour + directional_light_colour * diffuse_amount + specular;
    //output.color = color;
    output.texcoord = texcoord;

    return output;
}

float4 PShader(float4 position : SV_POSITION, float4 color : COLOR, float2 texcoord : TEXCOORD) : SV_TARGET
{


    return color * texture0.Sample(sampler0, texcoord);;
}