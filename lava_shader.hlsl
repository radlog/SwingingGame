Texture2D texture0 : register(t0);
Texture2D texture1 : register(t1);
Texture2D texture2 : register(t2);

SamplerState sampler0;

cbuffer CB0
{
    matrix WorldViewProjection; // 64 bytes
	matrix ViewProjection; // 64 bytes
    float4 directional_light; // 16 bytes
    float4 directional_light_colour; // 16 bytes
    float4 ambient_light_colour; // 16 bytes
    float TotalTime; // 4 bytes
    float3 packing; // 12 bytes
}; // 192 bytes


struct VOut
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
    float2 texcoord_diffuse : TEXCOORD0;
    float2 texcoord_normal : TEXCOORD1;
    float2 texcoord_noise : TEXCOORD2;
    float3 worldToEye : TEXCOORD3;
};

struct VIn
{
    float4 position : POSITION;
    float2 texcoord : TEXCOORD;
    float3 normal : NORMAL;
    float4 color : COLOR;
};


VOut VShader(VIn input)
{
    VOut output;
    //float aplitude = 1.0f;
    //float frequency = 12.0f;
   // float2 noise = sin(TotalTime * frequency) * sin(TotalTime * frequency / 3);

    float2 tmp = input.texcoord;

    output.position = mul(WorldViewProjection, input.position);
    output.worldToEye = (WorldViewProjection, input.position);

    // map texcoords and move them
    output.texcoord_diffuse = float2(tmp.x, tmp.y)+TotalTime / 10;
    output.texcoord_normal = float2(tmp.x, tmp.y) + TotalTime/10;
    output.texcoord_noise = tmp + TotalTime/2;
    //float2(sin(tmp.x * TotalTime / 10), sin(tmp.y * TotalTime / 10));


    // move the lava vertices themselves
    output.position.y += (abs(sin((TotalTime / 2 + input.position.z)) * sin(TotalTime / 10 + input.position.x))) / 2;
    output.position.z += sin(TotalTime + input.position.y) / 100;
    output.position.x += sin(TotalTime + input.position.y) / 100;


    float diffuse_amount = dot(directional_light, input.normal);
    diffuse_amount = saturate(diffuse_amount);
    output.color = ambient_light_colour + (directional_light_colour * diffuse_amount) + input.color;

    return output;
}

float4 PShader(VOut input) : SV_TARGET
{
    
	input.texcoord_diffuse.x /= 50 + sin(TotalTime);
	input.texcoord_diffuse.y /= 50 + sin(TotalTime);

	input.texcoord_normal.x /= 50 + sin(TotalTime);
	input.texcoord_normal.y /= 50 + sin(TotalTime);

	input.texcoord_noise.x /= 5 + sin(TotalTime);
	input.texcoord_noise.y /= 5 + sin(TotalTime);

    float4 diffuse = texture0.Sample(sampler0, input.texcoord_diffuse);
    float4 normal = texture1.Sample(sampler0, input.texcoord_normal);
    float4 noise = texture2.Sample(sampler0, input.texcoord_noise);// * float4(0.1, 0.1, 0.1, 1);

    //return input.color;
    return input.color * normal * diffuse - noise;
    //return input.color * texture0.Sample(sampler0, input.texcoord_diffuse) * texture1.Sample(sampler0, input.texcoord_noise);
}