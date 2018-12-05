Texture2D texture0;

//sampler2D tex0;
//sampler2D tex1;
//sampler2D noise_texture;

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
    float4 position : POSITION;
    float2 texcoord : TEXCOORD;
    float3 normal : NORMAL;
    float4 color : COLOR;
};


VOut VShader(VIn input)
{
    VOut output;

    float aplitude = 1.0f;
    float frequency = 12.0f;
    float2 noise = sin(TotalTime * frequency) * sin(TotalTime * frequency / 3);


    output.position = mul(WVPMatrix, input.position);
    output.texcoord = input.texcoord;
	//output.texcoord.y += sin(TotalTime/100);

    //output.position.y += abs(noise);
    //output.position.y += abs(sin((TotalTime / 2)) * sin(TotalTime / 10));
    
	//output.position.z += sin(TotalTime  + input.position.y)/100;


    float diffuse_amount = dot(directional_light, input.normal);
    diffuse_amount = saturate(diffuse_amount);
    output.color = ambient_light_colour + (directional_light_colour * diffuse_amount) + input.color;

    return output;
}

float4 PShader(VOut input) : SV_TARGET
{


    //float4 noise = tex2D(tex0, input.texcoord); // sample color map 
    //float2 T1 = input.texcoord + float2(1.5, -1.5) * TotalTime * 0.02;
    //float2 T2 = input.texcoord + float2(-0.5, 2.0) * TotalTime * 0.01;
    //T1.x += (noise.x) * 2.0;
    //T1.y += (noise.y) * 2.0;
    //T2.x += (noise.y) * 0.2;
    //T2.y += (noise.z) * 0.2;

    //float p = tex2D(tex0, T1 * 2.0).a;
    
    //float4 col = tex2D(tex1, T2 * 2.0);
    //float4 temp = col * (float4(p, p, p, p) * 2.0) + (col * col - 0.1);
    //if (temp.r > 1.0)
    //{
    //    temp.bg += clamp(temp.r - 2.0, 0.0, 100.0);
    //}
    //if (temp.g > 1.0)
    //{
    //    temp.rb += temp.g - 1.0;
    //}
    //if (temp.b > 1.0)
    //{
    //    temp.rg += temp.b - 1.0;
    //}

   // return input.color;

   // return temp;

    //output. = temp;



    input.texcoord.x /= 50; // input.texcoord.x*20.0f + sin(TotalTime * 3 + 10) / 2560;
    input.texcoord.y /= 50;
	//input.texcoord.y = input.texcoord.y*20.0f;
	//input.texcoord.y += sin(TotalTime * 300) + TotalTime;
    return input.color * texture0.Sample(sampler0, input.texcoord);
}