Texture2D texture0;
SamplerState sampler0;
TextureCube cube0;

cbuffer CB0
{
    matrix WVPMatrix; // 64 bytes
}; // total = 64 bytes


struct VOut
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
    float3 texcoord : TEXCOORD;
};

VOut VShader(float4 position : POSITION, float3 texcoord : TEXCOORD, float3 normal : NORMAL)
{
    VOut output;

    output.texcoord = position.xyz;
    output.position = mul(WVPMatrix, position);


    return output;
}

float4 PShader(float4 position : SV_POSITION, float4 color : COLOR, float3 texcoord : TEXCOORD) : SV_TARGET
{
    return cube0.Sample(sampler0,texcoord);
}