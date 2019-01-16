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

    // these vars are a copies from a resource i wanted to learn from, so i just commented it out
    //float3 PointLightPos : packoffset(c0);
    //float PointLightRangeRcp : packoffset(c0.w);
    //float scale; // 4 bytes

}; // total 80 bytes


struct VOut
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
    float2 texcoord : TEXCOORD;
};


// this code is a copy from a resource i wanted to learn from, so i just commented it out

// NOT IN USE

//float3 CalcPoint
//    (
//    float3 position, Material
//    material)
//{
//    float3 ToLight = PointLightPos.xyz - position;
//    float3 ToEye = EyePosition.xyz - position;
//    float DistToLight = length(ToLight);
//// Phong diffuse
//    ToLight /= DistToLight; // Normalize
//    float NDotL = saturate(dot(ToLight, material.normal));
//    float3 finalColor = PointColor.rgb * NDotL;
//// Blinn specular
//    ToEye = normalize(ToEye);

//    float3 HalfWay = normalize(ToEye + ToLight);
//    float NDotH = saturate(dot(HalfWay, material.normal));
//    finalColor += PointColor.rgb * pow(NDotH, material.specExp)
//* material.specIntensity;
//// Attenuation
//    float DistToLightNorm = 1.0 - saturate(DistToLight *
//PointLightRangeRcp);
//    float Attn = DistToLightNorm * DistToLightNorm;
//    finalColor *= material.diffuseColor * Attn;
//    return finalColor;
//}

VOut VShader(float4 position : POSITION, float4 color : COLOR, float2 texcoord : TEXCOORD, float3 normal : NORMAL)
{

  

    VOut output;

    output.position = mul(WVPMatrix, position);

    float3 world_eye_position = position;
    float3 view_position = normalize(mul(position, WVPMatrix));
    float3 look_direction = view_position - world_eye_position;
    
    // PHONG LIFGHTING
    float4 diffuse_amount = saturate(dot(normalize(normal), normalize(directional_light_vector)));
    float3 reflection = normalize(2 * diffuse_amount * normalize(normal) - normalize(directional_light_vector));
    float4 specular = pow(saturate(dot(reflection, look_direction)), 20); // R.V^n

    output.color = ambient_light_colour + directional_light_colour * diffuse_amount + specular;
    output.texcoord = texcoord;

    return output;
}

float4 PShader(float4 position : SV_POSITION, float4 color : COLOR, float2 texcoord : TEXCOORD) : SV_TARGET
{


    return color * texture0.Sample(sampler0, texcoord);;
}